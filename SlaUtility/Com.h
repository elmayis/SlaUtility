
#pragma once

#include <functional>
#include <memory>
#include "ComSettings.h"
#include "Delegates.h"
#include "StatusCodes.h"

class CComReadThread;
class CComWriteThread;

/**
   Interface to a COM port
*/
class CCom
{
public:
   CCom(const OutputMsgDelegate& oOutputMsgDelegate);
   virtual ~CCom();

   /**
      Synchronous method that establishes a connection to the port selected in the given settings
      @param[in] oComSettings container with all the settings needed to establish communtion with a COM port

      @return error code
   */
   CStatusCodes::ECodes Connect(const CComSettings& oComSettings);

   /**
      Posts message on thread to write to the buffer to the COM port
   */
   void FireWriteBuffer();

private:

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

   /*
   Thread that handles reading from the COM port
   */
   std::shared_ptr<CComReadThread> m_spoComReadThread;

   /*
   Thread that handles writing to the COM port
   */
   std::shared_ptr<CComWriteThread> m_spoComWriteThread;

   /**
      Settings passed in when requesting connection
   */
   std::shared_ptr<CComSettings> m_spoComSettings;

   /**
   Handle to the COM port to the Arduino
   */
   HANDLE m_hComm;

};