
// SlaUtilityDlg.h : header file
//

#pragma once

#include <memory>
#include <fstream>
#include "ComSettings.h"

class CComThread;

// CSlaUtilityDlg dialog
class CSlaUtilityDlg : public CDialogEx
{
// Construction
public:
	CSlaUtilityDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SLAUTILITY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

   afx_msg void OnBnClickedButtonSerialPorts();
   afx_msg void OnBnClickedButtonConnect();
   afx_msg void OnBnClickedButtonDisconnect();
   afx_msg void OnBnClickedLoadFile();
   afx_msg void OnBnClickedDownload();
   afx_msg void OnBnClickedSend();
   afx_msg void OnBnClickedClear();

   afx_msg void OnCbnSelchangeComboPortNumber();
   afx_msg void OnCbnSelchangeComboBaudRate();
   afx_msg void OnCbnSelchangeComboDataBits();
   afx_msg void OnCbnSelchangeComboStopBits();
   afx_msg void OnCbnSelchangeComboParity();
   afx_msg void OnCbnSelchangeComboHandshaking();

   /**
      Event handler that is called to output a message to the rich edit control
      @param[in] wParam will contain the true or false value
      @param[in] lParam will contain the raw pointer to a CString containing the message
   */
   afx_msg LRESULT OnOutputMsg(WPARAM wParam, LPARAM lParam);

   /**
   Event handler that is called when the write operation is finished for the manual command
   @param[in] wParam will contain the error code
   */
   afx_msg LRESULT OnManualCmdWriteFinished(WPARAM wParam, LPARAM lParam);

   /**
   Event handler that is called when the next write operation is to be executed
   */
   afx_msg LRESULT OnWriteNextFileBuffer(WPARAM wParam, LPARAM lParam);

   DECLARE_MESSAGE_MAP()

private:
   /**
      Validates all of the controls that are required for COM.
      @param[out] updated with the valid COM entries

      @return true if valid
   */
   bool IsCommEntriesValid(CComSettings& oComSettings) const;

   /**
   Opens the COM then updates the UI controls
   */
   void OpenCom(const CComSettings& oComSettings);

   /**
      @return COM port number selected in the control. A -1 indicates it is not selected.
   */
   int GetSelectedPortNumber(void) const;

   /**
      @return baud rate selected in the control. A -1 indicates it is not selected.
   */
   int GetSelectedBaudRate(void) const;

   /**
      @return data bits selected in the control. A -1 indicates it is not selected.
   */
   int GetSelectedDataBits(void) const;

   /**
      @return stop bits selected in the control (ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS). A -1 indicates it is not selected.
   */
   int GetSelectedStopBits(void) const;

   /**
      @return parity selected in the control (NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY). A -1 indicates it is not selected.
   */
   int GetSelectedParity(void) const;

   /**
      @return handshaking selected in the control (0 = None, 1 = Xoff/Xon, 2 = RTS Handshake, 3 = RTS Handshake/Xoff/Xon). A -1 indicates it is not selected.
   */
   int GetSelectedHandshaking(void) const;

   /**
      Adds content to each combobox control
   */
   void AddItemsToComboBoxes(void);

   /**
      Retrieves data from the registry and applies to the drop down controls
   */
   void InitControlsFromRegistry(void);

   /**
      Retrieves the value from the registry and applies it to the control
   */
   void InitBaudRateFromRegistry(HKEY hKey);

   /**
      Retrieves the value from the registry and applies it to the control
   */
   void InitDataBitsFromRegistry(HKEY hKey);

   /**
      Retrieves the value from the registry and applies it to the control
   */
   void InitStopBitsFromRegistry(HKEY hKey);

   /**
      Retrieves the value from the registry and applies it to the control
   */
   void InitParityFromRegistry(HKEY hKey);

   /**
      Retrieves the value from the registry and applies it to the control
   */
   void InitHandshakingFromRegistry(HKEY hKey);

   /**
      Retrieves the port number from the registry

      @return port number or empty string if does not exist
   */
   CString RetrievePortNumber(void);

   /**
      Creates the subkey if it does not already exist and then opens the key for read/write access
   */
   HKEY GetAppSubkey(void);

   /**
      Enables the controls after the dialog is presented.
   */
   void EnableInitialControls(void);

   /**
   Enables the controls after the serial ports are enumerated
   */
   void EnableEnumComControls(void);

   /**
   Enables the controls after connecting to the serial port
   */
   void EnableConnectedControls(void);

   /**
      Enables/disables all of the controls
   */
   void EnableAllControls(bool bEnable);

   /**
      Outputs the message to the output window. Appends a new line to the message.
      @param[in] sMsg message to output to the control
      @param[in] bPresentModal true to present a modal dialog with the message. Default is false.
   */
   void OutputMessage(const CString& sMsg, bool bPresentModal = false);

   /**
      Called from the COM object to output a message to the edit control
      @param[in] sMsg message to output to the control
      @param[in] bPresentModal true to present a modal dialog with the message
   */
   void FireOutputMsg(const CString& sMsg, bool bPresentModal);

   /**
   Called from the COM object when the manual command has been written to the COM port
   @param[in] iErrCode error code for the operation
   */
   void FireManualCmdWriteFinished(int iErrCode);

   /*
   Opens the file for reading and initializes the buffer size
   */
   void PrepareToDownload(void);

   /*
   Called by the COM thread when the write operation is finished. Will post a message to the thread
   to invoke the event handler OnWriteNextFileBuffer.
   */
   void FireWriteNextFileBuffer(void);

   /*
   Called when the entire contents of the file have been written to the COM port or the operation has
   been aborted.
   */
   void FinishedWritingFile(void);

   static const int WM_ON_OUTPUT_MSG                  = WM_USER + 1;
   static const int WM_ON_MANUAL_CMD_WRITE_FINISHED   = WM_ON_OUTPUT_MSG + 1;
   static const int WM_ON_WRITE_NEXT_FILE_BUFFER      = WM_ON_MANUAL_CMD_WRITE_FINISHED + 1;
   static const int WM_ON_FILE_BUFFER_WRITE_FINISHED  = WM_ON_WRITE_NEXT_FILE_BUFFER + 1;

   std::shared_ptr<CComThread> m_spoComThread;

   CComboBox m_oCboPortNumber;
   CComboBox m_oCboBaudRate;
   CComboBox m_oCboDataBits;
   CComboBox m_oCboStopBits;
   CComboBox m_oCboParity;
   CComboBox m_oCboHandshaking;

   /*
   Control that contains the operator entered command to send to the COM port
   */
   CEdit m_oEditManualCmd;

   /**
   This is the output window
   */
   CRichEditCtrl m_oOutputWnd;

   /**
   This is the file stream to download to the Arduino via the COM port
   */
   std::ifstream m_oFileStream;

   /**
   The full path and file name
   */
   CString m_sPathName;
};
