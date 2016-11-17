
#pragma once

#include <functional>
#include <memory>
#include "ComSettings.h"
#include "Delegates.h"
#include "StatusCodes.h"

class COneShotTimer;

/**
   Interface to a COM port
*/
class CComThread : public CWinThread
{
   DECLARE_DYNCREATE(CComThread)
public:
   CComThread();
   virtual ~CComThread();

   /*
   Synchronous call that sets the delegate that will be called to output a message to the UI
   @param[in] oOutputMsgDelegate delegate called to output a message to the UI
   */
   void SetOutputMsgDelegate(const OutputMsgDelegate& oOutputMsgDelegate);

   /*
   Synchronous method that establishes a connection to the port selected in the given settings
   @param[in] oComSettings container with all the settings needed to establish communtion with a COM port

   @return error code
   */
   CStatusCodes::ECodes Connect(const CComSettings& oComSettings);

   /*
   Shuts down the read or write operation, waits for the thread to stop, then closes the handle
   */
   void Disconnect(void);

   /*
   Asynchronously writes the text to the opened COM port using the writing thread
   @param[in] oWriteFinishedDelegate delegate called when the operation is finished
   @param[in] sBuffer contains the text to write to the COM port
   */
   void FireWrite(const WriteFinishedDelegate& oWriteFinishedDelegate, const CString& sBuffer);

protected:
   virtual BOOL InitInstance();
   virtual int ExitInstance();

   DECLARE_MESSAGE_MAP()

private:

   /**
   Opens the COM port based on the selected port from the drop down list
   */
   bool OpenComm(void);

   /**
   Updates the opened COM ports settings

   @return error code
   */
   CStatusCodes::ECodes UpdateCommSettings(void);

   /*
   Retrieves and updates the read and write timeout settings. Disable the timeouts by setting all members to zero.

   @return error code
   */
   CStatusCodes::ECodes UpdateTimeoutSettings(void);

   /**
   Called by InitInstance to begin reading from the COM port
   */
   void FireBeginRead(void);

   /**
   Event handler that loops to read the COM port and then output the text to the UI via OnOutputMsg
   */
   void OnReadCom(WPARAM wParam, LPARAM lParam);

   /*
   Event handler that writes the string buffer to the selected COM
   @param[in] wParam is the WriteFinishedDelegate to call when finished
   @param[in] lParam is the CString buffer
   */
   void OnWriteBuffer(WPARAM wParam, LPARAM lParam);

   /**
      Delegate called to output messages to the UI
   */
   OutputMsgDelegate OnOutputMsg;

   static const int WM_READ_COM = WM_USER + 1;
   static const int WM_WRITE_BUFFER = WM_READ_COM + 1;

   /**
      Settings passed in when requesting connection
   */
   std::shared_ptr<CComSettings> m_spoComSettings;

   /*
   Used to pump the read operation
   */
   std::shared_ptr<COneShotTimer> m_spoOneShotTimer;

   /**
   Handle to the COM port to the Arduino
   */
   HANDLE m_hComm;

   /*
   Flag used by OnWriteBuffer indicating that the operation was aborted
   */
   bool m_bAbort;
};