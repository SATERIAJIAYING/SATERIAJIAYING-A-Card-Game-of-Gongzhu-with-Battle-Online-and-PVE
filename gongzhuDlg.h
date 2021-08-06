#pragma once

// gongzhuDlg.h: 头文件
//
#include<string>
#include "CDlgLink.h"

class CgongzhuDlg;
//全局变量声明
extern int g_id;
extern int g_order;//开始菜单1为主机，0为客户端
extern std::string g_ipAddr;
extern CDlgLink* g_pCDlgLink;
extern CgongzhuDlg* g_pCgongzhuDlg;

// CgongzhuDlg 对话框
class CgongzhuDlg : public CDialogEx
{
	friend class CDislogTest;
// 构造
public:
	CgongzhuDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GONGZHU_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_Edit_Start;
private:
	// 储存玩家输入的ID
	int m_iEdit_Start;
private:
	// 作为主机登录复选框
	CButton m_Check_Start;
public:
	afx_msg void OnBnClickedOk();
private:
	// ip地址编辑框控制变量
	CEdit m_Edit_Ip;
	// 保存输入的ip地址
	CString m_szEdit_Ip;
public:
	afx_msg void OnClickedCheck1Start();
	CButton m_Button_Rule;
	afx_msg void OnClickedButtonRule();
	afx_msg void OnClickedButtonRank();
	BOOL PreTranslateMessage(MSG* pMsg)//屏蔽热键
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}
	afx_msg void OnClickedButtonPaste();
	CButton m_Button_Paste;
	afx_msg void OnClickedButtonGenerate();
	afx_msg void OnChangeEdit1Start();
	bool IsFirstSet = true;
};


struct Player
{
	int ID;
	int gold;
};