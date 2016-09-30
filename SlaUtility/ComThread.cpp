
#include "stdafx.h"
#include "StatusCodes.h"
#include "ComReadThread.h"
#include "ComWriteThread.h"
#include "ComThread.h"

IMPLEMENT_DYNCREATE(CComThread, CWinThread)

CComThread::CComThread()
:  m_hComm(NULL)
{
}

CComThread::~CComThread()
{
   if (NULL != m_hComm)
   {
      CloseHandle(m_hComm);
      m_hComm = NULL;
   }
}

void CComThread::SetOutputMsgDelegate(const OutputMsgDelegate& oOutputMsgDelegate)
{
   OnOutputMsg = oOutputMsgDelegate;
}

void CComThread::FireConnect(const ConnectFinishedDelegate& oConnectFinishedDelegate, const CComSettings& oComSettings)
{
   // Allocate a copy of the delegate on the heap. This needs to be done so that the raw pointer can be
   // passed to this thread across thread bounderies. The OS layer is only capable of dealing with simple POD.
   // The pointer will be free'd by the OnConnect message handler.
   //
   ConnectFinishedDelegate* poDispatch = new ConnectFinishedDelegate(oConnectFinishedDelegate);
   CComSettings* poSettings = new CComSettings(oComSettings);
   PostThreadMessage(WM_CONNECT, reinterpret_cast<WPARAM>(poDispatch), reinterpret_cast<LPARAM>(poSettings));
}

void CComThread::FireWriteBuffer()
{
   PostThreadMessage(WM_WRITE_BUFFER, 0, 0);
}

BEGIN_MESSAGE_MAP(CComThread, CWinThread)
   ON_THREAD_MESSAGE(WM_CONNECT, OnConnect)
   ON_THREAD_MESSAGE(WM_WRITE_BUFFER, OnWriteBuffer)
END_MESSAGE_MAP()

BOOL CComThread::InitInstance()
{
   return TRUE;
}

int CComThread::ExitInstance()
{
   return 0;
}

void CComThread::OnConnect(WPARAM wParam, LPARAM lParam)
{
   // Cast in the ConnectFinishedDelegate handle. It is required that this thing be on the heap. Wrap it in a shared_ptr
   // so that it is destroyed upon function exit.
   //
   std::shared_ptr<ConnectFinishedDelegate> spoDispatch(reinterpret_cast<ConnectFinishedDelegate*>(wParam));
   m_soComSettings.reset(reinterpret_cast<CComSettings*>(lParam));

   int iErrCode = CStatusCodes::SC_COM_OPEN_FAILED;
   if (OpenComm())
   {
      iErrCode = CStatusCodes::SC_COM_SETTINGS_FAILED;
      if (UpdateCommSettings())
      {
         m_spoComReadThread.reset(dynamic_cast<CComReadThread*>(AfxBeginThread(RUNTIME_CLASS(CComReadThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED)));
         if (NULL != m_spoComReadThread)
         {
            m_spoComWriteThread.reset(dynamic_cast<CComWriteThread*>(AfxBeginThread(RUNTIME_CLASS(CComWriteThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED)));
            if (NULL != m_spoComWriteThread)
            {
               m_spoComReadThread->SetComHandle(m_hComm);
               m_spoComReadThread->SetOutputMsgDelegate(OnOutputMsg);
               m_spoComWriteThread->SetComHandle(m_hComm);
               m_spoComWriteThread->SetOutputMsgDelegate(OnOutputMsg);
               // Start the threads pumping
               //
               m_spoComReadThread->ResumeThread();
               m_spoComWriteThread->ResumeThread();
               iErrCode = CStatusCodes::SC_OK;
            }
            else
            {
               iErrCode = CStatusCodes::SC_COM_WRITE_THREAD_FAILED;
               OnOutputMsg("Failed to create the COM write thread.", true);
            }
         }
         else
         {
            iErrCode = CStatusCodes::SC_COM_READ_THREAD_FAILED;
            OnOutputMsg("Failed to create the COM read thread.", true);
         }
      }
   }
   (*spoDispatch)(iErrCode);
}

void CComThread::OnWriteBuffer(WPARAM wParam, LPARAM lParam)
{

}

bool CComThread::OpenComm(void)
{
   CString sCommPort;
   // Need to add the text "COM" to the port number
   //
   sCommPort.Format("\\\\.\\COM%d", m_soComSettings->m_iPortNumber);
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
      oDcb.BaudRate = m_soComSettings->m_iBaudRate;
      oDcb.ByteSize = m_soComSettings->m_iDataBits;
      oDcb.StopBits = m_soComSettings->m_iStopBits;
      oDcb.Parity = m_soComSettings->m_iParity;
      // The default is no flow control
      //
      oDcb.fOutxCtsFlow = false;
      oDcb.fOutxDsrFlow = false;
      oDcb.fRtsControl = false;
      oDcb.fDtrControl = false;
      oDcb.fOutX = false;
      oDcb.fInX = false;
      switch (m_soComSettings->m_iHandshaking)
      {
      case 1:
      {
         oDcb.fOutX = true;
         oDcb.fInX = true;
         break;
      }
      case 2:
      {
         oDcb.fRtsControl = true;
         break;
      }
      case 3:
      {
         oDcb.fRtsControl = true;
         oDcb.fOutX = true;
         oDcb.fInX = true;
         break;
      }
      }
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