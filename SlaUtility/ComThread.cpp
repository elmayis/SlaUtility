
#include "stdafx.h"
#include "OneShotTimer.h"
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
      eErrCode = UpdateCommSettings();
      if (CStatusCodes::SC_OK == eErrCode)
      {
         eErrCode = UpdateTimeoutSettings();
         if (CStatusCodes::SC_OK == eErrCode)
         {
            // Start reading the COM
            //
            m_spoOneShotTimer.reset(new COneShotTimer(std::bind(&CComThread::FireBeginRead, this)));
            m_spoOneShotTimer->Trigger(50);
         }
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
   // The pointer will be free'd by the OnWrite message handler.
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
   //FireBeginRead();
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

CStatusCodes::ECodes CComThread::UpdateCommSettings(void)
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
      oDcb.fBinary = 1;
      oDcb.fOutxCtsFlow = 0;
      oDcb.fOutxDsrFlow = 0;
      oDcb.fDtrControl = 1;
      oDcb.fDsrSensitivity = 0;
      oDcb.fTXContinueOnXoff = 1;
      oDcb.fOutX = 0;
      oDcb.fInX = 0;
      oDcb.fErrorChar = 0;
      oDcb.fNull = 0;
      oDcb.fRtsControl = 1;
      oDcb.fAbortOnError = 0;
      oDcb.XonLim = 2048;
      oDcb.XoffLim = 512;
      oDcb.XonChar = 17;                  // '\x11'
      oDcb.XoffChar = 19;                 // '\x13'
      oDcb.ErrorChar = 0;                 // '\x0'
      oDcb.EofChar = 0;                   // '\x0'
      oDcb.EvtChar = 0;                   // '\x0'
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
      if (SetCommState(m_hComm, &oDcb)) return CStatusCodes::SC_OK;

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
   return CStatusCodes::SC_COM_SETTINGS_FAILED;
}

CStatusCodes::ECodes CComThread::UpdateTimeoutSettings(void)
{
   COMMTIMEOUTS oSettings;
   if (!GetCommTimeouts(m_hComm, &oSettings)) return CStatusCodes::SC_COM_GET_TIMEOUT_FAILED;

   oSettings.ReadIntervalTimeout = MAXDWORD;
   //oSettings.ReadIntervalTimeout = 0;
   oSettings.ReadTotalTimeoutMultiplier = 0;
   oSettings.ReadTotalTimeoutConstant = 0;
   oSettings.WriteTotalTimeoutConstant = 0;
   oSettings.WriteTotalTimeoutMultiplier = 0;
   
   return SetCommTimeouts(m_hComm, &oSettings) ? CStatusCodes::SC_OK : CStatusCodes::SC_COM_SET_TIMEOUT_FAILED;
}

void CComThread::FireBeginRead(void)
{
   PostThreadMessage(WM_READ_COM, 0, 0);
}

void CComThread::OnReadCom(WPARAM wParam, LPARAM lParam)
{
   if (m_bAbort) return;

   char pcBuf[512];
   memset(pcBuf, 0, 512);
   DWORD dwBytesRead = 0;
   const BOOL bResult = ReadFile(m_hComm, pcBuf, 512, &dwBytesRead, NULL);
   if (bResult && (0 != dwBytesRead))
   {
      CString sData(pcBuf, dwBytesRead);
      OnOutputMsg(sData, false);
   }
   m_spoOneShotTimer->Trigger(50);
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
