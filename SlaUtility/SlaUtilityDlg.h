
// SlaUtilityDlg.h : header file
//

#pragma once

#include <memory>
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

   afx_msg void OnCbnSelchangeComboPortNumber();
   afx_msg void OnCbnSelchangeComboBaudRate();
   afx_msg void OnCbnSelchangeComboDataBits();
   afx_msg void OnCbnSelchangeComboStopBits();
   afx_msg void OnCbnSelchangeComboParity();
   afx_msg void OnCbnSelchangeComboHandshaking();

   /**
      Event handler that is called when the COM is connected whether pass or fail
      @param[in] lParam will contain the result code for the connection
   */
   afx_msg LRESULT OnComConnected(WPARAM wParam, LPARAM lParam);

   /**
      Event handler that is called to output a message to the rich edit control
      @param[in] lParam will contain the raw pointer to a CString containing the message
   */
   afx_msg LRESULT OnOutputMsg(WPARAM wParam, LPARAM lParam);

   DECLARE_MESSAGE_MAP()

private:

   /**
      Validates all of the controls that are required for COM.
      @param[out] updated with the valid COM entries

      @return true if valid
   */
   bool IsCommEntriesValid(CComSettings& oComSettings) const;

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
      Enables/disables all of the controls
   */
   void EnableAllControls(bool bEnable);

   /**
      Outputs the message to the output window. Appends a new line to the message.
   */
   void OutputMessage(const CString& sMsg);

   /**
      Called from the COM thread when the connection operation is finished
   */
   void FireComConnected(int iErrCode);

   /**
      Called from the COM thread to output a message to the edit control
   */
   void FireOutputMsg(const CString& sMsg);

   static const int WM_ON_COM_CONNECTED   = WM_USER + 1;
   static const int WM_ON_OUTPUT_MSG      = WM_ON_COM_CONNECTED + 1;

   std::shared_ptr<CComThread> m_spoComThread;

   CComboBox m_oCboPortNumber;
   CComboBox m_oCboBaudRate;
   CComboBox m_oCboDataBits;
   CComboBox m_oCboStopBits;
   CComboBox m_oCboParity;
   CComboBox m_oCboHandshaking;

   /**
      This is the output window
   */
   CRichEditCtrl m_oOutputWnd;

   /**
      The full path and file name
   */
   CString m_sPathName;
};
