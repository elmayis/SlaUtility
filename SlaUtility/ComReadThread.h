
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

   /*
   Aborts the read operation
   */
   void Abort(void);

protected:
   virtual BOOL InitInstance();
   virtual int ExitInstance();

   DECLARE_MESSAGE_MAP()

private:
   static const int WM_READ_COM = WM_USER + 1;

   /**
      Called by InitInstance to begin reading from the COM port
   */
   void FireBeginRead(void);

   /**
   Event handler that loops to read the COM port and then output the text to the UI via OnOutputMsg
   */
   void OnReadCom(WPARAM wParam, LPARAM lParam);

   /*
   Delegate called to output messages to the UI
   */
   OutputMsgDelegate OnOutputMsg;

   /*
   Handle to the COM port
   */
   HANDLE m_hComm;

   /**
      Flag used to exit the read loop
   */
   bool m_bExitLoop;
};