
#include "stdafx.h"
#include "ComThread.h"

IMPLEMENT_DYNCREATE(CComThread, CWinThread)

CComThread::CComThread()
:  m_hComm(NULL),
   m_bAbort(false)
{
}

CComThread::~CComThread()
{
   Disconnect();
}

void CComThread::SetOutputMsgDelegate(const OutputMsgDelegate& oOutputMsgDelegate)
{
   OnOutputMsg = oOutputMsgDelegate;
}

CStatusCodes::ECodes CComThread::Connect(const CComSettings& oComSettings)
{
   m_spoComSettings.reset(new CComSettings(oComSettings));

   CStatusCodes::ECodes eErrCode = CStatusCodes::SC_COM_OPEN_FAILED;
   if (OpenComm())
   {
      eErrCode = CStatusCodes::SC_COM_SETTINGS_FAILED;
      if (UpdateCommSettings())
      {
         //m_spoComReadThread.reset(dynamic_cast<CComReadThread*>(AfxBeginThread(RUNTIME_CLASS(CComReadThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED)));
         //if (NULL != m_spoComReadThread)
         //{
         //   // Do not allow the thread to delete itself because we will do it manually via the smart pointer
         //   //
         //   m_spoComReadThread->m_bAutoDelete = FALSE;
         //   m_spoComWriteThread.reset(dynamic_cast<CComWriteThread*>(AfxBeginThread(RUNTIME_CLASS(CComWriteThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED)));
         //   if (NULL != m_spoComWriteThread)
         //   {
         //      // Do not allow the thread to delete itself because we will do it manually via the smart pointer
         //      //
         //      m_spoComWriteThread->m_bAutoDelete = FALSE;
         //      m_spoComReadThread->SetComHandle(m_hComm);
         //      m_spoComReadThread->SetOutputMsgDelegate(OnOutputMsg);
         //      m_spoComWriteThread->SetComHandle(m_hComm);
         //      m_spoComWriteThread->SetOutputMsgDelegate(OnOutputMsg);
         //      // Start the threads pumping
         //      //
         //      m_spoComReadThread->ResumeThread();
         //      m_spoComWriteThread->ResumeThread();
         //      eErrCode = CStatusCodes::SC_OK;
         //   }
         //   else
         //   {
         //      eErrCode = CStatusCodes::SC_COM_WRITE_THREAD_FAILED;
         //      OnOutputMsg("Failed to create the COM write thread.", true);
         //   }
         //}
         //else
         //{
         //   eErrCode = CStatusCodes::SC_COM_READ_THREAD_FAILED;
         //   OnOutputMsg("Failed to create the COM read thread.", true);
         //}
      }
   }
   return eErrCode;
}

void CComThread::Disconnect(void)
{
   // Cancels pending IO on this thread
   //
   CancelSynchronousIo(this->m_hThread);
   ::PostThreadMessage(this->m_nThreadID, WM_QUIT, 0, 0);
   // In debug mode, we are much stricter about waiting for the thread to shutdown.
   // Issue an error if it times out.
   //
   if (WAIT_TIMEOUT == ::MsgWaitForMultipleObjects(1, &this->m_hThread, FALSE, 5000, 0))
   {
      _RPTF1(_CRT_ERROR, "The '%s' thread did not shutdown in the time alloted.", "read");
   }
   if (NULL != m_hComm)
   {
      CloseHandle(m_hComm);
      m_hComm = NULL;
   }
}

void CComThread::FireWrite(const WriteFinishedDelegate& oWriteFinishedDelegate, const CString& sBuffer)
{
   // Allocate a copy of the delegate on the heap. This needs to be done so that the raw pointer can be
   // passed to this thread across thread bounderies. The OS layer is only capable of dealing with simple POD.
   // The pointer will be free'd by the OnConnect message handler.
   //
   WriteFinishedDelegate* poDispatch = new WriteFinishedDelegate(oWriteFinishedDelegate);
   CString* poBuffer = new CString(sBuffer);
   PostThreadMessage(WM_WRITE_BUFFER, reinterpret_cast<WPARAM>(poDispatch), reinterpret_cast<LPARAM>(poBuffer));
}

BEGIN_MESSAGE_MAP(CComThread, CWinThread)
   ON_THREAD_MESSAGE(WM_READ_COM, OnReadCom)
   ON_THREAD_MESSAGE(WM_WRITE_BUFFER, OnWriteBuffer)
END_MESSAGE_MAP()

BOOL CComThread::InitInstance()
{
   // This will start the reading loop
   //
   FireBeginRead();
   return TRUE;
}

int CComThread::ExitInstance()
{
   return 0;
}

