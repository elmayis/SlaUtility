
// SlaUtilityDlg.cpp : implementation file
//

#include "stdafx.h"
#include <functional>
#include <string>
#include "Com.h"
#include "SlaUtility.h"
#include "SlaUtilityDlg.h"
#include "StatusCodes.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
:  CDialogEx(IDD_SLAUTILITY_DIALOG, pParent)
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

   DDX_Control(pDX, IDC_RICHEDIT_OUTPUT, m_oOutputWnd);
}

BEGIN_MESSAGE_MAP(CSlaUtilityDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
   ON_BN_CLICKED(IDC_BUTTON_SERIAL_PORTS, &CSlaUtilityDlg::OnBnClickedButtonSerialPorts)
   ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CSlaUtilityDlg::OnBnClickedButtonConnect)
   ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CSlaUtilityDlg::OnBnClickedButtonDisconnect)
   ON_BN_CLICKED(IDC_BUTTON_LOAD_FILE, &CSlaUtilityDlg::OnBnClickedLoadFile)
   ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CSlaUtilityDlg::OnBnClickedDownload)

   ON_CBN_SELCHANGE(IDC_COMBO_PORT_NUMBER, &CSlaUtilityDlg::OnCbnSelchangeComboPortNumber)
   ON_CBN_SELCHANGE(IDC_COMBO_BAUD_RATE, &CSlaUtilityDlg::OnCbnSelchangeComboBaudRate)
   ON_CBN_SELCHANGE(IDC_COMBO_DATA_BITS, &CSlaUtilityDlg::OnCbnSelchangeComboDataBits)
   ON_CBN_SELCHANGE(IDC_COMBO_STOP_BITS, &CSlaUtilityDlg::OnCbnSelchangeComboStopBits)
   ON_CBN_SELCHANGE(IDC_COMBO_PARITY, &CSlaUtilityDlg::OnCbnSelchangeComboParity)
   ON_CBN_SELCHANGE(IDC_COMBO_HANDSHAKING, &CSlaUtilityDlg::OnCbnSelchangeComboHandshaking)

   ON_MESSAGE(WM_ON_OUTPUT_MSG, OnOutputMsg)

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
   EnableInitialControls();
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

void CSlaUtilityDlg::OnBnClickedButtonSerialPorts()
{
   EnableAllControls(false);
   m_oCboPortNumber.Clear();
   CString sCommPort;
   // We will only expect no more than 15 COM ports on a system
   //
   for (int iIndex = 1; iIndex <= 15; ++iIndex)
   {
      CString sNumber;
      sNumber.Format("COM%d", iIndex);
      sCommPort.Format("\\\\.\\%s", sNumber);
      HANDLE hCom =
         CreateFile(sCommPort,
            GENERIC_READ | GENERIC_WRITE,
            0,                //  must be opened with exclusive-access
            NULL,             //  default security attributes
            OPEN_EXISTING,    //  must use OPEN_EXISTING
            0,                //  not overlapped I/O
            NULL);            //  hTemplate must be NULL for comm devices
      if (INVALID_HANDLE_VALUE != hCom)
      {
         CloseHandle(hCom);
         m_oCboPortNumber.AddString(sNumber);
      }
      else
      {
         CString sErrMsg;
         const DWORD dwErrCode = GetLastError();
         sErrMsg.Format("CreateFile failed with error code %d for %s.", dwErrCode, sNumber);
         OutputMessage(sErrMsg);
      }
   }
   if (0 != m_oCboPortNumber.GetCount())
   {
      // Make a selection from the registry if possible
      //
      const CString sRegPortNumber = RetrievePortNumber();
      const int iItemPos = m_oCboPortNumber.FindString(-1, sRegPortNumber);
      if (-1 != iItemPos)
      {
         m_oCboPortNumber.SetCurSel(iItemPos);
      }
      else
      {
         m_oCboPortNumber.SetCurSel(0);
      }
      GetDlgItem(IDC_BUTTON_SERIAL_PORTS)->EnableWindow(true);
      m_oCboPortNumber.EnableWindow(true);
      GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(true);
      m_oCboPortNumber.EnableWindow(true);
      m_oCboBaudRate.EnableWindow(true);
      m_oCboDataBits.EnableWindow(true);
      m_oCboStopBits.EnableWindow(true);
      m_oCboParity.EnableWindow(true);
      m_oCboHandshaking.EnableWindow(true);
   }
}

