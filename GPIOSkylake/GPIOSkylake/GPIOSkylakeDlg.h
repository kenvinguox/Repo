
// GPIOSkylakeDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CGPIOSkylakeDlg dialog
class CGPIOSkylakeDlg : public CDialogEx
{
// Construction
public:
	CGPIOSkylakeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GPIOSKYLAKE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CBrush brush;
	HBRUSH m_brush;
	static UINT WorkThread(LPVOID lpvoid);
	CWinThread *mThread;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();

	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonSet();

	BOOL ReadINI();
	void WriteINI();
	DWORD ReadGPIORegister(CString Group, int SubGroupNum, int CommunityNum, DWORD dPAD_CFG_OFFSET);
	CString NoteList();
	DWORD string_to_hex(const char *str);

public:
	CString Logname;
	CString strLine;
	CString sResultText;
	CString strText, cIniName;
	DWORD DRegiterValue[4];
	int ibit1[4];
	BOOL bResult;

	CComboBox m_Group[4];
	CComboBox m_SubGroup[4];
	CComboBox m_Community[4];
	CEdit m_OFFSET[4];
	CString startTime, EndTime;
	
	CEdit m_result;

};
