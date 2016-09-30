
#pragma once

#include <functional>

/*
Delegate declarations
*/

typedef std::function<void(const CString&, bool)> OutputMsgDelegate;
typedef std::function<void(int)> ConnectFinishedDelegate;
