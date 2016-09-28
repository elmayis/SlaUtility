#pragma once

/**
   Wrapper for COM settings that will be passed between threads
*/
struct CComSettings
{
   CComSettings()
   :  m_iPortNumber(-1),
      m_iBaudRate(-1),
      m_iDataBits(-1),
      m_iStopBits(-1),
      m_iParity(-1),
      m_iHandshaking(-1)
   {}

   virtual ~CComSettings()
   {}

   int m_iPortNumber;
   int m_iBaudRate;
   int m_iDataBits;
   int m_iStopBits;
   int m_iParity;
   int m_iHandshaking;
};
