
#include "stdafx.h"
#include "ComReadThread.h"
#include "ComWriteThread.h"
#include "Com.h"

CCom::CCom(const OutputMsgDelegate& oOutputMsgDelegate)
:  OnOutputMsg(oOutputMsgDelegate),
   m_hComm(NULL)
{
}

CCom::~CCom()
{
   if (NULL != m_hComm)
   {
      CloseHandle(m_hComm);
      m_hComm = NULL;
   }
}

CStatusCodes::ECodes CCom::Connect(const CComSettings& oComSettings)
{
   m_spoComSettings.reset(new CComSettings(oComSettings));

   CStatusCodes::ECodes eErrCode = CStatusCodes::SC_COM_OPEN_FAILED;
   if (OpenComm())
   {
      eErrCode = CStatusCodes::SC_COM_SETTINGS_FAILED;
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
               eErrCode = CStatusCodes::SC_OK;
            }
            else
            {
               eErrCode = CStatusCodes::SC_COM_WRITE_THREAD_FAILED;
               OnOutputMsg("Failed to create the COM write thread.", true);
            }
         }
         else
         {
            eErrCode = CStatusCodes::SC_COM_READ_THREAD_FAILED;
            OnOutputMsg("Failed to create the COM read thread.", true);
         }
      }
   }
   return eErrCode;
}

void CCom::FireWriteBuffer()
{

}

bool CCom::OpenComm(void)
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

bool CCom::UpdateCommSettings(void)
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
      // The default is no flow control
      //
      oDcb.fOutxCtsFlow = false;
      oDcb.fOutxDsrFlow = false;
      oDcb.fRtsControl = false;
      oDcb.fDtrControl = false;
      oDcb.fOutX = false;
      oDcb.fInX = false;
      switch (m_spoComSettings->m_iHandshaking)
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