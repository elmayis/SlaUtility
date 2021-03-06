
#pragma once

#include <functional>
#include "StatusCodes.h"

/*
Delegate declarations
*/

typedef std::function<void(const CString&, bool)> OutputMsgDelegate;
typedef std::function<void(int)> ConnectFinishedDelegate;
typedef std::function<void(CStatusCodes::ECodes)> WriteFinishedDelegate;
typedef std::function<void(void)> OneShotTickDelegate;
