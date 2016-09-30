
#include "stdafx.h"
#include "StatusCodes.h"
#include "ComWriteThread.h"

IMPLEMENT_DYNCREATE(CComWriteThread, CWinThread)

CComWriteThread::CComWriteThread()
:  m_hComm(NULL)
{
}

CComWriteThread::~CComWriteThread()
{
}

void CComWriteThread::SetComHandle(HANDLE hComm)
{
   m_hComm = hComm;
}

void CComWriteThread::SetOutputMsgDelegate(const OutputMsgDelegate& oOutputMsgDelegate)
{
   OnOutputMsg = oOutputMsgDelegate;
}

void CComWriteThread::FireWriteBuffer()
{
   PostThreadMessage(WM_WRITE_BUFFER, 0, 0);
}

BEGIN_MESSAGE_MAP(CComWriteThread, CWinThread)
   ON_THREAD_MESSAGE(WM_WRITE_BUFFER, OnWriteBuffer)
END_MESSAGE_MAP()

BOOL CComWriteThread::InitInstance()
{
   return TRUE;
}

int CComWriteThread::ExitInstance()
{
   return 0;
}

void CComWriteThread::OnWriteBuffer(WPARAM wParam, LPARAM lParam)
{

}
