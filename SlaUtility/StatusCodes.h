
#pragma once

struct CStatusCodes
{
   enum ECodes
   {
      SC_OK = 0,
      SC_OPERATOR_ABORT,
      SC_COM_OPEN_FAILED,              // COM port failed to open
      SC_COM_SETTINGS_FAILED,          // COM failed to update the settings
      SC_COM_READ_THREAD_FAILED,       // Failed to create read COM thread
      SC_COM_WRITE_THREAD_FAILED,      // Failed to create read COM thread
      SC_UNKNOWN,
      END_OF_STATUS_CODES
   };
};
