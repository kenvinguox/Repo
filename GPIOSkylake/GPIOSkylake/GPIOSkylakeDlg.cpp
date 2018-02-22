
// GPIOSkylakeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GPIOSkylake.h"
#include "GPIOSkylakeDlg.h"
#include "afxdialogex.h"
#include "8502EC.h"
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PCH_GPIO_COM0 0x00af0000 // PID_GPIOCOM0 = 0xAF
#define PCH_GPIO_COM1 0x00ae0000 // PID_GPIOCOM1 = 0xAE
#define PCH_GPIO_COM2 0x00ad0000 // PID_GPIOCOM2 = 0xAD
#define PCH_GPIO_COM3 0x00ac0000 // PID_GPIOCOM3 = 0xAC
#define PCH_PCR_BASE_ADDRESS 0xFD000000



//R_PCH_PCR_GPIO_GPP_A_PADCFG_OFFSET,    // SPT-LP GPP_A  0x400
//R_PCH_PCR_GPIO_GPP_B_PADCFG_OFFSET,    // SPT-LP GPP_B  0x4C0 
//R_PCH_PCR_GPIO_GPP_C_PADCFG_OFFSET,    // SPT-LP GPP_C  0x400 
//R_PCH_PCR_GPIO_GPP_D_PADCFG_OFFSET,    // SPT-LP GPP_D  0x4C0
//R_PCH_PCR_GPIO_GPP_E_PADCFG_OFFSET,    // SPT-LP GPP_E  0x580
//R_PCH_LP_PCR_GPIO_GPP_F_PADCFG_OFFSET, // SPT-LP GPP_F  0x400 
//R_PCH_LP_PCR_GPIO_GPP_G_PADCFG_OFFSET, // SPT-LP GPP_G  0x4C0
//
//PadCfgReg = 0x8 * PadNumber + GpioGroupInfo[GroupIndex].PadCfgOffset;
//
//PCH_PCR_BASE_ADDRESS | GpioGroupInfo[GroupIndex].Community << 16 | PadCfgReg
//
//PCH_PCR_BASE_ADDRESS = 0x FD000000

// CAboutDlg dialog used for App About


#define ptitle           _T("********************************************************\r\n")
#define pProgramName     _T("*ProgramName: GPIOSkylake.exe                        \r\n")      
#define pProgramVersion  _T("*ProgramVersion: Ver:1.0.0.1                   \r\n")
#define pCreateDate      _T("*CreateDate: 2015-8-12                         \r\n")
#define pAuthor          _T("*Author: STE-SW Copyright (C) by xiangxiang    \r\n")
#define pMoreInfo        _T("*MoreInfo: Developed By STE                    \r\n")
#define psplit           _T("--------------------------------------------------------\r\n")

struct _CONFIG
{
	int mAutoRun;
	int mAutoPassExit;
	int mAutoFailExit;
	int mCycleTime;
	TCHAR sValue[16];
};
struct _CONFIG Con;


struct _GPIO
{
	int mIFREAD;
	TCHAR sGroup[16];
	int mSubGroupNum;
	int mCommunityNum;
	DWORD dPAD_CFG_OFFSET;
};
struct _GPIO GPIOGroup[4];

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGPIOSkylakeDlg dialog



CGPIOSkylakeDlg::CGPIOSkylakeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGPIOSkylakeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(DRegiterValue, 0, sizeof(DRegiterValue));
	bResult = FALSE;
}

void CGPIOSkylakeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Group1, m_Group[0]);
	DDX_Control(pDX, IDC_COMBO_Group2, m_Group[1]);
	DDX_Control(pDX, IDC_COMBO_Group3, m_Group[2]);
	DDX_Control(pDX, IDC_COMBO_Group4, m_Group[3]);
	DDX_Control(pDX, IDC_COMBO_SubGroup1, m_SubGroup[0]);
	DDX_Control(pDX, IDC_COMBO_SubGroup2, m_SubGroup[1]);
	DDX_Control(pDX, IDC_COMBO_SubGroup3, m_SubGroup[2]);
	DDX_Control(pDX, IDC_COMBO_SubGroup4, m_SubGroup[3]);
	DDX_Control(pDX, IDC_COMBO_Community1, m_Community[0]);
	DDX_Control(pDX, IDC_COMBO_Community2, m_Community[1]);
	DDX_Control(pDX, IDC_COMBO_Community3, m_Community[2]);
	DDX_Control(pDX, IDC_COMBO_Community4, m_Community[3]);
	DDX_Control(pDX, IDC_EDIT_OFFSET1, m_OFFSET[0]);
	DDX_Control(pDX, IDC_EDIT_OFFSET2, m_OFFSET[1]);
	DDX_Control(pDX, IDC_EDIT_OFFSET3, m_OFFSET[2]);
	DDX_Control(pDX, IDC_EDIT_OFFSET4, m_OFFSET[3]);
	DDX_Control(pDX, IDC_EDIT_Result, m_result);

}

