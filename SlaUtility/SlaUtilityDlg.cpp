
// SlaUtilityDlg.cpp : implementation file
//

#include "stdafx.h"
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

void CSlaUtilityDlg::InitComboBoxes(void)
{
}