void CSlaUtilityDlg::OnBnClickedButtonConnect()
{
   CComSettings oComSettings;
   if (IsCommEntriesValid(oComSettings))
   {
      m_spoCom.reset(new CCom(std::bind(&CSlaUtilityDlg::FireOutputMsg, this, std::placeholders::_1, std::placeholders::_2)));
      if (NULL != m_spoCom)
      {
         OpenCom(oComSettings);
      }
      else
      {
         OutputMessage("Failed to create the COM object.");
      }

   }
   else
   {
      OutputMessage("All COM setting entries are not valid.");
   }
}

void CSlaUtilityDlg::OnBnClickedButtonDisconnect()
{
   if (m_spoCom)
   {
      m_spoCom.reset();
   }
}

void CSlaUtilityDlg::OnBnClickedLoadFile()
{
   m_sPathName.Empty();
   CFileDialog oBrowse(
      true,
      ".ild",
      CString(),
      OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ENABLESIZING,
      "ILDA File (*.ild)|*.ild|");
   //oBrowse.m_pOFN->lpstrInitialDir = folder;
   oBrowse.m_pOFN->lpstrTitle = "Select an ILDA file to download";
   if (IDOK == oBrowse.DoModal())
   {
      m_sPathName = oBrowse.GetPathName();
      GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(true);
   }
}

void CSlaUtilityDlg::OnBnClickedDownload()
{

}

void CSlaUtilityDlg::OnCbnSelchangeComboPortNumber()
{
   CString sSelection;
   m_oCboPortNumber.GetLBText(m_oCboPortNumber.GetCurSel(), sSelection);
   HKEY hKey = GetAppSubkey();
   if (NULL == hKey) return;

   RegSetValueEx(hKey, "PortNumber", 0, REG_SZ, reinterpret_cast<BYTE*>(sSelection.GetBuffer()), sSelection.GetLength() + 1);
}

void CSlaUtilityDlg::OnCbnSelchangeComboBaudRate()
{
   int iValue = GetSelectedBaudRate();
   if (-1 == iValue) return;

   HKEY hKey = GetAppSubkey();
   if (NULL == hKey) return;

   RegSetValueEx(hKey, "BaudRate", 0, REG_DWORD, reinterpret_cast<BYTE*>(&iValue), sizeof(iValue));
}

void CSlaUtilityDlg::OnCbnSelchangeComboDataBits()
{
   int iValue = GetSelectedDataBits();
   if (-1 == iValue) return;

   HKEY hKey = GetAppSubkey();
   if (NULL == hKey) return;

   RegSetValueEx(hKey, "DataBits", 0, REG_DWORD, reinterpret_cast<BYTE*>(&iValue), sizeof(iValue));
}

void CSlaUtilityDlg::OnCbnSelchangeComboStopBits()
{
   int iValue = GetSelectedStopBits();
   if (-1 == iValue) return;

   HKEY hKey = GetAppSubkey();
   if (NULL == hKey) return;

   RegSetValueEx(hKey, "StopBits", 0, REG_DWORD, reinterpret_cast<BYTE*>(&iValue), sizeof(iValue));
}

void CSlaUtilityDlg::OnCbnSelchangeComboParity()
{
   int iValue = GetSelectedParity();
   if (-1 == iValue) return;

   HKEY hKey = GetAppSubkey();
   if (NULL == hKey) return;

   RegSetValueEx(hKey, "Parity", 0, REG_DWORD, reinterpret_cast<BYTE*>(&iValue), sizeof(iValue));
}

void CSlaUtilityDlg::OnCbnSelchangeComboHandshaking()
{
   int iValue = GetSelectedHandshaking();
   if (-1 == iValue) return;

   HKEY hKey = GetAppSubkey();
   if (NULL == hKey) return;

   RegSetValueEx(hKey, "Handshaking", 0, REG_DWORD, reinterpret_cast<BYTE*>(&iValue), sizeof(iValue));
}