BEGIN_MESSAGE_MAP(CGPIOSkylakeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_Read, &CGPIOSkylakeDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CGPIOSkylakeDlg::OnBnClickedButtonSet)
END_MESSAGE_MAP()


// CGPIOSkylakeDlg message handlers

BOOL CGPIOSkylakeDlg::OnInitDialog()
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

	 //TODO: Add extra initialization here
	CString strtitle;
	GetModuleFileName(AfxGetInstanceHandle(), strtitle.GetBuffer(1024), 1024);
	strtitle.ReleaseBuffer();
	int iPos = strtitle.ReverseFind('\\');
	strtitle = strtitle.Mid(iPos + 1);
	strtitle = strtitle.Left(strtitle.Find('.'));
	//设置窗口标题
	SetWindowText(strtitle);
	brush.CreateSolidBrush(RGB(160, 16, 21));
	m_brush = (HBRUSH)brush;
	HFONT hFont;
	hFont = CreateFont(35, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
	SendDlgItemMessage(IDC_EDIT_Result, WM_SETFONT, (int)hFont, TRUE);
	if (!ReadINI())
	{

		strText += "Not find ini!!";
		theApp.m_iRetCode = 255;
		SendMessage(WM_CLOSE);
		return FALSE;
	}

	if (!InitializeWinIO())
	{
		strText += "Error InitializeWinIO()!!";
		theApp.m_iRetCode = 254;
		SendMessage(WM_CLOSE);
		return FALSE;
	}

	 //TODO: Add extra initialization here

	if (Con.mAutoRun)
	{
		::PostMessage(m_hWnd, WM_COMMAND, MAKEWPARAM(IDC_BUTTON_Read, BN_CLICKED), 0);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGPIOSkylakeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGPIOSkylakeDlg::OnPaint()
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
HCURSOR CGPIOSkylakeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CGPIOSkylakeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CGPIOSkylakeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CString strtext;
	m_result.GetWindowText(strtext);
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_Result)
	{
		if (strtext.CompareNoCase("PASS")==0)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			hbr = CreateSolidBrush(RGB(0, 255, 0));
		}
		else if (strtext.CompareNoCase("FAIL") == 0)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			hbr = CreateSolidBrush(RGB(255, 0, 0));
		}
		else if (strtext.CompareNoCase("Test") == 0)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
			hbr = CreateSolidBrush(RGB(255, 255, 0));
		}

	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

