
#pragma once

struct CStatusCodes
{
   enum ECodes
   {
      SC_OK = 0,
      SC_OPERATOR_ABORT,
      SC_COM_OPEN_FAILED,              // COM port failed to open
      SC_COM_SETTINGS_FAILED,          // COM failed to update the settings
      SC_COM_THREAD_FAILED,            // Failed to create COM thread
      SC_COM_READ_FAILED,              // Failed to read from COM
      SC_COM_WRITE_FAILED,             // WriteFile failed
      SC_COM_WRITE_MISMATCH,           // WriteFile output does not match buffer size
      SC_UNKNOWN,
      END_OF_STATUS_CODES
   };
};
