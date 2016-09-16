
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

   afx_msg void OnCbnSelchangeComboBaudRate();
   afx_msg void OnCbnSelchangeComboDataBits();
   afx_msg void OnCbnSelchangeComboStopBits();
   afx_msg void OnCbnSelchangeComboParity();
   afx_msg void OnCbnSelchangeComboHandshaking();

   DECLARE_MESSAGE_MAP()

private:
   /**
      Adds content to each combobox control
   */
   void AddItemsToComboBoxes(void);

   /**
   */
   void InitControlsFromRegistry(void);

   /**
      Creates the subkey if it does not already exist and then opens the key for read/write access
   */
   HKEY GetAppSubkey(void);

   CComboBox m_oCboPortNumber;
   CComboBox m_oCboBaudRate;
   CComboBox m_oCboDataBits;
   CComboBox m_oCboStopBits;
   CComboBox m_oCboParity;
   CComboBox m_oCboHandshaking;
};