CString CGPIOSkylakeDlg::NoteList()
{
	CString strNoteList;
	strNoteList += ptitle;
	strNoteList += pProgramName;
	strNoteList += pProgramVersion;
	strNoteList += pCreateDate;
	strNoteList += pAuthor;
	strNoteList += pMoreInfo;
	strNoteList += ptitle;
	strNoteList += "Release Notes:  \r\n";
	strNoteList += "  1.Ver1.0.0.0 build 2015/8/10  support skylake platfrom(PCH-LP).\r\n";
	strNoteList += "  2.Ver1.0.0.1 build 2015/8/12  support skylake platfrom(PCH-LP/PCH-H).\r\n";
	strNoteList += "  2.Ver1.0.0.2 build 2016/12/14 add return code. if pass,return 0,otherwise,return non-zero.\r\n";
	strNoteList += psplit;

	return strNoteList;
}
void CGPIOSkylakeDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	ShutdownWinIO();
	CString StrHelp;
	if (bResult)
	{
		sResultText = "PASS";
	}
	else
	{
		sResultText = "FAIL";
	}
	sResultText += "\r\n";
	StrHelp = NoteList();
	sResultText += StrHelp;

	sResultText += strText;
	sResultText += startTime;
	sResultText += EndTime;
	
	WriteINI();
	CDialogEx::OnClose();
}
//
BOOL CGPIOSkylakeDlg::ReadINI()
{
	memset(&Con, '\0', sizeof(Con));
	memset(&GPIOGroup, 0, sizeof(GPIOGroup));
	CString strkey, strGroup, strSubGroup, strgroupnum;
	GetModuleFileName(AfxGetInstanceHandle(), Logname.GetBuffer(1024), 1024);
	Logname.ReleaseBuffer();
	cIniName = Logname.Mid(Logname.ReverseFind('\\') + 1);
	cIniName = _T(".\\..\\INI\\") + cIniName.Left(cIniName.Find('.'));
	cIniName += _T(".ini");
	Logname = Logname.Left(Logname.GetLength() - 4) + _T("Log.txt");


	if (PathFileExists(Logname))
	{
		DeleteFile(Logname);      ///  删除原有的Log
	}

	if (!PathFileExists(cIniName))
	{
		return FALSE;
	}
	CString keyname = "Setting";
	Con.mAutoRun      = GetPrivateProfileInt(keyname, "IFAUTORUN", 1, cIniName);
	Con.mAutoPassExit = GetPrivateProfileInt(keyname, "IFPASSEXIT", 1, cIniName);
	Con.mAutoFailExit = GetPrivateProfileInt(keyname, "IFFAILEXIT", 0, cIniName);
	::GetPrivateProfileString(keyname, "Value", "0", Con.sValue, 16, cIniName);
	for (int i = 0; i < 4; i++)
	{
		strkey.Format("GPIO_%d", i);
		GPIOGroup[i].mIFREAD = GetPrivateProfileInt(strkey, "IFREAD", 1, cIniName);
		GPIOGroup[i].mSubGroupNum = GetPrivateProfileInt(strkey, "SubGroupNum", 0, cIniName);
		GPIOGroup[i].mCommunityNum = GetPrivateProfileInt(strkey, "CommunityNum", 0, cIniName);
		GPIOGroup[i].dPAD_CFG_OFFSET = (DWORD)GetPrivateProfileInt(strkey, "PAD_CFG_OFFSET", 0, cIniName);
		GetPrivateProfileString(strkey, "Group", "GPP_A0", GPIOGroup[i].sGroup, sizeof(GPIOGroup[i].sGroup), cIniName);

	}

	//Group1
	for (int m = 10; m < 17;m++)
	{
		if (16 == m)
		{
			strGroup="GPP_G";
		}
		else
		{
			strGroup.Format("GPP_%X", m);
		}
		//strGroup.Format("GPP_%X", m);
		m_Group[0].AddString(strGroup);
		m_Group[1].AddString(strGroup);
		m_Group[2].AddString(strGroup);
		m_Group[3].AddString(strGroup);
	}
	//add subgroup
	for (int n= 0; n < 24; n++)
	{
	    strGroup.Format("%d", n);
		m_SubGroup[0].AddString(strGroup);
		m_SubGroup[1].AddString(strGroup);
		m_SubGroup[2].AddString(strGroup);
		m_SubGroup[3].AddString(strGroup);
	}
	//add Community
	for (int l = 0; l < 4; l++)
	{
		strGroup.Format("%d", l);
		m_Community[0].AddString(strGroup);
		m_Community[1].AddString(strGroup);
		m_Community[2].AddString(strGroup);
		m_Community[3].AddString(strGroup);
	}
	//MFC中动态设置组合框的下拉菜单高度
	CRect rc_com,rc_group,rc_subgroup;        
	for (int i = 0; i < 4; i++)
	{
		m_Community[i].GetDroppedControlRect(&rc_com);
		m_Community[i].GetParent()->ScreenToClient(&rc_com);
		rc_com.bottom += 150;
		m_Community[i].MoveWindow(&rc_com);

		m_Group[i].GetDroppedControlRect(&rc_group);
		m_Group[i].GetParent()->ScreenToClient(&rc_group);
		rc_group.bottom += 150;
		m_Group[i].MoveWindow(&rc_group);

		m_SubGroup[i].GetDroppedControlRect(&rc_subgroup);
		m_SubGroup[i].GetParent()->ScreenToClient(&rc_subgroup);
		rc_subgroup.bottom += 150;
		m_SubGroup[i].MoveWindow(&rc_subgroup);
	}
	// 设置控件显示值
	for (int j = 0; j < 4; j++)
	{
		m_Group[j].SetWindowText(GPIOGroup[j].sGroup);
		strgroupnum.Format("%d", GPIOGroup[j].mSubGroupNum);
		m_SubGroup[j].SetWindowText(strgroupnum);
		strLine.Format("%d", GPIOGroup[j].mCommunityNum);
		m_Community[j].SetWindowText(strLine);
		strLine.Format("0x%X", GPIOGroup[j].dPAD_CFG_OFFSET);
		m_OFFSET[j].SetWindowText(strLine);
	}

	return TRUE;
}

