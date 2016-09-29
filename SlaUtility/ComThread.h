
#pragma once

#include <functional>
#include <memory>
#include "ComSettings.h"

/**
   Thread that reads and writes to a COM port
*/
class CComThread : public CWinThread
{
public:
   typedef std::function<void(int)> ConnectFinishedDelegate;
   typedef std::function<void(const CString&, bool)> OutputMsgDelegate;

   DECLARE_DYNCREATE(CComThread)

   CComThread();
   virtual ~CComThread();

   /**
      Synchronous call that sets the delegate that will be called to output a message to the UI
      @param[in] oOutputMsgDelegate delegate called to output a message to the UI
   */
   void SetOutputMsgDelegate(const OutputMsgDelegate& oOutputMsgDelegate);

   /**
      Asynchronous method that establishes a connection to the port selected in the given settings
      @param[in] oConnectFinishedDelegate delegate called when the operation is finished
      @param[in] oComSettings container with all the settings needed to establish communtion with a COM port
   */
   void FireConnect(const ConnectFinishedDelegate& oConnectFinishedDelegate, const CComSettings& oComSettings);

   /**
      Posts message on thread to write to the buffer to the COM port
   */
   void FireWriteBuffer();

protected:
   virtual BOOL InitInstance();
   virtual int ExitInstance();

   DECLARE_MESSAGE_MAP()

private:
   static const int WM_CONNECT = WM_USER + 1;
   static const int WM_WRITE_BUFFER = WM_CONNECT + 1;

   /**
   */
   void OnConnect(WPARAM wParam, LPARAM lParam);

   /**
   */
   void OnWriteBuffer(WPARAM wParam, LPARAM lParam);

   /**
   Opens the COM port based on the selected port from the drop down list
   */
   bool OpenComm(void);

   /**
   Updates the opened COM ports settings

   @return true if successfully updated the COM settings
   */
   bool UpdateCommSettings(void);

   /**
      Delegate called to output messages to the UI
   */
   OutputMsgDelegate OnOutputMsg;

   /**
      Settings passed in when requesting connection
   */
   std::shared_ptr<CComSettings> m_soComSettings;

   /**
   Handle to the COM port to the Arduino
   */
   HANDLE m_hComm;

};