#pragma once

/**
*/
class CComThread : public CWinThread
{
public:
   DECLARE_DYNCREATE(CComThread)

   CComThread();
   virtual ~CComThread();

   /**
      Posts message on thread to write to the buffer to the COM port
   */
   void FireWriteBuffer();

protected:
   virtual BOOL InitInstance();
   virtual int ExitInstance();

   DECLARE_MESSAGE_MAP()

private:
   static const int WM_WRITE_BUFFER = WM_USER + 1;
};