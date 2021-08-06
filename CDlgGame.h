#pragma once
#include <string>
#include "mmsystem.h"
#include <thread>
#include <chrono>

#pragma comment(lib,"winmm.lib")

class Game;
class Com;
//声明全局变量
extern Game* g_pGame;
extern int g_second;
extern Com* g_pCom;

//定义自定义消息
#define WM_UPDATEMYCARD (WM_USER + 209)
#define WM_UPDATETIME (WM_USER + 210)
#define WM_CLISHOWDOWN (WM_USER + 211)
#define WM_DEALSMSG (WM_USER + 212)
#define WM_STARTSTATE3 (WM_USER + 213)
#define WM_STATE3CLIENT (WM_USER + 214)
#define WM_DEALPMSG_CLI (WM_USER + 215)
#define WM_DEALPMSG_SER (WM_USER + 216)
#define WM_PLAYCLI (WM_USER + 217)
#define WM_PLAYSER (WM_USER + 218)
#define WM_ROUNDSETTLE (WM_USER + 219)
#define WM_ENDOFGAME (WM_USER + 220)
#define WM_RESTART (WM_USER + 221)

#define ZUOCI(order) (order==1?("座次：北"):(order==2?("座次：西"):(order==3?("座次：南"):("座次：东"))))
// CDlgGame 对话框

class CDlgGame : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGame)

public:
	CDlgGame(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgGame();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();

public:
	void SetStatic();//游戏开始时设定八个静态文本框
	int m_szId_Down;
	int m_szId_Up;
	int m_szId_Left;
	int m_szId_Right;
	CString m_szOrder_Down;
	CString m_szOrder_Left;
	CString m_szOrder_Right;
	CString m_szOrder_Up;
	CButton m_Button_Card[13];
	int m_iScore_Up = 0;
	int m_iScore_Right = 0;
	int m_iScore_Left = 0;
	int m_iScore_Down = 0;
	int m_iGold_Up;
	int m_iGold_Right;
	int m_iGold_Down;
	int m_iGold_Left;
	int m_iTime;
	CStatic m_Static_Tip;
	CButton m_Check_Sheep;
	CButton m_Check_Trans;
	CButton m_Check_HeartA;
	CButton m_Check_Pig;
	CString m_szStatic_Collection_Up;
	CString m_szStatic_Collection_Down;
	CString m_szStatic_Collection_Left;
	CString m_szStatic_Collection_Right;
	CButton m_Button_Show_Left;
	CButton m_Button_Show_Up;
	CButton m_Button_Show_Right;
	CButton m_Button_Show_Down;
	CButton m_Button_Restart;
	CButton m_Check_Sound;
	CStatic m_Static_Com;

	void ChangeButtonBitmap(int i, int card);//改变按钮i的位图   展示按钮：-1下 -2右  -3上  -4左
	afx_msg LRESULT OnUpdatemycard(WPARAM wParam, LPARAM lParam); //根据myCardg更新按钮位图和可用
	afx_msg LRESULT OnUpdatetime(WPARAM wParam, LPARAM lParam);//更新倒计时显示
	UINT static ThreadShowDown_Server(LPVOID lParam);//服务器执行亮牌线程
	void ShowDown_Server();//服务器亮牌线程
	void SetMessage(std::string infor, int order);//给特征牌静态框添加信息
	UINT static ThreadShowDown_Client(LPVOID lParam);//服务器执行亮牌线程
	void ShowDown_Client();//服务器亮牌线程
	afx_msg LRESULT OnClishowdown(WPARAM wParam, LPARAM lParam);//服务器执行亮牌
	afx_msg LRESULT OnDealsmsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStartstate3(WPARAM wParam, LPARAM lParam);//开始第三阶段
	afx_msg void OnClickedButtonCard1();
	afx_msg void OnClickedButtonCard10();
	afx_msg void OnClickedButtonCard11();
	afx_msg void OnClickedButtonCard12();
	afx_msg void OnClickedButtonCard13();
	afx_msg void OnClickedButtonCard2();
	afx_msg void OnClickedButtonCard3();
	afx_msg void OnClickedButtonCard4();
	afx_msg void OnClickedButtonCard5();
	afx_msg void OnClickedButtonCard6();
	afx_msg void OnClickedButtonCard7();
	afx_msg void OnClickedButtonCard8();
	afx_msg void OnClickedButtonCard9();
	afx_msg LRESULT OnState3client(WPARAM wParam, LPARAM lParam);
	UINT static Play_Server(LPVOID lParam);//服务器执行出牌线程
	void Play_Server();//服务器出牌线程
	void JudgeRange();//根据temp1判断出牌范围并更新按钮可用
	void BanCardButton();//禁用所有出牌按钮
	UINT static Play_Client(LPVOID lParam);//客户端执行出牌线程
	void Play_Client();//客户端出牌线程
	afx_msg LRESULT OnDealpmsg_Cli(WPARAM wParam, LPARAM lParam);//客户端处理出牌信息
	afx_msg LRESULT OnDealpmsg_Ser(WPARAM wParam, LPARAM lParam);//服务器处理出牌信息
	afx_msg LRESULT OnPlaycli(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRoundsettle(WPARAM wParam, LPARAM lParam);//回合结算
	void SetCollectionMsg(int card, int order);//给特征牌静态框添加获得特征牌的信息
	void UpdateScore(int order);//根据Collection[order-1]更新分数
	void RecardScore();//保存对局到到文件"Recard.txt"
	int GainGold(int order);//返回玩家获得的金币
	void SaveData();//存ID和Gold到"Sav.dat"
	int GetScore(int order);//返回玩家的分数
	afx_msg LRESULT OnEndofgame(WPARAM wParam, LPARAM lParam);//游戏结算
	afx_msg void OnClickedButtonRestart();//开始新一局游戏
	afx_msg LRESULT OnRestart(WPARAM wParam, LPARAM lParam);//开始新一局游戏
	afx_msg void OnClickedCheckSound();
	BOOL PreTranslateMessage(MSG* pMsg)//屏蔽热键
	{
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
		else
			return CDialog::PreTranslateMessage(pMsg);
	}
	
};
