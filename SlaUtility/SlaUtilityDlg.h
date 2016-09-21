
// SlaUtilityDlg.h : header file
//

#pragma once


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

   afx_msg void OnCbnSelchangeComboPortNumber();
   afx_msg void OnCbnSelchangeComboBaudRate();
   afx_msg void OnCbnSelchangeComboDataBits();
   afx_msg void OnCbnSelchangeComboStopBits();
   afx_msg void OnCbnSelchangeComboParity();
   afx_msg void OnCbnSelchangeComboHandshaking();

   DECLARE_MESSAGE_MAP()

private:

   /**
      Updates the opened COM ports settings

      @return true if successfully updated the COM settings
   */
   bool UpdateCommSettings(void);

   /**
      Validates all of the controls that are required for COM.

      @return true if valid
   */
   bool IsCommEntriesValid(void) const;

   /**
      @return baud rate selected in the control. A -1 indicates it is not selected.
   */
   int GetSelectedBaudRate(void);

   /**
      @return data bits selected in the control. A -1 indicates it is not selected.
   */
   int GetSelectedDataBits(void);

   /**
      @return stop bits selected in the control (ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS). A -1 indicates it is not selected.
   */
   int GetSelectedStopBits(void);

   /**
      @return parity selected in the control (NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY). A -1 indicates it is not selected.
   */
   int GetSelectedParity(void);

   /**
      @return handshaking selected in the control (0 = None, 1 = Xoff/Xon, 2 = RTS Handshake, 3 = RTS Handshake/Xoff/Xon). A -1 indicates it is not selected.
   */
   int GetSelectedHandshaking(void);

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
      Enables/disables all of the controls
   */
   void EnableAllControls(bool bEnable);

   CComboBox m_oCboPortNumber;
   CComboBox m_oCboBaudRate;
   CComboBox m_oCboDataBits;
   CComboBox m_oCboStopBits;
   CComboBox m_oCboParity;
   CComboBox m_oCboHandshaking;
   
   /**
      Handle to the COM port to the Arduino
   */
   HANDLE m_hComm;
};
