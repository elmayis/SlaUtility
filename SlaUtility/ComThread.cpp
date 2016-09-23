
#include "stdafx.h"
#include "ComThread.h"

IMPLEMENT_DYNCREATE(CComThread, CWinThread)

CComThread::CComThread()
{

}

CComThread::~CComThread()
{

}

void CComThread::FireWriteBuffer()
{
   PostThreadMessage(WM_WRITE_BUFFER, 0, 0);
}

BEGIN_MESSAGE_MAP(CComThread, CWinThread)
END_MESSAGE_MAP()

BOOL CComThread::InitInstance()
{
   return TRUE;
}

int CComThread::ExitInstance()
{
   return 0;
}

