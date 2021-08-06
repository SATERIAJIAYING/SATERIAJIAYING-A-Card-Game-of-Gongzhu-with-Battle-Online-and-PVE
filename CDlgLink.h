#pragma once
#include <Windows.h>

//定义自定义消息
#define WM_CLIENTLOGIN (WM_USER + 200)
#define WM_CLOSEBUTTON (WM_USER + 201)
#define WM_BROADCASTID (WM_USER + 202)
#define WM_BROADCASTORDER (WM_USER + 204)
#define WM_UPDATALIST (WM_USER + 205)
#define WM_STARTEXORDER (WM_USER + 206)
#define WM_STARTGAME (WM_USER + 207)

class user;
class server;
class client;
class CDlgGame;
//全局变量声明  
extern int g_state;//1连接阶段 2发牌阶段 
extern int g_user_num;
extern user* g_pUser[4];
extern int temp,temp1,temp2;
extern server* g_pServer;
extern client* g_pClient;
extern int g_c_port;
extern CDlgGame* g_pCDlgGame;
extern int g_Model;


// CDlgLink 对话框

class CDlgLink : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLink)

public:
	CDlgLink(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgLink();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1_LINK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	// 连接对话框要显示的内容
//	CStatic m_Static_Link;
public:
	virtual BOOL OnInitDialog();
private:
	// 连接菜单的控件变量
	CListCtrl m_List_Link;

	void InsertPlayer(int i, int PlayerID, int order);//在列表中插入第i-1个玩家

	// 按钮 西
	CButton m_Button_Order2;
	// 按钮 南
	CButton m_Button_Order3;
	// 按钮 东
	CButton m_Button_Order4;
private:
	UINT static ThreadServer(LPVOID  lParam);//线程执行函数
	void RunServer();//线程执行函数调用
protected:
	afx_msg LRESULT OnClientlogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClosebutton(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBroadcastid(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnExchangeorder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBroadcastorder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatalist(WPARAM wParam, LPARAM lParam);
public:
	//客户端发送线程
	//UINT static ThreadClientSend(LPVOID  lParam);//线程执行函数
	//void RunClientSend();//线程执行函数调用
	//客户端接收线程
	UINT static ThreadClientRecv(LPVOID  lParam);//线程执行函数
	void RunClientRecv();//线程执行函数调用

	void DealBMsg(char* recv_);//处理游戏成员更新信息
	void DealEMsg(char* recv_);//处理换座信息
	afx_msg void OnClickedButtonOrder2();
	afx_msg void OnClickedButtonOrder3();
	afx_msg void OnClickedButtonOrder4();
protected:
	afx_msg LRESULT OnStartexorder(WPARAM wParam, LPARAM lParam);
private:
	// 开始游戏按钮控制按键
	CButton m_Button_Startgame;
public:
	afx_msg void OnClickedButton1Startgame();
protected:
	afx_msg LRESULT OnStartgame(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	void DealSMsg(char* recv_);//处理亮牌信息
	void GetHostAddress(CString& strIPAddr);//获取本机IP地址
	afx_msg void OnClickedButtonGetip();
	CButton m_Check_Com;
	CComboBox m_Combo_Degree;
	afx_msg void OnClickedCheckCom();
	bool IsThreadEnd = false;
	BOOL PreTranslateMessage(MSG* pMsg)//屏蔽热键
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}
	afx_msg void OnEditupdateCombo();
	bool IsLogon = false;
};

