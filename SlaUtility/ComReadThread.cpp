
#include "stdafx.h"
#include "StatusCodes.h"
#include "ComReadThread.h"

IMPLEMENT_DYNCREATE(CComReadThread, CWinThread)

CComReadThread::CComReadThread()
:  m_hComm(NULL)
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

//void CComReadThread::FireConnect(const ConnectFinishedDelegate& oConnectFinishedDelegate, const CComSettings& oComSettings)
//{
//   // Allocate a copy of the delegate on the heap. This needs to be done so that the raw pointer can be
//   // passed to this thread across thread bounderies. The OS layer is only capable of dealing with simple POD.
//   // The pointer will be free'd by the OnConnect message handler.
//   //
//   ConnectFinishedDelegate* poDispatch = new ConnectFinishedDelegate(oConnectFinishedDelegate);
//   CComSettings* poSettings = new CComSettings(oComSettings);
//   PostThreadMessage(WM_CONNECT, reinterpret_cast<WPARAM>(poDispatch), reinterpret_cast<LPARAM>(poSettings));
//}
//
//void CComReadThread::FireWriteBuffer()
//{
//   PostThreadMessage(WM_WRITE_BUFFER, 0, 0);
//}

BEGIN_MESSAGE_MAP(CComReadThread, CWinThread)
   //ON_THREAD_MESSAGE(WM_CONNECT, OnConnect)
   //ON_THREAD_MESSAGE(WM_WRITE_BUFFER, OnWriteBuffer)
END_MESSAGE_MAP()

BOOL CComReadThread::InitInstance()
{
   return TRUE;
}

int CComReadThread::ExitInstance()
{
   return 0;
}

//void CComReadThread::OnConnect(WPARAM wParam, LPARAM lParam)
//{
//   // Cast in the ConnectFinishedDelegate handle. It is required that this thing be on the heap. Wrap it in a shared_ptr
//   // so that it is destroyed upon function exit.
//   //
//   std::shared_ptr<ConnectFinishedDelegate> spoDispatch(reinterpret_cast<ConnectFinishedDelegate*>(wParam));
//   m_soComSettings.reset(reinterpret_cast<CComSettings*>(lParam));
//
//   int iErrCode = CStatusCodes::SC_COM_OPEN_FAILED;
//   if (OpenComm())
//   {
//      iErrCode = CStatusCodes::SC_COM_SETTINGS_FAILED;
//      if (UpdateCommSettings())
//      {
//         iErrCode = CStatusCodes::SC_OK;
//      }
//   }
//   (*spoDispatch)(iErrCode);
//}
