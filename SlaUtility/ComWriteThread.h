
#pragma once

#include "Delegates.h"

/**
   Thread that writes to a COM port
*/
class CComWriteThread : public CWinThread
{
public:
   DECLARE_DYNCREATE(CComWriteThread)

   CComWriteThread();
   virtual ~CComWriteThread();

   /*
   Sets the COM's handle
   */
   void SetComHandle(HANDLE hComm);

   /*
   Synchronous call that sets the delegate that will be called to output a message to the UI
   @param[in] oOutputMsgDelegate delegate called to output a message to the UI
   */
   void SetOutputMsgDelegate(const OutputMsgDelegate& oOutputMsgDelegate);

   /*
   Posts message on thread to write to the buffer to the COM port
   @param[in] oWriteFinishedDelegate delegate called when the operation is finished
   @param[in] sBuffer contains the text to write to the COM port
   */
   void FireWrite(const WriteFinishedDelegate& oWriteFinishedDelegate, const CString& sBuffer);

   /*
   Aborts the write operation
   */
   void Abort(void);

protected:
   virtual BOOL InitInstance();
   virtual int ExitInstance();

   DECLARE_MESSAGE_MAP()

private:
   /*
   */
   void OnWriteBuffer(WPARAM wParam, LPARAM lParam);

   static const int WM_WRITE_BUFFER = WM_USER + 1;

   /*
   Delegate called to output messages to the UI
   */
   OutputMsgDelegate OnOutputMsg;

   /*
   Handle to the COM port
   */
   HANDLE m_hComm;

   /*
   Flag used by OnWriteBuffer indicating that the operation was aborted
   */
   bool m_bAbort;
};