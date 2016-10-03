
#include "stdafx.h"
#include <memory>
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

void CComWriteThread::FireWrite(const WriteFinishedDelegate& oWriteFinishedDelegate, const CString& sBuffer)
{
   // Allocate a copy of the delegate on the heap. This needs to be done so that the raw pointer can be
   // passed to this thread across thread bounderies. The OS layer is only capable of dealing with simple POD.
   // The pointer will be free'd by the OnConnect message handler.
   //
   WriteFinishedDelegate* poDispatch = new WriteFinishedDelegate(oWriteFinishedDelegate);
   CString* poBuffer = new CString(sBuffer);
   PostThreadMessage(WM_WRITE_BUFFER, reinterpret_cast<WPARAM>(poDispatch), reinterpret_cast<LPARAM>(poBuffer));
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
   (*spoDispatch)(eErrCode);
}
