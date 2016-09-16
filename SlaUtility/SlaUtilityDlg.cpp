
// SlaUtilityDlg.cpp : implementation file
//

#include "stdafx.h"
#include <string>
#include "SlaUtility.h"
#include "SlaUtilityDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


CSlaUtilityDlg::CSlaUtilityDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SLAUTILITY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSlaUtilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

   DDX_Control(pDX, IDC_COMBO_PORT_NUMBER, m_oCboPortNumber);
   DDX_Control(pDX, IDC_COMBO_BAUD_RATE, m_oCboBaudRate);
   DDX_Control(pDX, IDC_COMBO_DATA_BITS, m_oCboDataBits);
   DDX_Control(pDX, IDC_COMBO_STOP_BITS, m_oCboStopBits);
   DDX_Control(pDX, IDC_COMBO_PARITY, m_oCboParity);
   DDX_Control(pDX, IDC_COMBO_HANDSHAKING, m_oCboHandshaking);
}

BEGIN_MESSAGE_MAP(CSlaUtilityDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
   ON_CBN_SELCHANGE(IDC_COMBO_BAUD_RATE, &CSlaUtilityDlg::OnCbnSelchangeComboBaudRate)
END_MESSAGE_MAP()


// CSlaUtilityDlg message handlers

BOOL CSlaUtilityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

   AddItemsToComboBoxes();
   InitControlsFromRegistry();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSlaUtilityDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSlaUtilityDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSlaUtilityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSlaUtilityDlg::OnCbnSelchangeComboBaudRate()
{
   CString sSelection;
   m_oCboBaudRate.GetLBText(m_oCboBaudRate.GetCurSel(), sSelection);
   const std::string sValue = sSelection;
   DWORD dwValue = std::stol(sValue);
}

void CSlaUtilityDlg::AddItemsToComboBoxes(void)
{
   // Baud rate control
   //
   m_oCboBaudRate.AddString(CString("300"));
   m_oCboBaudRate.AddString(CString("600"));
   m_oCboBaudRate.AddString(CString("1200"));
   m_oCboBaudRate.AddString(CString("2400"));
   m_oCboBaudRate.AddString(CString("9600"));
   m_oCboBaudRate.AddString(CString("14400"));
   m_oCboBaudRate.AddString(CString("19200"));
   m_oCboBaudRate.AddString(CString("38400"));
   m_oCboBaudRate.AddString(CString("57600"));
   m_oCboBaudRate.AddString(CString("115200"));
   m_oCboBaudRate.AddString(CString("250000"));
   m_oCboBaudRate.SetCurSel(m_oCboBaudRate.GetCount() - 1);

   // Data bits control
   //
   m_oCboDataBits.AddString(CString("7"));
   m_oCboDataBits.AddString(CString("8"));
   m_oCboDataBits.SetCurSel(1);

   // Stop bits control
   //
   m_oCboStopBits.AddString(CString("One"));
   m_oCboStopBits.AddString(CString("OnePointFive"));
   m_oCboStopBits.AddString(CString("Two"));
   m_oCboStopBits.SetCurSel(0);

   // Parity control
   //
   m_oCboParity.AddString(CString("None"));
   m_oCboParity.AddString(CString("Even"));
   m_oCboParity.AddString(CString("Mark"));
   m_oCboParity.AddString(CString("Odd"));
   m_oCboParity.AddString(CString("Space"));
   m_oCboParity.SetCurSel(0);

   //Handshake control
   //
   m_oCboHandshaking.AddString(CString("None"));
   m_oCboHandshaking.AddString(CString("XOnXOff"));
   m_oCboHandshaking.AddString(CString("RequestToSend"));
   m_oCboHandshaking.AddString(CString("RequestToSendXOnXOff"));
   m_oCboHandshaking.SetCurSel(0);
}

void CSlaUtilityDlg::InitControlsFromRegistry(void)
{
   HKEY hKey = GetAppSubkey();
   if (NULL == hKey) return;

   int iItemPos = 0;
   DWORD dwDwordSize = sizeof(DWORD);
   // Retrieve the baud rate from the registry
   //
   DWORD dwValue = 250000;
   RegGetValue(hKey, CString(), CString("BaudRate"), RRF_RT_REG_DWORD, NULL, &dwValue, &dwDwordSize);
   CString sRegValue(std::to_string(dwValue).c_str());
   iItemPos = m_oCboBaudRate.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboBaudRate.SetCurSel(iItemPos);
   }
   // Retrieve the data bits from the registy
   //
   dwValue = 8;
   RegGetValue(hKey, CString(), CString("DataBits"), RRF_RT_REG_DWORD, NULL, &dwValue, &dwDwordSize);
   sRegValue = CString(std::to_string(dwValue).c_str());
   iItemPos = m_oCboDataBits.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboDataBits.SetCurSel(iItemPos);
   }
   DWORD dwStringSize = 30;
   // Retrieve the stop bits from the registy
   //
   sRegValue = CString("One");
   RegGetValue(hKey, CString(), CString("StopBits"), RRF_RT_REG_SZ, NULL, sRegValue.GetBuffer(dwStringSize), &dwStringSize);
   sRegValue.ReleaseBuffer();
   iItemPos = m_oCboStopBits.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboStopBits.SetCurSel(iItemPos);
   }
   // Retrieve the parity from the registy
   //
   sRegValue = CString("None");
   RegGetValue(hKey, CString(), CString("Parity"), RRF_RT_REG_SZ, NULL, sRegValue.GetBuffer(dwStringSize), &dwStringSize);
   sRegValue.ReleaseBuffer();
   iItemPos = m_oCboParity.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboParity.SetCurSel(iItemPos);
   }
   // Retrieve the handshaking from the registy
   //
   sRegValue = CString("None");
   RegGetValue(hKey, CString(), CString("Handshaking"), RRF_RT_REG_SZ, NULL, sRegValue.GetBuffer(dwStringSize), &dwStringSize);
   sRegValue.ReleaseBuffer();
   iItemPos = m_oCboHandshaking.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboHandshaking.SetCurSel(iItemPos);
   }
}

HKEY CSlaUtilityDlg::GetAppSubkey(void)
{
   CString sAppFullPath;
   ::GetModuleFileName(NULL, sAppFullPath.GetBuffer(MAX_PATH), MAX_PATH);
   sAppFullPath.ReleaseBuffer();

   CString sAppName;
   _tsplitpath_s(sAppFullPath, NULL, 0, NULL, 0, sAppName.GetBuffer(_MAX_FNAME), _MAX_FNAME, NULL, 0);
   sAppName.ReleaseBuffer();

   const CString sSubkey = CString("SOFTWARE\\") + sAppName;

   HKEY hKey = NULL;
   LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, sSubkey, 0, KEY_WRITE, &hKey);
   if(lResult == ERROR_SUCCESS) return hKey;

   RegCreateKeyEx(HKEY_CURRENT_USER, sSubkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
   return hKey;
}