void CSlaUtilityDlg::OpenCom(const CComSettings& oComSettings)
{
   const CStatusCodes::ECodes eErrCode = m_spoCom->Connect(oComSettings);
   if (CStatusCodes::SC_OK == eErrCode)
   {
      OutputMessage("Established connection to the COM port.");
      // Enable/disable appropriate controls
      //
      GetDlgItem(IDC_BUTTON_SERIAL_PORTS)->EnableWindow(false);
      GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(false);
      GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(true);
      GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(true);
      GetDlgItem(IDC_BUTTON_LOAD_FILE)->EnableWindow(true);
      GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(true);
      GetDlgItem(IDC_EDIT_MANUAL_COMMAND)->EnableWindow(true);
      m_oCboPortNumber.EnableWindow(false);
      m_oCboBaudRate.EnableWindow(false);
      m_oCboDataBits.EnableWindow(false);
      m_oCboStopBits.EnableWindow(false);
      m_oCboParity.EnableWindow(false);
      m_oCboHandshaking.EnableWindow(false);
   }
   else
   {
      CString sMsg;
      sMsg.Format("COM connection failed with status code %d.", eErrCode);
      OutputMessage(sMsg);
   }
}

LRESULT CSlaUtilityDlg::OnOutputMsg(WPARAM wParam, LPARAM lParam)
{
   std::shared_ptr<CString> spoMsg(reinterpret_cast<CString*>(lParam));
   OutputMessage(*spoMsg, (0 != wParam));
   return 0;
}

bool CSlaUtilityDlg::IsCommEntriesValid(CComSettings& oComSettings) const
{
   oComSettings = CComSettings();
   CComSettings oNewSettings;
   oNewSettings.m_iPortNumber = GetSelectedPortNumber();
   if (-1 == oNewSettings.m_iPortNumber) return false;

   oNewSettings.m_iBaudRate = GetSelectedBaudRate();
   if (-1 == oNewSettings.m_iBaudRate) return false;

   oNewSettings.m_iDataBits = GetSelectedDataBits();
   if (-1 == oNewSettings.m_iDataBits) return false;

   oNewSettings.m_iStopBits = GetSelectedStopBits();
   if (-1 == oNewSettings.m_iStopBits) return false;

   oNewSettings.m_iParity = GetSelectedParity();
   if (-1 == oNewSettings.m_iParity) return false;

   oNewSettings.m_iHandshaking = GetSelectedHandshaking();
   if (-1 == oNewSettings.m_iHandshaking) return false;

   oComSettings = oNewSettings;
   return true;
}

int CSlaUtilityDlg::GetSelectedPortNumber(void) const
{
   CString sSelection;
   int iValue = -1;
   m_oCboPortNumber.GetLBText(m_oCboPortNumber.GetCurSel(), sSelection);
   if (!sSelection.IsEmpty())
   {
      // The port number entry is prepended with the text "COM" followed by the port number.
      // Extract only the port number.
      //
      const std::string sValue = sSelection.Mid(3);
      iValue = std::stol(sValue);
   }
   return iValue;
}

int CSlaUtilityDlg::GetSelectedBaudRate(void) const
{
   CString sSelection;
   int iValue = -1;
   m_oCboBaudRate.GetLBText(m_oCboBaudRate.GetCurSel(), sSelection);
   if(!sSelection.IsEmpty())
   {
      const std::string sValue = sSelection;
      iValue = std::stol(sValue);
   }
   return iValue;
}

int CSlaUtilityDlg::GetSelectedDataBits(void) const
{
   CString sSelection;
   int iValue = -1;
   m_oCboDataBits.GetLBText(m_oCboDataBits.GetCurSel(), sSelection);
   if (!sSelection.IsEmpty())
   {
      const std::string sValue = sSelection;
      iValue = std::stol(sValue);
   }
   return iValue;
}

int CSlaUtilityDlg::GetSelectedStopBits(void) const
{
   CString sSelection;
   int iValue = -1;
   m_oCboStopBits.GetLBText(m_oCboStopBits.GetCurSel(), sSelection);
   if (!sSelection.IsEmpty())
   {
      if ("One" == sSelection)
      {
         iValue = ONESTOPBIT;
      }
      else if ("OnePointFive" == sSelection)
      {
         iValue = ONE5STOPBITS;
      }
      else if ("Two" == sSelection)
      {
         iValue = TWOSTOPBITS;
      }
   }
   return iValue;
}