void CGPIOSkylakeDlg::WriteINI()
{
	CFile Logfile;
	if (_access(Logname, 0) == -1)
	{
		Logfile.Open(Logname, CFile::modeCreate | CFile::modeWrite, NULL);
	}
	else
	{
		DeleteFile(Logname);
		Sleep(20);
		Logfile.Open(Logname, CFile::modeCreate | CFile::modeReadWrite);
	}
	Logfile.Write(sResultText.GetBuffer(sResultText.GetLength()), sResultText.GetLength());
	Logfile.Close();
}


DWORD CGPIOSkylakeDlg::ReadGPIORegister(CString Group, int SubGroupNum, int CommunityNum, DWORD dPAD_CFG_OFFSET)
{
	DWORD DReg = 0, DPadCfgReg, DPADCFG_OFFSET_Start, DOFFSET_COM, DGPIO_MEM_BASE;
	switch (CommunityNum)
	{
	case 0:
		DOFFSET_COM = PCH_GPIO_COM0;
		break;
	case 1:
		DOFFSET_COM = PCH_GPIO_COM1;
		break;
	case 2:
		DOFFSET_COM = PCH_GPIO_COM2;
		break;
	case 3:
		DOFFSET_COM = PCH_GPIO_COM3;
		break;
	default:
		break;
	}
	DPADCFG_OFFSET_Start = dPAD_CFG_OFFSET;
	DPadCfgReg = 0x8 * SubGroupNum + DPADCFG_OFFSET_Start;
	DGPIO_MEM_BASE = PCH_PCR_BASE_ADDRESS | DOFFSET_COM | DPadCfgReg;

	strLine.Format("Mem_address: 0x%08X\r\n", DGPIO_MEM_BASE);
	strText += strLine;
	GetDlgItem(IDC_EDIT_INFO)->SetWindowText(strText);

	DReg = GetPhysValue((PBYTE)DGPIO_MEM_BASE);
	return DReg;
}

UINT CGPIOSkylakeDlg::WorkThread(LPVOID lpvoid)
{
	CGPIOSkylakeDlg *p = (CGPIOSkylakeDlg*)lpvoid;
	CString strformt, strIDC_Group, strIDC_subGroup;
	CString strgroup[4], strsubgroup[4],strcommunity[4],stroffset[4];
	CString strRval;//GPIO bit 1组合值
	p->m_result.SetWindowText("Test");
	p->strText = "*************GPIO TOOL**************\r\n";
	CTime t = CTime::GetCurrentTime();
	p->startTime.Format("\r\nStartTime: %02d:%02d:%02d %04d-%02d-%02d\r\n", t.GetHour(), t.GetMinute(), t.GetSecond(), t.GetYear(), t.GetMonth(), t.GetDay());
	p->strText += "GPIO Read:\r\n";
	p->GetDlgItem(IDC_EDIT_INFO)->SetWindowText(p->strText);
	//p->DRegiterValue[4] = { 0 };
	for (int i = 0, j = 0; i < 4; i++)
	{
		p->m_Group[i].GetWindowText(strgroup[i]);
		p->m_SubGroup[i].GetWindowText(strsubgroup[i]);
		p->m_Community[i].GetWindowText(strcommunity[i]);
		p->m_OFFSET[i].GetWindowText(stroffset[i]);
		if (GPIOGroup[i].mIFREAD)
		{
			strformt.Format("[%s_%d]\r\n", strgroup[i], atoi(strsubgroup[i]));
			p->strText += strformt;
			p->DRegiterValue[j] = p->ReadGPIORegister(strgroup[i], atoi(strsubgroup[i]), atoi(strcommunity[i]), p->string_to_hex(stroffset[i].MakeLower())); //p->string_to_hex(stroffset[i].MakeLower())
			strformt.Format("GPIO_val   : 0x%08X\r\n", p->DRegiterValue[j]);
			p->strText += strformt;
			p->GetDlgItem(IDC_EDIT_INFO)->SetWindowText(p->strText);
			//get the GPIO val bit 1
			p->ibit1[j] = (p->DRegiterValue[j] >> 1) & 0x00000001;
			strformt.Format("bit 1      : %d\r\n",p->ibit1[j]);
			p->strText += strformt;
			p->GetDlgItem(IDC_EDIT_INFO)->SetWindowText(p->strText);
			j++;
		}
	}
	p->strText += "------------------------------\r\n";
	//strRval = p->ibit1[3] * 1000 + p->ibit1[2] * 100 + p->ibit1[1] * 10 + p->ibit1[0] * 1;
	strRval.Format("%d%d%d%d", p->ibit1[3], p->ibit1[2], p->ibit1[1], p->ibit1[0]);
	strformt.Format("out put    : %s\r\n", strRval);
	p->strText += strformt;
	p->GetDlgItem(IDC_EDIT_INFO)->SetWindowText(p->strText);

	CTime t2 = CTime::GetCurrentTime();
	p->EndTime.Format(_T("EndTime  : %02d:%02d:%02d %04d-%02d-%02d\r\n"), t2.GetHour(), t2.GetMinute(), t2.GetSecond(), t2.GetYear(), t2.GetMonth(), t2.GetDay());

	//*****************************************************************
	CString strval;
	strval.Format("%s",Con.sValue);
	if (strRval == strval)
	{
		p->bResult = TRUE;
		if (Con.mAutoPassExit)
		{
			p->SendMessage(WM_CLOSE);
		}
		p->m_result.SetWindowText("PASS");
		return 0;
	}
	else
	{
		p->bResult = FALSE;
		if (Con.mAutoFailExit)
		{
			p->SendMessage(WM_CLOSE);
		}
		p->m_result.SetWindowText("FAIL");
		return 1;
	}
}

