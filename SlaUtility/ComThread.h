#pragma once

/**
*/
class CComThread : public CWinThread
{
public:
   typedef void(*ConnectFinishedDelegate)(int);

   DECLARE_DYNCREATE(CComThread)

   CComThread();
   virtual ~CComThread();

   void FireConnect(ConnectFinishedDelegate oConnectFinishedDelegate);

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

   ConnectFinishedDelegate OnConnectFinished;
};