int CSlaUtilityDlg::GetSelectedParity(void) const
{
   CString sSelection;
   int iValue = -1;
   m_oCboParity.GetLBText(m_oCboParity.GetCurSel(), sSelection);
   if (!sSelection.IsEmpty())
   {
      if ("None" == sSelection)
      {
         iValue = NOPARITY;
      }
      else if ("Even" == sSelection)
      {
         iValue = EVENPARITY;
      }
      else if ("Mark" == sSelection)
      {
         iValue = MARKPARITY;
      }
      else if ("Odd" == sSelection)
      {
         iValue = ODDPARITY;
      }
      else if ("Space" == sSelection)
      {
         iValue = SPACEPARITY;
      }
   }
   return iValue;
}

int CSlaUtilityDlg::GetSelectedHandshaking(void) const
{
   CString sSelection;
   int iValue = -1;
   m_oCboHandshaking.GetLBText(m_oCboHandshaking.GetCurSel(), sSelection);
   if (!sSelection.IsEmpty())
   {
      if ("None" == sSelection)
      {
         iValue = 0;
      }
      else if ("XOnXOff" == sSelection)
      {
         iValue = 1;
      }
      else if ("RequestToSend" == sSelection)
      {
         iValue = 2;
      }
      else if ("RequestToSendXOnXOff" == sSelection)
      {
         iValue = 3;
      }
   }
   return iValue;
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

   InitBaudRateFromRegistry(hKey);
   InitDataBitsFromRegistry(hKey);
   InitStopBitsFromRegistry(hKey);
   InitParityFromRegistry(hKey);
   InitHandshakingFromRegistry(hKey);
}

void CSlaUtilityDlg::InitBaudRateFromRegistry(HKEY hKey)
{
   DWORD dwDwordSize = sizeof(DWORD);
   DWORD dwValue = 250000;
   // lResult used just for debugging at this point
   //
   LONG lResult = RegGetValue(hKey, CString(), CString("BaudRate"), RRF_RT_REG_DWORD, NULL, &dwValue, &dwDwordSize);
   CString sRegValue(std::to_string(dwValue).c_str());
   int iItemPos = m_oCboBaudRate.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboBaudRate.SetCurSel(iItemPos);
   }
}

void CSlaUtilityDlg::InitDataBitsFromRegistry(HKEY hKey)
{
   DWORD dwDwordSize = sizeof(DWORD);
   DWORD dwValue = 8;
   // lResult used just for debugging at this point
   //
   LONG lResult = RegGetValue(hKey, CString(), CString("DataBits"), RRF_RT_REG_DWORD, NULL, &dwValue, &dwDwordSize);
   CString sRegValue = CString(std::to_string(dwValue).c_str());
   int iItemPos = m_oCboDataBits.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboDataBits.SetCurSel(iItemPos);
   }
}

void CSlaUtilityDlg::InitStopBitsFromRegistry(HKEY hKey)
{
   DWORD dwDwordSize = sizeof(DWORD);
   DWORD dwValue = ONESTOPBIT;
   CString sRegValue = "One";
   // lResult used just for debugging at this point
   //
   LONG lResult = RegGetValue(hKey, CString(), CString("StopBits"), RRF_RT_REG_DWORD, NULL, &dwValue, &dwDwordSize);
   switch (dwValue)
   {
   case ONESTOPBIT:
      {
         sRegValue = "One";
         break;
      }
   case ONE5STOPBITS:
      {
         sRegValue = "OnePointFive";
         break;
      }
   case TWOSTOPBITS:
      {
         sRegValue = "Two";
         break;
      }
   }
   int iItemPos = m_oCboStopBits.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboStopBits.SetCurSel(iItemPos);
   }
}

void CSlaUtilityDlg::InitParityFromRegistry(HKEY hKey)
{
   DWORD dwDwordSize = sizeof(DWORD);
   DWORD dwValue = NOPARITY;
   CString sRegValue = "None";
   // lResult used just for debugging at this point
   //
   LONG lResult = RegGetValue(hKey, CString(), CString("Parity"), RRF_RT_REG_DWORD, NULL, &dwValue, &dwDwordSize);
   switch (dwValue)
   {
      case NOPARITY:
      {
         sRegValue = "None";
         break;
      }
   case ODDPARITY:
      {
         sRegValue = "Odd";
         break;
      }
   case EVENPARITY:
      {
         sRegValue = "Even";
         break;
      }
   case MARKPARITY:
      {
         sRegValue = "Mark";
         break;
      }
   case SPACEPARITY:
      {
         sRegValue = "Space";
         break;
      }
   }
   int iItemPos = m_oCboParity.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboParity.SetCurSel(iItemPos);
   }
}