void CGPIOSkylakeDlg::OnBnClickedButtonRead()
{
	// TODO:  在此添加控件通知处理程序代码
	//mThread->ResumeThread();
	//启动线程，
	mThread = AfxBeginThread((AFX_THREADPROC)WorkThread, this, 0, 0, NULL);
}


void CGPIOSkylakeDlg::OnBnClickedButtonSet()
{
	// TODO:  在此添加控件通知处理程序代码
	CString key, sgroup[4], ssubgroup[4], sCommunityNum[4], sPAD_CFG_OFFSET[4];
	for (int i = 0; i < 4; i++)
	{
		m_Group[i].GetWindowText(sgroup[i]);
		m_SubGroup[i].GetWindowText(ssubgroup[i]);
		m_Community[i].GetWindowText(sCommunityNum[i]);
		m_OFFSET[i].GetWindowText(sPAD_CFG_OFFSET[i]);
		key.Format("GPIO_%d", i);
		BOOL hr=WritePrivateProfileString(key, "Group", sgroup[i], cIniName);
		hr=WritePrivateProfileString(key, "SubGroupNum", ssubgroup[i], cIniName);
		hr = WritePrivateProfileString(key, "CommunityNum", sCommunityNum[i], cIniName);
		hr = WritePrivateProfileString(key, "PAD_CFG_OFFSET", sPAD_CFG_OFFSET[i], cIniName);
	}

}


DWORD CGPIOSkylakeDlg::string_to_hex(const char *str)
{
	int   i = 0;
	char  *index = "0123456789abcdef";              //记录查找索引  
	char  *temp=(char*)malloc(sizeof(str));                     //copy str  
	char  *find = NULL;
	DWORD dword = 0;
	StrCpy(temp, str);
	if (strstr(temp, "0x")) {                       //检测"ox"标记  
		strcpy_s(temp,sizeof(str), temp + 2);
	}
	while (i < strlen(temp)) {

		find = strchr(index, temp[i]);

		dword = dword ^ (((DWORD)(find - index)) << ((strlen(temp) - 1 - i) * 4));

		i++;
	}
	free(temp);
	return dword;

}



//BOOL CGPIOSkylakeDlg::WriteGPIO(CString Group, int SubGroupNum, int CommunityNum, DWORD dPAD_CFG_OFFSET,int mbit0)
//{
//	DWORD DReg = 0, DPadCfgReg, DPADCFG_OFFSET_Start, DOFFSET_COM, DGPIO_MEM_BASE;
//	BOOL hr;
//	switch (CommunityNum)
//	{
//	case 0:
//		DOFFSET_COM = PCH_GPIO_COM0;
//		break;
//	case 1:
//		DOFFSET_COM = PCH_GPIO_COM1;
//		break;
//	case 2:
//		DOFFSET_COM = PCH_GPIO_COM2;
//		break;
//	case 3:
//		DOFFSET_COM = PCH_GPIO_COM3;
//		break;
//	default:
//		break;
//	}
//	DPADCFG_OFFSET_Start = dPAD_CFG_OFFSET;
//	DPadCfgReg = 0x8 * SubGroupNum + DPADCFG_OFFSET_Start;
//	DGPIO_MEM_BASE = PCH_PCR_BASE_ADDRESS | DOFFSET_COM | DPadCfgReg;
//
//	DReg = GetPhysValue((PBYTE)DGPIO_MEM_BASE);
//
//	if (mbit0)
//	{
//		DReg = DReg | 0x01;
//	}
//	else
//	{
//		DReg = DReg & 0xFFFFFFFE;
//	}
//	hr = SetPhysValue((PBYTE)DGPIO_MEM_BASE, DReg);
//	return hr;
//}