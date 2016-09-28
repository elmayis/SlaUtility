
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
   // Allocate a copy of the delegate on the heap. This needs to be done so that the raw pointer can be
   // passed to this thread across thread bounderies. The OS layer is only capable of dealing with simple POD.
   // The pointer will be free'd by the OnConnect message handler.
   //
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
   // Cast in the ConnectFinishedDelegate handle. It is required that this thing be on the heap. Wrap it in a shared_ptr
   // so that it is destroyed upon function exit.
   //
   std::shared_ptr<ConnectFinishedDelegate> spoDispatch(reinterpret_cast<ConnectFinishedDelegate*>(wParam));
   OnConnectFinished = *spoDispatch;
   OnConnectFinished(-1);
}

void CComThread::OnWriteBuffer(WPARAM wParam, LPARAM lParam)
{

}
