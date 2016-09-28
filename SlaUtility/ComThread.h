#pragma once

#include <functional>
#include <memory>
#include "ComSettings.h"

/**
*/
class CComThread : public CWinThread
{
public:
   typedef std::function<void(int)> ConnectFinishedDelegate;

   DECLARE_DYNCREATE(CComThread)

   CComThread();
   virtual ~CComThread();

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
      Delegate called when the COM connection operation is finished
   */
   ConnectFinishedDelegate OnConnectFinished;

   /**
      Settings passed in when requesting connection
   */
   std::shared_ptr<CComSettings> m_soComSettings;

   /**
   Handle to the COM port to the Arduino
   */
   HANDLE m_hComm;

};