void CSlaUtilityDlg::InitHandshakingFromRegistry(HKEY hKey)
{
   DWORD dwDwordSize = sizeof(DWORD);
   DWORD dwValue = NOPARITY;
   CString sRegValue = "None";
   // lResult used just for debugging at this point
   //
   LONG lResult = RegGetValue(hKey, CString(), CString("Handshaking"), RRF_RT_REG_DWORD, NULL, &dwValue, &dwDwordSize);
   switch (dwValue)
   {
   case 0:
   {
      sRegValue = "None";
      break;
   }
   case 1:
   {
      sRegValue = "XOnXOff";
      break;
   }
   case 2:
   {
      sRegValue = "RequestToSend";
      break;
   }
   case 3:
   {
      sRegValue = "RequestToSendXOnXOff";
      break;
   }
   }
   int iItemPos = m_oCboHandshaking.FindString(-1, sRegValue);
   if (-1 != iItemPos)
   {
      m_oCboHandshaking.SetCurSel(iItemPos);
   }
}

CString CSlaUtilityDlg::RetrievePortNumber(void)
{
   HKEY hKey = GetAppSubkey();
   if (NULL == hKey) return CString();

   DWORD dwStringSize = 30;
   CString sRegValue;
   LONG lResult = RegGetValue(hKey, CString(), CString("PortNumber"), RRF_RT_REG_SZ, NULL, sRegValue.GetBuffer(dwStringSize), &dwStringSize);
   sRegValue.ReleaseBuffer();
   return sRegValue;
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
   LONG lResult = RegOpenKeyEx(HKEY_CURRENT_USER, sSubkey, 0, KEY_ALL_ACCESS, &hKey);
   if(lResult == ERROR_SUCCESS) return hKey;

   RegCreateKeyEx(HKEY_CURRENT_USER, sSubkey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
   return hKey;
}

void CSlaUtilityDlg::EnableInitialControls(void)
{
   EnableAllControls(false);
   GetDlgItem(IDC_BUTTON_SERIAL_PORTS)->EnableWindow(true);
   m_oCboPortNumber.EnableWindow(true);
}

void CSlaUtilityDlg::EnableAllControls(bool bEnable)
{
   GetDlgItem(IDC_BUTTON_SERIAL_PORTS)->EnableWindow(bEnable);
   GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(bEnable);
   GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(bEnable);
   GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(bEnable);
   GetDlgItem(IDC_BUTTON_LOAD_FILE)->EnableWindow(bEnable);
   GetDlgItem(IDC_BUTTON_DOWNLOAD)->EnableWindow(bEnable);
   GetDlgItem(IDC_EDIT_MANUAL_COMMAND)->EnableWindow(bEnable);
   m_oCboPortNumber.EnableWindow(bEnable);
   m_oCboBaudRate.EnableWindow(bEnable);
   m_oCboDataBits.EnableWindow(bEnable);
   m_oCboStopBits.EnableWindow(bEnable);
   m_oCboParity.EnableWindow(bEnable);
   m_oCboHandshaking.EnableWindow(bEnable);
}

void CSlaUtilityDlg::OutputMessage(const CString& sMsg, bool bPresentModal)
{
   const CString sNewMessage = sMsg + "\n";
   m_oOutputWnd.SetSel(-1, -1);
   m_oOutputWnd.ReplaceSel(sNewMessage);
   if (bPresentModal)
   {
      AfxMessageBox(sMsg);
   }
}

void CSlaUtilityDlg::FireOutputMsg(const CString& sMsg, bool bPresentModal)
{
   // Allocate a copy of the delegate on the heap. This needs to be done so that the raw pointer can be
   // passed to this thread across thread bounderies. The OS layer is only capable of dealing with simple POD.
   // The pointer will be free'd by the OnConnect message handler.
   //
   CString* poMsg = new CString(sMsg);
   PostMessage(WM_ON_OUTPUT_MSG, bPresentModal, reinterpret_cast<LPARAM>(poMsg));
}