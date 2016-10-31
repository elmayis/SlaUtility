
#include "stdafx.h"
#include "OneShotTimer.h"

COneShotTimer::COneShotTimer(const CString& sSignature)
   : m_uTimerHandle(0)
{
   const CString sDebugSignature = sSignature.IsEmpty() ? CString() : (" (" + sSignature + ")");
   if (!CreateEx(0, ::AfxRegisterWndClass(0), "COneShotTimer" + sDebugSignature, 0, 0, 0, 0, 0, NULL, NULL))
   {
      throw "COneShotTimer creation failed";
   }
}

COneShotTimer::COneShotTimer(const OneShotTickDelegate& oOneShotTickDelegate, const CString& sSignature)
   : OnOneShotTick(oOneShotTickDelegate),
   m_uTimerHandle(0)
{
   const CString sDebugSignature = sSignature.IsEmpty() ? CString() : (" (" + sSignature + ")");
   if (!CreateEx(0, ::AfxRegisterWndClass(0), "COneShotTimer" + sDebugSignature, 0, 0, 0, 0, 0, NULL, NULL))
   {
      throw "COneShotTimer creation failed";
   }
}

COneShotTimer::~COneShotTimer()
{
   // Check if the window still exist otherwise there is no need to destroy the window handle.
   //
   if (::IsWindow(GetSafeHwnd()))
   {
      Destroy();
   }
}

void COneShotTimer::SetDelegate(const OneShotTickDelegate& oOneShotTickDelegate)
{
   OnOneShotTick = oOneShotTickDelegate;
}

void COneShotTimer::Trigger(DWORD dwPeriod_ms)
{
   // If the timer was already started, calling SetTimer again will stop the previous timer and start a new
   // timer with the new time interval.
   //
   if (!(m_uTimerHandle = SetTimer(k_iTimerEventId, dwPeriod_ms, NULL)))
   {
      throw "COneShotTimer Windows timer could not be set.";
   }
}

BEGIN_MESSAGE_MAP(COneShotTimer, CWnd)
   ON_WM_TIMER()
   ON_MESSAGE(WM_DESTROY_ONE_SHOT_TIMER, OnDestroyOneShot)
END_MESSAGE_MAP()

void COneShotTimer::OnTimer(UINT nIDEvent)
{
   if (k_iTimerEventId == nIDEvent)
   {
      StopTimer();
      OnOneShotTick();
   }
   else
   {
      CWnd::OnTimer(nIDEvent);
   }
}

LRESULT COneShotTimer::OnDestroyOneShot(WPARAM wParam, LPARAM lParam)
{
   StopTimer();
   DestroyWindow();
   return 0;
}

void COneShotTimer::Destroy(void)
{
   // According to MSDN, if the specified window was created by the calling thread, the window procedure is called
   // immediately as a subroutine. If the specified window was created by a different thread, the system switches to
   // that thread and calls the appropriate window procedure. The sending thread is blocked until the receiving thread
   // processes the message.
   //
   ::SendMessage(m_hWnd, WM_DESTROY_ONE_SHOT_TIMER, 0, 0);
}

void COneShotTimer::StopTimer(void)
{
   if (m_uTimerHandle)
   {
      KillTimer(m_uTimerHandle);
      m_uTimerHandle = 0;
   }
}