bool CComThread::OpenComm(void)
{
   CString sCommPort;
   // Need to add the text "COM" to the port number
   //
   sCommPort.Format("\\\\.\\COM%d", m_spoComSettings->m_iPortNumber);
   m_hComm =
      CreateFile(sCommPort,
         GENERIC_READ | GENERIC_WRITE,
         0,                //  must be opened with exclusive-access
         NULL,             //  default security attributes
         OPEN_EXISTING,    //  must use OPEN_EXISTING
         0,                //  not overlapped I/O
         NULL);            //  hTemplate must be NULL for comm devices
   if (INVALID_HANDLE_VALUE != m_hComm) return true;

   CString sMsg;
   sMsg.Format("CreateFile failed with Windows error code %d.\n", GetLastError());
   OnOutputMsg(sMsg, true);
   return false;
}

bool CComThread::UpdateCommSettings(void)
{
   DCB oDcb;
   //  Initialize the DCB structure.
   //
   SecureZeroMemory(&oDcb, sizeof(DCB));
   oDcb.DCBlength = sizeof(DCB);
   // Get the current state of the COM so that we can build upon it.
   //
   BOOL bSuccess = GetCommState(m_hComm, &oDcb);
   if (bSuccess)
   {
      oDcb.BaudRate = m_spoComSettings->m_iBaudRate;
      oDcb.ByteSize = m_spoComSettings->m_iDataBits;
      oDcb.StopBits = m_spoComSettings->m_iStopBits;
      oDcb.Parity = m_spoComSettings->m_iParity;

      // The following is the default flow control
      //
      oDcb.fOutxCtsFlow = false;
      oDcb.fOutxDsrFlow = false;
      oDcb.fRtsControl = false;
      oDcb.fDtrControl = true;
      oDcb.fRtsControl = true;
      oDcb.fTXContinueOnXoff = true;
      oDcb.fOutX = false;
      oDcb.fInX = false;
      // @todo need to figure this out sometime
      //
      //switch (m_spoComSettings->m_iHandshaking)
      //{
      //case 1:
      //{
      //   oDcb.fOutX = true;
      //   oDcb.fInX = true;
      //   break;
      //}
      //case 2:
      //{
      //   oDcb.fRtsControl = true;
      //   break;
      //}
      //case 3:
      //{
      //   oDcb.fRtsControl = true;
      //   oDcb.fOutX = true;
      //   oDcb.fInX = true;
      //   break;
      //}
      //}
      if (SetCommState(m_hComm, &oDcb)) return true;

      CString sMsg;
      sMsg.Format("SetCommState failed with error %d.\n", GetLastError());
      OnOutputMsg(sMsg, true);
   }
   else
   {
      CString sMsg;
      sMsg.Format("GetCommState failed with error %d.\n", GetLastError());
      OnOutputMsg(sMsg, true);
   }
   return false;
}

void CComThread::FireBeginRead(void)
{
   PostThreadMessage(WM_READ_COM, 0, 0);
}

void CComThread::OnReadCom(WPARAM wParam, LPARAM lParam)
{
   //mayis
   //while (!m_bExitLoop)
   //{
      char pcBuf[512];
      DWORD dwBytesRead = 0;
      const BOOL bResult = ReadFile(m_hComm, pcBuf, 512, &dwBytesRead, NULL);
      //if (!m_bExitLoop && bResult)
      {
         CString sData(pcBuf, dwBytesRead);
         OnOutputMsg(sData, false);
      }
   //}
}

void CComThread::OnWriteBuffer(WPARAM wParam, LPARAM lParam)
{
   if (m_bAbort) return;

   // Cast in the WriteFinishedDelegate handle. It is required that this thing be on the heap. Wrap it in a shared_ptr
   // so that it is destroyed upon function exit.
   //
   std::shared_ptr<WriteFinishedDelegate> spoDispatch(reinterpret_cast<WriteFinishedDelegate*>(wParam));
   std::shared_ptr<CString> spoBuffer(reinterpret_cast<CString*>(lParam));

   DWORD dwBytesWritten = 0;
   CStatusCodes::ECodes eErrCode = CStatusCodes::SC_COM_WRITE_FAILED;
   if (WriteFile(m_hComm, spoBuffer->GetBuffer(), spoBuffer->GetLength(), &dwBytesWritten, NULL))
   {
      eErrCode =
         (spoBuffer->GetLength() == dwBytesWritten) ? CStatusCodes::SC_OK : CStatusCodes::SC_COM_WRITE_MISMATCH;
   }
   if (!m_bAbort)
   {
      (*spoDispatch)(eErrCode);
   }
}
