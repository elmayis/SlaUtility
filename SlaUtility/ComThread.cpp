
#include "stdafx.h"
#include <memory>
#include "ComThread.h"

IMPLEMENT_DYNCREATE(CComThread, CWinThread)

CComThread::CComThread()
: OnConnectFinished(NULL)
{

}

CComThread::~CComThread()
{

}

void CComThread::FireConnect(const ConnectFinishedDelegate& oConnectFinishedDelegate)
{
   ConnectFinishedDelegate* poDispatch = new ConnectFinishedDelegate(oConnectFinishedDelegate);
   PostThreadMessage(WM_CONNECT, reinterpret_cast<WPARAM>(poDispatch), 0);
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
   std::shared_ptr<ConnectFinishedDelegate> spoDispatch(reinterpret_cast<ConnectFinishedDelegate*>(wParam));
   OnConnectFinished = *spoDispatch;
   OnConnectFinished(-1);
}

void CComThread::OnWriteBuffer(WPARAM wParam, LPARAM lParam)
{

}
