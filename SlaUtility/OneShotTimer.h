
#pragma once

#include <functional>
#include "Delegates.h"

class COneShotTimer : public CWnd
{
public:
   /**
   ctor without event delegate but requires a window signature
   @param[in] sWndSignature is helpful when using the application Spy to locate the one shot timer window.
   */
   COneShotTimer(const CString& sWndSignature);

   /**
   ctor with event delegate
   @param[in] sWndSignature is helpful when using the application Spy to locate the one shot timer window.
   */
   COneShotTimer(const OneShotTickDelegate& oOneShotTickDelegate, const CString& sWndSignature = CString());

   virtual ~COneShotTimer();

   /**
   Sets the event that will be raised with the one shot timer expires.
   */
   void SetDelegate(const OneShotTickDelegate& oOneShotTickDelegate);

   /**
   Starts the 'one shot' timer.
   @note invoke this method from the creator thread otherwise the behavior is undefined according to MSDN.
   */
   void Trigger(DWORD dwPeriod_ms);

private:
   /**
   WM_TIMER event handler.
   */
   afx_msg void OnTimer(UINT nIDEvent);

   /**
   This event is necessary when the dtor of this instance is invoked from a different thread.
   Event handler that will kill the timer and will destroy the window handle on the thread from which it was
   created.
   */
   afx_msg LRESULT OnDestroyOneShot(WPARAM wParam, LPARAM lParam);

   DECLARE_MESSAGE_MAP()

   /**
   Guarantees that the window is destroyed on the same thread from which it was created.
   */
   void Destroy(void);

   /**
   Kills the windows timer.
   */
   void StopTimer(void);

   /**
   Event raised when the windows timer expires.
   */
   OneShotTickDelegate OnOneShotTick;

   /**
   Identifier assigned to the windows timer.
   */
   static const int k_iTimerEventId = 10;

   /**
   Custom message used for destroying this instance when the dtor is invoked from a different thread.
   */
   static const UINT WM_DESTROY_ONE_SHOT_TIMER = WM_USER + 100;

   unsigned m_uTimerHandle;
};