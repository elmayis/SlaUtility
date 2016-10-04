
#include "stdafx.h"
#include "StatusCodes.h"
#include "ComReadThread.h"

IMPLEMENT_DYNCREATE(CComReadThread, CWinThread)

CComReadThread::CComReadThread()
:  m_hComm(NULL),
   m_bExitLoop(false)
{
}

CComReadThread::~CComReadThread()
{
}

void CComReadThread::SetComHandle(HANDLE hComm)
{
   m_hComm = hComm;
}

void CComReadThread::SetOutputMsgDelegate(const OutputMsgDelegate& oOutputMsgDelegate)
{
   OnOutputMsg = oOutputMsgDelegate;
}

void CComReadThread::Abort(void)
{
   m_bExitLoop = true;
   CancelSynchronousIo(this->m_hThread);
}

BEGIN_MESSAGE_MAP(CComReadThread, CWinThread)
   ON_THREAD_MESSAGE(WM_READ_COM, OnReadCom)
END_MESSAGE_MAP()

BOOL CComReadThread::InitInstance()
{
   return TRUE;
}

int CComReadThread::ExitInstance()
{
   return 0;
}

void CComReadThread::OnReadCom(WPARAM wParam, LPARAM lParam)
{
   while (!m_bExitLoop)
   {
      char pcBuf[512];
      DWORD dwBytesRead = 0;
      const BOOL bResult = ReadFile(m_hComm, pcBuf, 512, &dwBytesRead, NULL);
      if (bResult)
      {
         CString sData(pcBuf, dwBytesRead);
         OnOutputMsg(sData, false);
      }
   }
}

void CComReadThread::FireBeginRead(void)
{
   PostThreadMessage(WM_READ_COM, 0, 0);
}