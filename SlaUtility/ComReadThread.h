
#pragma once

#include "Delegates.h"

/*
   Thread that reads from the COM port
*/
class CComReadThread : public CWinThread
{
public:
   DECLARE_DYNCREATE(CComReadThread)

   CComReadThread();
   virtual ~CComReadThread();

   /*
   Sets the COM's handle
   */
   void SetComHandle(HANDLE hComm);

   /*
   Synchronous call that sets the delegate that will be called to output a message to the UI
   @param[in] oOutputMsgDelegate delegate called to output a message to the UI
   */
   void SetOutputMsgDelegate(const OutputMsgDelegate& oOutputMsgDelegate);

protected:
   virtual BOOL InitInstance();
   virtual int ExitInstance();

   DECLARE_MESSAGE_MAP()

private:
   static const int WM_CONNECT = WM_USER + 1;
   static const int WM_WRITE_BUFFER = WM_CONNECT + 1;

   /*
   Delegate called to output messages to the UI
   */
   OutputMsgDelegate OnOutputMsg;

   /*
   Handle to the COM port
   */
   HANDLE m_hComm;
};