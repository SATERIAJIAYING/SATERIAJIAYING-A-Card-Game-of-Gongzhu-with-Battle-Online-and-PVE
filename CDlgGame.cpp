// CDlgGame.cpp: 实现文件
//

#include "pch.h"
#include "gongzhu.h"
#include "CDlgGame.h"
#include "afxdialogex.h"
#include "gongzhuDlg.h"
#include "CDlgLink.h"
#include "net.h"
#include "game.h"
#include "resource.h"
#include "Com.h"
#include <sstream>

//定义全局变量
Game* g_pGame;
int g_second;
Com* g_pCom;

// CDlgGame 对话框

IMPLEMENT_DYNAMIC(CDlgGame, CDialogEx)

CDlgGame::CDlgGame(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GAME, pParent)
	, m_iScore_Up(0)
	, m_iScore_Right(0)
	, m_iScore_Left(0)
	, m_iScore_Down(0)
	, m_iGold_Up(0)
	, m_iGold_Right(0)
	, m_iGold_Down(0)
	, m_iGold_Left(0)
	, m_iTime(0)
{

}

CDlgGame::~CDlgGame()
{

}

void CDlgGame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_ID_DOWN, m_szId_Down);
	DDX_Text(pDX, IDC_STATIC_ID_UP, m_szId_Up);
	DDX_Text(pDX, IDC_STATIC_ID_LEFT, m_szId_Left);
	DDX_Text(pDX, IDC_STATIC_ID_RIGHT, m_szId_Right);
	DDX_Text(pDX, IDC_STATIC_ORDER_DOWN, m_szOrder_Down);
	DDX_Text(pDX, IDC_STATIC_ORDER_LEFT, m_szOrder_Left);
	DDX_Text(pDX, IDC_STATIC_ORDER_RIGHT, m_szOrder_Right);
	DDX_Text(pDX, IDC_STATIC_ORDER_UP, m_szOrder_Up);
	DDX_Control(pDX, IDC_BUTTON_CARD1, m_Button_Card[0]);
	DDX_Control(pDX, IDC_BUTTON_CARD2, m_Button_Card[1]);
	DDX_Control(pDX, IDC_BUTTON_CARD3, m_Button_Card[2]);
	DDX_Control(pDX, IDC_BUTTON_CARD4, m_Button_Card[3]);
	DDX_Control(pDX, IDC_BUTTON_CARD5, m_Button_Card[4]);
	DDX_Control(pDX, IDC_BUTTON_CARD6, m_Button_Card[5]);
	DDX_Control(pDX, IDC_BUTTON_CARD7, m_Button_Card[6]);
	DDX_Control(pDX, IDC_BUTTON_CARD8, m_Button_Card[7]);
	DDX_Control(pDX, IDC_BUTTON_CARD9, m_Button_Card[8]);
	DDX_Control(pDX, IDC_BUTTON_CARD10, m_Button_Card[9]);
	DDX_Control(pDX, IDC_BUTTON_CARD11, m_Button_Card[10]);
	DDX_Control(pDX, IDC_BUTTON_CARD12, m_Button_Card[11]);
	DDX_Control(pDX, IDC_BUTTON_CARD13, m_Button_Card[12]);
	DDX_Text(pDX, IDC_STATIC_SCORE_UP, m_iScore_Up);
	DDX_Text(pDX, IDC_STATIC_SCORE_RIGHT, m_iScore_Right);
	DDX_Text(pDX, IDC_STATIC_SCORE_LEFT, m_iScore_Left);
	DDX_Text(pDX, IDC_STATIC_SCORE_DOWN, m_iScore_Down);
	DDX_Text(pDX, IDC_STATIC_GOLD_UP, m_iGold_Up);
	DDX_Text(pDX, IDC_STATIC_GOLD_RIGHT, m_iGold_Right);
	DDX_Text(pDX, IDC_STATIC_GOLD_DOWN, m_iGold_Down);
	DDX_Text(pDX, IDC_STATIC_GOLD_LEFT, m_iGold_Left);
	DDX_Text(pDX, IDC_STATIC_TIME, m_iTime);
	DDX_Control(pDX, IDC_STATIC_TIP, m_Static_Tip);
	DDX_Control(pDX, IDC_CHECK_SHEEP, m_Check_Sheep);
	DDX_Control(pDX, IDC_CHECK_TRANS, m_Check_Trans);
	DDX_Control(pDX, IDC_CHECK_HEARTA, m_Check_HeartA);
	DDX_Control(pDX, IDC_CHECK_PIG, m_Check_Pig);
	DDX_Text(pDX, IDC_STATIC_COLLECTION_UP, m_szStatic_Collection_Up);
	DDX_Text(pDX, IDC_STATIC_COLLECTION_DOWN, m_szStatic_Collection_Down);
	DDX_Text(pDX, IDC_STATIC_COLLECTION_LEFT, m_szStatic_Collection_Left);
	DDX_Text(pDX, IDC_STATIC_COLLECTION_RIGHT, m_szStatic_Collection_Right);
	DDX_Control(pDX, IDC_BUTTON_SHOWCARD_LEFT, m_Button_Show_Left);
	DDX_Control(pDX, IDC_BUTTON_SHOWCARD_UP, m_Button_Show_Up);
	DDX_Control(pDX, IDC_BUTTON_SHOWCARD_RIGHT, m_Button_Show_Right);
	DDX_Control(pDX, IDC_BUTTON_SHOWCARD_DOWN, m_Button_Show_Down);
	DDX_Control(pDX, IDC_BUTTON_RESTART, m_Button_Restart);
	DDX_Control(pDX, IDC_CHECK_SOUND, m_Check_Sound);
	DDX_Control(pDX, IDC_STATIC_Com, m_Static_Com);
}


BEGIN_MESSAGE_MAP(CDlgGame, CDialogEx)
	ON_MESSAGE(WM_STATE3CLIENT, &CDlgGame::OnState3client)
	ON_BN_CLICKED(IDC_BUTTON_CARD9, &CDlgGame::OnClickedButtonCard9)
	ON_BN_CLICKED(IDC_BUTTON_CARD8, &CDlgGame::OnClickedButtonCard8)
	ON_BN_CLICKED(IDC_BUTTON_CARD7, &CDlgGame::OnClickedButtonCard7)
	ON_BN_CLICKED(IDC_BUTTON_CARD6, &CDlgGame::OnClickedButtonCard6)
	ON_BN_CLICKED(IDC_BUTTON_CARD5, &CDlgGame::OnClickedButtonCard5)
	ON_BN_CLICKED(IDC_BUTTON_CARD4, &CDlgGame::OnClickedButtonCard4)
	ON_BN_CLICKED(IDC_BUTTON_CARD3, &CDlgGame::OnClickedButtonCard3)
	ON_BN_CLICKED(IDC_BUTTON_CARD2, &CDlgGame::OnClickedButtonCard2)
	ON_BN_CLICKED(IDC_BUTTON_CARD13, &CDlgGame::OnClickedButtonCard13)
	ON_BN_CLICKED(IDC_BUTTON_CARD12, &CDlgGame::OnClickedButtonCard12)
	ON_BN_CLICKED(IDC_BUTTON_CARD11, &CDlgGame::OnClickedButtonCard11)
	ON_BN_CLICKED(IDC_BUTTON_CARD10, &CDlgGame::OnClickedButtonCard10)
	ON_BN_CLICKED(IDC_BUTTON_CARD1, &CDlgGame::OnClickedButtonCard1)
	ON_BN_CLICKED(IDC_BUTTON_RESTART, &CDlgGame::OnClickedButtonRestart)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_UPDATEMYCARD, &CDlgGame::OnUpdatemycard)
	ON_MESSAGE(WM_UPDATETIME, &CDlgGame::OnUpdatetime)
	ON_MESSAGE(WM_CLISHOWDOWN, &CDlgGame::OnClishowdown)
	ON_MESSAGE(WM_DEALSMSG, &CDlgGame::OnDealsmsg)
	ON_MESSAGE(WM_STARTSTATE3, &CDlgGame::OnStartstate3)
	ON_MESSAGE(WM_STATE3CLIENT, &CDlgGame::OnState3client)
	ON_MESSAGE(WM_DEALPMSG_CLI, &CDlgGame::OnDealpmsg_Cli)
	ON_MESSAGE(WM_DEALPMSG_SER, &CDlgGame::OnDealpmsg_Ser)
	ON_MESSAGE(WM_PLAYCLI, &CDlgGame::OnPlaycli)
	ON_MESSAGE(WM_PLAYSER, &CDlgGame::OnPlayser)
	ON_MESSAGE(WM_ROUNDSETTLE, &CDlgGame::OnRoundsettle)
	ON_MESSAGE(WM_ENDOFGAME, &CDlgGame::OnEndofgame)
	ON_MESSAGE(WM_RESTART, &CDlgGame::OnRestart)
	ON_BN_CLICKED(IDC_CHECK_SOUND, &CDlgGame::OnClickedCheckSound)
END_MESSAGE_MAP()


// CDlgGame 消息处理程序


BOOL CDlgGame::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	srand((unsigned)time(NULL));

	m_Button_Restart.EnableWindow(FALSE);//禁用重开按钮

	ModifyStyleEx(0, WS_EX_APPWINDOW);//在任务栏中显示
	SetStatic();//改变静态文本

	m_Check_HeartA.EnableWindow(FALSE);//禁用亮牌按钮
	m_Check_Pig.EnableWindow(FALSE);
	m_Check_Sheep.EnableWindow(FALSE);
	m_Check_Trans.EnableWindow(FALSE);


	for (int i = -1; i > -5; i--)
	{
		ChangeButtonBitmap(i, 0);
	}

	g_pGame = new Game;
	
	if (g_Model != 0)
		m_Static_Com.SetWindowText(_T("机器人"));

	if (g_order == 1)//主机端
	{

		g_pGame->Deal();//发牌
		SendMessage(WM_UPDATEMYCARD, 0, 0);//根据myCardg更新按钮位图和可用

		AfxBeginThread(ThreadShowDown_Server, (LPVOID)this);//创建亮牌线程
	}
	else//客户端
	{
		if (g_Model != 0)//机器人模式默认静音
		{
			m_Check_Sound.SetCheck(TRUE);

			g_pCom = new Com;
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



void CDlgGame::OnClose()
{
	delete g_pGame;
	g_pCDlgLink->SendMessage(WM_CLOSE, 0, 0);//关闭该窗口也同时关闭连接窗口
	CDialogEx::OnClose();
}

void CDlgGame::SetStatic()//游戏开始时设定ID，GOLD，座次 静态文本框
{
	
	m_szId_Down = g_pUser[g_order - 1]->ID;
	m_iGold_Down = g_pUser[g_order - 1]->gold;

	if (g_pUser[g_order % 4])
	{ 
		m_szId_Right = g_pUser[g_order % 4]->ID;
		m_iGold_Right = g_pUser[g_order % 4]->gold;
	}
	else
	{
		m_szId_Right = -1;
		m_iGold_Right = 1000;
	}
		
	if (g_pUser[(g_order + 1) % 4])
	{
		m_szId_Up = g_pUser[(g_order + 1) % 4]->ID;
		m_iGold_Up = g_pUser[(g_order + 1) % 4]->gold;
	}
	else
	{
		m_szId_Up = -1;
		m_iGold_Up = 1000;
	}

	if (g_pUser[(g_order + 2) % 4])
	{
		m_szId_Left = g_pUser[(g_order + 2) % 4]->ID;
		m_iGold_Left = g_pUser[(g_order + 2) % 4]->gold;
	}
	else
	{
		m_szId_Left = -1;
		m_iGold_Left = 1000;
	}

	m_szOrder_Down = ZUOCI(g_order);
	m_szOrder_Right = ZUOCI((g_order + 1) % 4);
	m_szOrder_Up = ZUOCI((g_order + 2) % 4);
	m_szOrder_Left = ZUOCI((g_order + 3) % 4);

	UpdateData(FALSE);
}


void CDlgGame::ChangeButtonBitmap(int i, int card)//改变按钮i的位图   展示按钮：-1下 -2右  -3上  -4左
{
	HBITMAP hBmp;
	switch (card)
	{
	case 1:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1)); break;
	case 2:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2)); break;
	case 3:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP3)); break;
	case 4:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP4)); break;
	case 5:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP5)); break;
	case 6:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP6)); break;
	case 7:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP7)); break;
	case 8:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP8)); break;
	case 9:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP9)); break;
	case 10:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP10)); break;
	case 11:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP11)); break;
	case 12:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP12)); break;
	case 13:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP13)); break;
	case 14:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP14)); break;
	case 15:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP15)); break;
	case 16:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP16)); break;
	case 17:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP17)); break;
	case 18:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP18)); break;
	case 19:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP19)); break;
	case 20:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP20)); break;
	case 21:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP21)); break;
	case 22:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP22)); break;
	case 23:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP23)); break;
	case 24:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP24)); break;
	case 25:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP25)); break;
	case 26:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP26)); break;
	case 27:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP27)); break;
	case 28:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP28)); break;
	case 29:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP29)); break;
	case 30:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP30)); break;
	case 31:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP31)); break;
	case 32:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP32)); break;
	case 33:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP33)); break;
	case 34:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP34)); break;
	case 35:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP35)); break;
	case 36:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP36)); break;
	case 37:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP37)); break;
	case 38:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP38)); break;
	case 39:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP39)); break;
	case 40:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP40)); break;
	case 41:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP41)); break;
	case 42:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP42)); break;
	case 43:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP43)); break;
	case 44:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP44)); break;
	case 45:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP45)); break;
	case 46:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP46)); break;
	case 47:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP47)); break;
	case 48:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP48)); break;
	case 49:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP49)); break;
	case 50:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP50)); break;
	case 51:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP51)); break;
	case 52:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP52)); break;
	default:hBmp = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP53));
	}
	if(i>=0)
		m_Button_Card[i].SetBitmap(hBmp);
	if (i == -1)
		m_Button_Show_Down.SetBitmap(hBmp);
	if (i == -2)
		m_Button_Show_Right.SetBitmap(hBmp);
	if (i == -3)
		m_Button_Show_Up.SetBitmap(hBmp);
	if (i == -4)
		m_Button_Show_Left.SetBitmap(hBmp);
		
	DeleteObject(hBmp);
}



afx_msg LRESULT CDlgGame::OnUpdatemycard(WPARAM wParam, LPARAM lParam)//根据myCardg更新按钮位图和可用
{
	for (int i = 0; i < 13; i++)
	{
		ChangeButtonBitmap(i, g_pGame->myCard[i].data);//更改位图

		if(g_pGame->myCard[i].Usable)//更改可用
			m_Button_Card[i].EnableWindow(TRUE);
		else
			m_Button_Card[i].EnableWindow(FALSE);
	}

	return 0;
}



afx_msg LRESULT CDlgGame::OnUpdatetime(WPARAM wParam, LPARAM lParam)//更新倒计时显示
{
	SetDlgItemInt(IDC_STATIC_TIME, g_second);
	return 0;
}

UINT CDlgGame::ThreadShowDown_Server(LPVOID lParam)//服务器执行亮牌线程
{
	CDlgGame* pWnd = (CDlgGame*)lParam;       //将窗口指针赋给无类型指针  
	pWnd->ShowDown_Server();

	return 1;
}

void CDlgGame::ShowDown_Server()//服务器亮牌线程执行函数
{
	Sleep(4999);

	if (m_Check_Sound.GetCheck() == FALSE)
		PlaySound(MAKEINTRESOURCE(IDR_WAVE_SHOWDOWN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);//播放亮牌音乐

	g_pGame->PlayedCollection = 0;
	
	for (int i = 0; i < 13; i++)//检查是否可以选择亮牌，开启复选框
	{
		if (g_pGame->myCard[i].data == 11)
			m_Check_Sheep.EnableWindow(TRUE);
		if (g_pGame->myCard[i].data == 23)
			m_Check_Trans.EnableWindow(TRUE);
		if (g_pGame->myCard[i].data == 27)
			m_Check_HeartA.EnableWindow(TRUE);
		if (g_pGame->myCard[i].data == 51)
			m_Check_Pig.EnableWindow(TRUE);
	}

	m_Static_Tip.SetWindowText(_T("↓                       ↓          请选择要亮的牌          ↓                       ↓"));

	g_second = 10;//开始计时
	while (g_second > -1)
	{
		SendMessage(WM_UPDATETIME, 0, 0);
		Sleep(999);
		g_second--;

	}
	m_Check_Sheep.EnableWindow(FALSE);//禁用亮牌复选框
	m_Check_Trans.EnableWindow(FALSE);
	m_Check_HeartA.EnableWindow(FALSE);
	m_Check_Pig.EnableWindow(FALSE);

	m_Static_Tip.SetWindowText(_T("请等待其他玩家选择亮牌"));


	//发送亮牌数据：'S' + order + boolSheep + boolTrans + boolHeartA + boolPig  如"S31010"
	string infor = "S1";
	if (m_Check_Sheep.GetCheck() == TRUE)
	{
		infor += "1";
		SetMessage("亮方块J", 1);
		g_pGame->BSheep = true;
	}
	else
		infor += "0";
	if (m_Check_Trans.GetCheck() == TRUE)
	{
		infor += "1";
		SetMessage("亮梅花10", 1);
		g_pGame->BTrans = true;
	}
	else
		infor += "0";
	if (m_Check_HeartA.GetCheck() == TRUE)
	{
		infor += "1";
		SetMessage("亮红桃A", 1);
		g_pGame->BHeartA = true;
	}
	else
		infor += "0";
	if (m_Check_Pig.GetCheck() == TRUE)
	{
		infor += "1";
		SetMessage("亮黑桃Q", 1);
		g_pGame->BPig = true;
	}
	else
		infor += "0";

	for (int i = 1; i < 4; i++)//广播消息
	{
		g_pServer->Send_Message(infor, g_pUser[i]->receiver);
	
	}
		

}

UINT CDlgGame::ThreadShowDown_Client(LPVOID lParam)//客户端执行亮牌线程
{
	CDlgGame* pWnd = (CDlgGame*)lParam;       //将窗口指针赋给无类型指针  
	pWnd->ShowDown_Client();

	return 1;
}

void CDlgGame::ShowDown_Client()//客户端亮牌线程执行函数
{
	g_pGame->PlayedCollection = 0;

	bool sheep = false, trans = false, heartA = false, pig = false;

	for (int i = 0; i < 13; i++)//检查是否可以选择亮牌，开启复选框
	{
		if (g_pGame->myCard[i].data == 11)
		{
			if(g_Model==0)
				m_Check_Sheep.EnableWindow(TRUE);
			sheep = true;
		}	
		if (g_pGame->myCard[i].data == 23)
		{ 
			if (g_Model == 0)
				m_Check_Trans.EnableWindow(TRUE);
			trans = true;
		}
		if (g_pGame->myCard[i].data == 27)
		{ 
			if (g_Model == 0)
				m_Check_HeartA.EnableWindow(TRUE);
			heartA = true;
		}
		if (g_pGame->myCard[i].data == 51)
		{ 
			if (g_Model == 0)
				m_Check_Pig.EnableWindow(TRUE);
			pig = true;
		}
	}

	m_Static_Tip.SetWindowText(_T("↓                       ↓          请选择要亮的牌          ↓                       ↓"));

	g_second = 10;//开始计时
	while (g_second > -1)
	{
		SendMessage(WM_UPDATETIME, 0, 0);
		Sleep(999);
		g_second--;

	}
	m_Check_Sheep.EnableWindow(FALSE);//禁用亮牌复选框
	m_Check_Trans.EnableWindow(FALSE);
	m_Check_HeartA.EnableWindow(FALSE);
	m_Check_Pig.EnableWindow(FALSE);

	m_Static_Tip.SetWindowText(_T("请等待其他玩家选择亮牌"));

	if (g_Model >= 2)
	{
		m_Check_Sheep.SetCheck(sheep && g_pCom->Showdown_Sheep());
		m_Check_Trans.SetCheck(trans && g_pCom->Showdown_Trans());
		m_Check_HeartA.SetCheck(heartA && g_pCom->Showdown_HeartA());
		m_Check_Pig.SetCheck(pig && g_pCom->Showdown_Pig());
	}


	//发送亮牌数据：'S' + order + boolSheep + boolTrans + boolHeartA + boolPig  如"S31010"
	string infor = "S"+ to_string(g_order);
	if (m_Check_Sheep.GetCheck() == TRUE)
	{
		infor += "1";
		SetMessage("亮方块J", g_order);
		g_pGame->BSheep = true;
	}
	else
		infor += "0";
	if (m_Check_Trans.GetCheck() == TRUE)
	{
		infor += "1";
		SetMessage("亮梅花10", g_order);
		g_pGame->BTrans = true;
	}
	else
		infor += "0";
	if (m_Check_HeartA.GetCheck() == TRUE)
	{
		infor += "1";
		SetMessage("亮红桃A", g_order);
		g_pGame->BHeartA = true;
	}
	else
		infor += "0";
	if (m_Check_Pig.GetCheck() == TRUE)
	{
		infor += "1";
		SetMessage("亮黑桃Q", g_order);
		g_pGame->BPig = true;
	}
	else
		infor += "0";

	//发送消息给服务器
	g_pClient->Send_Message(infor);

}



void CDlgGame::SetMessage(std::string infor, int order)//给特征牌静态框添加信息
{
	if (order == (g_order - 1) % 4 + 1)
	{
		m_szStatic_Collection_Down += CString((infor + "  ").c_str());
		GetDlgItem(IDC_STATIC_COLLECTION_DOWN)->SetWindowTextW(m_szStatic_Collection_Down);
	}
	if (order == (g_order) % 4 + 1)
	{
		m_szStatic_Collection_Right += CString((infor + "\n").c_str());
		GetDlgItem(IDC_STATIC_COLLECTION_RIGHT)->SetWindowTextW(m_szStatic_Collection_Right);
	}
	if (order == (g_order + 1) % 4 + 1)
	{
		m_szStatic_Collection_Up += CString((infor + "  ").c_str());
		GetDlgItem(IDC_STATIC_COLLECTION_UP)->SetWindowTextW(m_szStatic_Collection_Up);
	}
	if (order == (g_order + 2) % 4 + 1)
	{
		m_szStatic_Collection_Left += CString((infor + "\n").c_str());
		GetDlgItem(IDC_STATIC_COLLECTION_LEFT)->SetWindowTextW(m_szStatic_Collection_Left);
	}

}


afx_msg LRESULT CDlgGame::OnClishowdown(WPARAM wParam, LPARAM lParam)//服务器执行亮牌
{
	AfxBeginThread(ThreadShowDown_Client, (LPVOID)this);//创建亮牌线程
	return 0;
}

afx_msg LRESULT CDlgGame::OnDealsmsg(WPARAM wParam, LPARAM lParam)//处理亮牌信息
{
	char* recv_;
	if (g_order == 1)
		recv_ = g_pServer->recv_buf;
	else
		recv_ = g_pClient->recv_buf;

	string strtemp = recv_;
	int order = atoi((strtemp.substr(1, 1)).c_str());

	bool sheep = false, trans = false, heartA = false, pig = false;
	if (atoi((strtemp.substr(2, 1)).c_str()))
	{
		sheep = g_pGame->BSheep = TRUE;
		g_pCDlgGame->SetMessage("亮方块J", order);
	}
	if (atoi((strtemp.substr(3, 1)).c_str()))
	{
		trans = g_pGame->BTrans = TRUE;
		g_pCDlgGame->SetMessage("亮梅花10", order);
	}
	if (atoi((strtemp.substr(4, 1)).c_str()))
	{
		heartA = g_pGame->BHeartA = TRUE;
		g_pCDlgGame->SetMessage("亮红桃A", order);
	}
	if (atoi((strtemp.substr(5, 1)).c_str()))
	{
		pig = g_pGame->BPig = TRUE;
		g_pCDlgGame->SetMessage("亮黑桃Q", order);
	}

	if (g_Model >= 2)
		g_pCom->AcceptShowdownMsg(order, sheep, trans, heartA, pig);

	return 0;
}


afx_msg LRESULT CDlgGame::OnStartstate3(WPARAM wParam, LPARAM lParam)//服务器开始第三阶段
{
	g_state++;//第三阶段出牌阶段
	for (int i = 1; i < 4; i++)
	{
		g_pServer->Send_Message("PLAY", g_pUser[i]->receiver);//广播信息
	}

	m_Static_Tip.SetWindowText(_T("请等待其他玩家出牌"));

	for (int i = 1; i < 5; i++)//分隔亮牌和特征牌
		SetMessage("  ", i);

	if (g_pGame->CurrentOrder == 0)//判断是否首局首轮
	{
		for(int i=0;i<13;i++)
			if (g_pGame->myCard[i].data == 15)//找梅花2
			{
				
				temp1 = -1;

				AfxBeginThread(Play_Server, (LPVOID)this);//创建开始出牌线程
				return 0;

			}
	}
	else
	{
		if (g_pGame->CurrentOrder == g_order)//上一局获得猪的玩家会被指定
		{
			
			temp1 = 0;//可以出任意牌

			AfxBeginThread(Play_Server, (LPVOID)this);//创建开始出牌线程

		}
	}

	return 0;
}


void CDlgGame::OnClickedButtonCard1()
{
	temp = 0;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard10()
{
	temp = 9;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard11()
{
	temp = 10;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard12()
{
	temp = 11;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard13()
{
	temp = 12;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard2()
{
	temp = 1;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard3()
{
	temp = 2;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard4()
{
	temp = 3;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard5()
{
	temp = 4;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard6()
{
	temp = 5;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard7()
{
	temp = 6;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard8()
{
	temp = 7;
	g_second = -2;
}


void CDlgGame::OnClickedButtonCard9()
{
	temp = 8;
	g_second = -2;
}


afx_msg LRESULT CDlgGame::OnState3client(WPARAM wParam, LPARAM lParam)//客户端开始出牌
{
	g_state++;
	m_Static_Tip.SetWindowText(_T("请等待其他玩家出牌"));

	for(int i=1;i<5;i++)//分隔亮牌和特征牌
		SetMessage("  ",i);

	if (g_pGame->CurrentOrder == 0)//判断是否首局首轮
	{
		for (int i = 0; i < 13; i++)
			if (g_pGame->myCard[i].data == 15)//找梅花2
			{
				
				temp1 = -1;

				AfxBeginThread(Play_Client, (LPVOID)this);//创建开始出牌线程
				return 0;

			}
	}
	else
	{
		if (g_pGame->CurrentOrder == g_order)//上一局获得猪的玩家会被指定
		{
			
			temp1 = 0;//可以出任意牌

			AfxBeginThread(Play_Client, (LPVOID)this);//创建开始出牌线程
			
		}
	}
	return 0;
}

UINT CDlgGame::Play_Server(LPVOID lParam)//服务器执行出牌线程
{
	CDlgGame* pWnd = (CDlgGame*)lParam;       //将窗口指针赋给无类型指针  
	pWnd->Play_Server();

	return 1;
}

void CDlgGame::Play_Server()//服务器出牌线程
{
	if (!(g_pGame->PlayNum == 0 && g_pGame->Round == 0))
		if (m_Check_Sound.GetCheck() == FALSE)
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_PLAYTIP), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);

	JudgeRange();//根据temp1判断出牌范围更新按钮可用

	m_Static_Tip.SetWindowText(_T("请选择要出的牌"));

	g_second = 20;//开始计时

	while (g_second > -1)
	{
		if(g_second==4)//剩余五秒播放音效
			if (m_Check_Sound.GetCheck() == FALSE)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_COUNT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);

		SendMessage(WM_UPDATETIME, 0, 0);
		Sleep(999);
		g_second--;
	}

	if (g_second == -1)//如果时间到了，系统选择出最靠右的牌
		temp = temp2;

	int myPlay = g_pGame->myCard[temp].data;
	g_pGame->myCard[temp].data = 0;

	if (g_pGame->Round == 0 && g_pGame->PlayNum == 0)
	{
		g_pGame->CurrentOrder = g_order;

		string infor("O" + to_string(g_order) + "#");
		//改变g_pGame->CurrentOrder的信号：‘O’+order+'#'
		for(int i=1;i<4;i++)
			if(g_pUser[i])
				g_pServer->Send_Message(infor, g_pUser[i]->receiver);
	}

	int j;
	if (g_pGame->CurrentOrder <= g_order)
		j = g_order - g_pGame->CurrentOrder;
	else
		j = g_order - g_pGame->CurrentOrder + 4;
	g_pGame->Save[j] = myPlay;

	if (m_Check_Sound.GetCheck() == FALSE)
	{
		if (myPlay == 11)//出牌声音
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_SHEEP), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		else
		{
			if(myPlay == 23)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_TRANS), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			else
			{
				if(myPlay == 51)
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_PIG), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				else
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_PLAY), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
		} 
	}

	if (j == 0)//保留第一名出牌玩家的花色
		temp1 = g_pGame->JudgeDecor(myPlay);

	BanCardButton();//禁用所有出牌按钮

	m_Static_Tip.SetWindowText(_T("请等待其他玩家出牌"));

	ChangeButtonBitmap(-1, myPlay);//展示出牌的图案

	ChangeButtonBitmap(temp, 0);//把出过的牌图案变成空白

	string strtemp = "P1#" + to_string(myPlay) + "#"; //出牌信号数据格式：'P' + order + '#' + cardint + '#'
	
	for (int i = 1; i < 4; i++)//广播消息
	{
		g_pServer->Send_Message(strtemp, g_pUser[i]->receiver);
	}
	g_pGame->PlayNum++;

	if (g_pGame->PlayNum == 4)//回合结算
		SendMessage(WM_ROUNDSETTLE, 0, 0);
}

//根据temp1判断出牌范围并更新按钮可用
void CDlgGame::JudgeRange()//-1只能出梅花2  0可出任意花色  1方片 2梅花 3红桃 4黑桃
{
	if (temp1 == -1)//只能出梅花2
	{
		for (int i = 0; i < 13; i++)
		{
			if (g_pGame->myCard[i].data == 15)
			{
				if (g_Model == 0)
					m_Button_Card[i].EnableWindow(TRUE);

				temp2 = i;
			}
				
		}
		return;
	}

	if (temp1 == 0)//可出任意花色
	{
		for (int i = 0; i < 13; i++)
		{
			if (g_pGame->myCard[i].data)
			{
				if (g_Model == 0)
					m_Button_Card[i].EnableWindow(TRUE);
				temp2 = i;
			}
				
		}
	}

	else//1方片 2梅花 3红桃 4黑桃
	{
		int max=0, min=0,Bsamedecor = 1;
		switch (temp1)
		{
		case 1:max = 13; min = 1; break;
		case 2:max = 26; min = 14; break;
		case 3:max = 39; min = 27; break;
		case 4:max = 52; min = 40;
		}
		for (int i = 0; i < 13; i++)
		{
			if (g_pGame->myCard[i].data >= min && g_pGame->myCard[i].data <= max)
			{
				if (g_Model == 0)
					m_Button_Card[i].EnableWindow(TRUE);

				temp2 = i;
				Bsamedecor = 0;
			}
				
		}
		if(Bsamedecor)//如果没有同花色的手牌，可以出任意牌
			for (int i = 0; i < 13; i++)
			{
				if (g_pGame->myCard[i].data)
				{
					if (g_Model == 0)
						m_Button_Card[i].EnableWindow(TRUE);

					temp2 = i;
				}
					
			}
	}
}	


void CDlgGame::BanCardButton()//禁用所有出牌按钮
{
	for (int i = 0; i < 13; i++)
		m_Button_Card[i].EnableWindow(FALSE);
}

UINT CDlgGame::Play_Client(LPVOID lParam)//客户端执行出牌线程
{
	CDlgGame* pWnd = (CDlgGame*)lParam;       //将窗口指针赋给无类型指针  
	pWnd->Play_Client();

	return 1;
}

void CDlgGame::Play_Client()//客户端出牌线程
{
	if(!(g_pGame->PlayNum==0&& g_pGame->Round == 0))
		if (m_Check_Sound.GetCheck() == FALSE)
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_PLAYTIP), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);


	JudgeRange();//根据temp1判断出牌范围更新按钮可用

	m_Static_Tip.SetWindowText(_T("请选择要出的牌"));

	if (g_Model == 0)//开始计时
		g_second = 20;
	else
		g_second = 3;

	while (g_second > -1)
	{
		if (g_second == 4)//剩余五秒播放音效
			if (m_Check_Sound.GetCheck() == FALSE)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_COUNT), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);

		SendMessage(WM_UPDATETIME, 0, 0);
		Sleep(999);
		g_second--;
	}

	if (g_second == -1)//如果时间到了，系统选择出最靠右的牌
		temp = temp2;

	if (g_Model >= 2)
		temp = g_pCom->Play();

	int myPlay = g_pGame->myCard[temp].data;
	g_pGame->myCard[temp].data = 0;

	if (g_pGame->Round == 0 && g_pGame->PlayNum==0)
	{
		g_pGame->CurrentOrder = g_order;
		//改变g_pGame->CurrentOrder的信号：‘O’+order+'#'
		g_pClient->Send_Message(string("O" + to_string(g_order) + "#"));
	}

	int j;
	if (g_pGame->CurrentOrder <= g_order)
		j = g_order - g_pGame->CurrentOrder;
	else
		j = g_order - g_pGame->CurrentOrder + 4;
	g_pGame->Save[j] = myPlay;

	if (m_Check_Sound.GetCheck() == FALSE)
	{
		if (myPlay == 11)//出牌声音
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_SHEEP), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		else
		{
			if (myPlay == 23)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_TRANS), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			else
			{
				if (myPlay == 51)
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_PIG), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				else
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_PLAY), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
		}
	}

	if (j == 0)//保留第一名出牌玩家的花色
		temp1 = g_pGame->JudgeDecor(myPlay);

	BanCardButton();//禁用所有出牌按钮

	m_Static_Tip.SetWindowText(_T("请等待其他玩家出牌"));

	ChangeButtonBitmap(-1, myPlay);//展示出牌的图案

	ChangeButtonBitmap(temp, 0);//把出过的牌图案变成空白

	string strtemp = "P" + to_string(g_order) + "#" + to_string(myPlay) + "#"; //出牌信号数据格式：'P' + order + '#' + cardint + '#'

	g_pClient->Send_Message(strtemp);

	g_pGame->PlayNum++;

	if (g_pGame->PlayNum == 4)//回合结算
		SendMessage(WM_ROUNDSETTLE, 0, 0);
}

afx_msg LRESULT CDlgGame::OnDealpmsg_Cli(WPARAM wParam, LPARAM lParam)//客户端处理出牌信息 出牌信号：'P'+order+'#'+cardint+'#'
{
	int order = atoi((string(g_pClient->recv_buf).substr(1, 1)).c_str());//读取信息
	int card = atoi((string(g_pClient->recv_buf).substr(3)).c_str());

	int j;//把出牌保存在save里
	if (g_pGame->CurrentOrder <= order)
		j = order - g_pGame->CurrentOrder;
	else
		j = order - g_pGame->CurrentOrder + 4;
	g_pGame->Save[j] = card;

	if (j == 0)
		temp1 = g_pGame->JudgeDecor(card);//保留第一名出牌玩家的花色

	if(g_Model>=2)
		g_pCom->AcceptPlayMsg(order, card);//接收出牌信号

	if (m_Check_Sound.GetCheck() == FALSE)
	{ 
		if (card == 11)//出牌声音
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_SHEEP), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		else
		{
			if (card == 23)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_TRANS), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			else
			{
				if (card == 51)
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_PIG), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				else
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_PLAY), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
		}
	}

	if (g_order <= order)//展示出牌
		j = order - g_order;
	else
		j = order - g_order + 4;
	ChangeButtonBitmap(-j - 1, card);

	g_pGame->PlayNum++;

	return 0;
}


afx_msg LRESULT CDlgGame::OnDealpmsg_Ser(WPARAM wParam, LPARAM lParam)//服务器处理出牌信息
{
	int order = atoi((string(g_pServer->recv_buf).substr(1, 1)).c_str());//读取信息
	int card = atoi((string(g_pServer->recv_buf).substr(3)).c_str());

	int j;//把出牌保存在save里
	if (g_pGame->CurrentOrder <= order)
		j = order - g_pGame->CurrentOrder;
	else
		j = order - g_pGame->CurrentOrder + 4;
	g_pGame->Save[j] = card;

	if (j == 0)
		temp1 = g_pGame->JudgeDecor(card);//保留第一名出牌玩家的花色

	if (m_Check_Sound.GetCheck() == FALSE)
	{ 
		if (card == 11)//出牌声音
			PlaySound(MAKEINTRESOURCE(IDR_WAVE_SHEEP), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		else
		{
			if (card == 23)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_TRANS), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			else
			{
				if (card == 51)
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_PIG), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
				else
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_PLAY), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
		}
	}

	//展示出牌
	ChangeButtonBitmap(-order , card);

	g_pGame->PlayNum++;


	return 0;
}



afx_msg LRESULT CDlgGame::OnPlaycli(WPARAM wParam, LPARAM lParam)
{
	AfxBeginThread(Play_Client, (LPVOID)this);//创建开始出牌线程
	return 0;
}


afx_msg LRESULT CDlgGame::OnPlayser(WPARAM wParam, LPARAM lParam)
{
	AfxBeginThread(Play_Server, (LPVOID)this);//创建开始出牌线程
	return 0;
}



afx_msg LRESULT CDlgGame::OnRoundsettle(WPARAM wParam, LPARAM lParam)//回合结算
{
	g_pGame->CurrentOrder = g_pGame->JudgeLargest();//判断下回合谁出牌

	switch (g_pGame->CurrentOrder)
	{
	case 1:m_Static_Tip.SetWindowText(_T("本回合北座玩家牌最大")); break;
	case 2:m_Static_Tip.SetWindowText(_T("本回合西座玩家牌最大")); break;
	case 3:m_Static_Tip.SetWindowText(_T("本回合南座玩家牌最大")); break;
	case 4:m_Static_Tip.SetWindowText(_T("本回合东座玩家牌最大"));
	}
	
	for (int i = 0; i < 4; i++)
	{
		
		if ((g_pGame->Save[i] >= 27 && g_pGame->Save[i] <= 39) || g_pGame->Save[i] == 11 || g_pGame->Save[i] == 23 || g_pGame->Save[i] == 51)//判断是否加入特征牌库
		{
			for (int j = 0; j < 16; j++)
			{
				if (g_pGame->Collection[g_pGame->CurrentOrder - 1][j] == 0)//寻找特征数组中第一个空位
				{
					g_pGame->Collection[g_pGame->CurrentOrder - 1][j] = g_pGame->Save[i];
					break;
				}
			}
			g_pGame->PlayedCollection++;
			SetCollectionMsg(g_pGame->Save[i], g_pGame->CurrentOrder);//给特征牌静态框添加获得特征牌的信息

		}
	}

	g_pGame->ArrangeCard();//整理myCard
	SendMessage(WM_UPDATEMYCARD, 0, 0);//根据myCard更新位图

	for (int i = 0; i < 4; i++)
		g_pGame->Save[i] = 0;

	UpdateScore(g_pGame->CurrentOrder);//更新分数

	g_second = 5;
	while (g_second > -1)
	{
		Sleep(999);
		g_second--;
	}
	//Sleep(5999);//停顿几秒

	for (int i = -1; i > -5; i--)
		ChangeButtonBitmap(i, 0);//清除展示按钮的位图

	m_Static_Tip.SetWindowText(_T("请等待其他玩家出牌"));

	g_pGame->Round++;

	g_pGame->PlayNum = 0;

	if (g_pGame->Round != 13 && g_pGame->PlayedCollection != 16)//测试，记得改回13
	{
		if (g_order == g_pGame->CurrentOrder)
		{
			temp1 = 0;//首个玩家可任意出牌

			if(g_pServer)
				AfxBeginThread(Play_Server, (LPVOID)this);//创建服务器开始出牌线程
			else
				AfxBeginThread(Play_Client, (LPVOID)this);//创建客户端开始出牌线程

		}
	}
	else//一局游戏的结算
	{
		SendMessage(WM_ENDOFGAME, 0, 0);//开始游戏结算
	}

	return 0;
}


void CDlgGame::SetCollectionMsg(int card, int order)//给特征牌静态框添加获得特征牌的信息
{
	switch (card)
	{
	case 11:SetMessage("方片J", order); break;
	case 23:SetMessage("梅花10", order); break;
	case 51:SetMessage("黑桃Q", order); break;
	case 27:SetMessage("红桃A", order); break;
	case 28:SetMessage("红桃2", order); break;
	case 29:SetMessage("红桃3", order); break;
	case 30:SetMessage("红桃4", order); break;
	case 31:SetMessage("红桃5", order); break;
	case 32:SetMessage("红桃6", order); break;
	case 33:SetMessage("红桃7", order); break;
	case 34:SetMessage("红桃8", order); break;
	case 35:SetMessage("红桃9", order); break;
	case 36:SetMessage("红桃10", order); break;
	case 37:SetMessage("红桃J", order); break;
	case 38:SetMessage("红桃Q", order); break;
	case 39:SetMessage("红桃K", order);
	}
}

void CDlgGame::UpdateScore(int order)//根据Collection[order-1]更新分数
{
	int score = 0;
	int x = 1;//特征牌分数倍率
	for (int i = 0; i < 16; i++)
	{
		if (g_pGame->Collection[order - 1][i] == 0)
			break;
		if (g_pGame->Collection[order - 1][i] == 23)//特征牌中含有变压器分值翻倍
		{
			if (g_pGame->BTrans)
				x = 4;
			else
				x = 2;
			break;
		}
	}

	int CollectionNum = 0, HeartNum = 0;//记录特征牌数量和红桃数量
	for (int i = 0; i < 16; i++)
	{
		if (g_pGame->Collection[order - 1][i] == 0)
			break;
		if (g_pGame->Collection[order - 1][i] >= 27 && g_pGame->Collection[order - 1][i] <= 39) //红桃
		{
			CollectionNum++;
			HeartNum++;
			continue;
		}
		if (g_pGame->Collection[order - 1][i] == 11|| g_pGame->Collection[order - 1][i] == 51|| g_pGame->Collection[order - 1][i] == 23) //方块11 梅花10 黑桃12
		{
			CollectionNum++;
			continue;
		}
	}
	if (CollectionNum == 16)//满贯
	{
		score = 400;
		if (g_pGame->BHeartA)
			score *= 2;
		if (g_pGame->BSheep)
			score *= 2;
		if (g_pGame->BPig)
			score *= 2;
		if (g_pGame->BTrans)
			score *= 4;
	}
	else
	{
		int IsAllHeart = 1;
		if (HeartNum == 13)//满红
			IsAllHeart = -1;

		for (int i = 0; i < 16; i++)
		{
			if (g_pGame->Collection[order - 1][i] == 0)
				break;
			if (g_pGame->Collection[order - 1][i] >= 31 && g_pGame->Collection[order - 1][i] <= 36) //红桃5-10
			{
				if (g_pGame->BHeartA)
					score -= 20 * x * IsAllHeart;
				else
					score -= 10 * x * IsAllHeart;

				continue;
			}
			if (g_pGame->Collection[order - 1][i] == 37) //红桃11
			{
				if (g_pGame->BHeartA)
					score -= 40 * x * IsAllHeart;
				else
					score -= 20 * x * IsAllHeart;

				continue;
			}
			if (g_pGame->Collection[order - 1][i] == 38) //红桃12
			{
				if (g_pGame->BHeartA)
					score -= 60 * x * IsAllHeart;
				else
					score -= 30 * x * IsAllHeart;

				continue;
			}
			if (g_pGame->Collection[order - 1][i] == 39) //红桃13
			{
				if (g_pGame->BHeartA)
					score -= 80 * x * IsAllHeart;
				else
					score -= 40 * x * IsAllHeart;

				continue;
			}
			if (g_pGame->Collection[order - 1][i] == 27) //红桃1
			{
				if (g_pGame->BHeartA)
					score -= 100 * x * IsAllHeart;
				else
					score -= 50 * x * IsAllHeart;

				continue;
			}
			if (g_pGame->Collection[order - 1][i] == 11) //方块11
			{
				if (g_pGame->BSheep)
					score += 200 * x;
				else
					score += 100 * x;

				continue;
			}
			if (g_pGame->Collection[order - 1][i] == 51) //黑桃12
			{
				if (g_pGame->BPig)
					score -= 200 * x;
				else
					score -= 100 * x;

				continue;
			}
		}
	}
	
	int k=1;//保存分数  更新控件显示
	if (order >= g_order)
		k += order - g_order;
	else
		k += order - g_order + 4;
	switch (k)
	{
	case 1:m_iScore_Down = score; break;
	case 2:m_iScore_Right = score; break;
	case 3:m_iScore_Up = score; break;
	case 4:m_iScore_Left = score;
	}
	UpdateData(FALSE);
}

void CDlgGame::RecardScore()//保存对局到到文件"Recard.txt"
{
	ofstream o("Recard.txt", ios::app);

	time_t now = time(0);//现在时间
	tm* t_tm = localtime(&now);
	o << asctime(t_tm) << endl;

	for (int order = 1; order <= 4; order++)
	{
		o << "ID：" << g_pUser[order - 1]->ID << "    " << ZUOCI(order) << "    " << "Score：";

		int j = (order >= g_order ? (order - g_order + 1) : (order - g_order + 5));
		switch (j)
		{
		case 1:o << m_iScore_Down; break;
		case 2:o << m_iScore_Right; break;
		case 3:o << m_iScore_Up; break;
		case 4:o << m_iScore_Left;
		}
		o << "   " << "GainGold：" << GainGold(order) << endl;
	}
	o << endl << "----------------------------------------------------------" << endl;
	o.close();
}

int CDlgGame::GainGold(int order)//返回玩家获得的金币
{
	int TotalScore = m_iScore_Down + m_iScore_Left + m_iScore_Right + m_iScore_Up;
	int TeamScore;
	if ((abs(g_order - order)) % 2 == 1)
		TeamScore = m_iScore_Left + m_iScore_Right;
	else
		TeamScore = m_iScore_Down + m_iScore_Up;

	return (TeamScore / 2 - TotalScore / 4) * 5 + 50;

}

void CDlgGame::SaveData()//存ID和Gold到"Sav.dat"
{
	int IDtemp = 0;

	fstream mystream("Sav.dat", ios::in);//判断文件是否存在
	if (mystream.good())//文件存在
	{
		mystream.close();//重新打开
		mystream.open("Sav.dat", ios::in | ios::binary);

		//把文件保存在str里
		char chartemp;
		ostringstream buf;
		while (buf && mystream.get(chartemp))
			buf.put(chartemp);
		string str = buf.str();

		mystream.close();

		for (int i = 0; i < 4; i++)
		{
			if (!g_pUser[i])
				continue;

			const char* s = str.c_str();

			int j;
			for (j = 0; j <= str.length(); j += 2 * sizeof(int))
			{
				IDtemp = *(int*)(s + j);
				if (IDtemp == g_pUser[i]->ID)//找到相应位置
				{
					char* p = &str[j];
					*(int*)(p + sizeof(int)) = g_pUser[i]->gold;//覆盖gold
					break;
				}

			}

			if (j > str.length())//文件中找不到ID
			{
				char chartemp[9] = { 0 };

				for (int k = 0; k < 4; k++)
				{
					chartemp[k] = *(((char*)&g_pUser[i]->ID) + k);
				}
				for (int k = 0; k < 4; k++)
				{
					chartemp[k + 4] = *(((char*)&g_pUser[i]->gold) + k);
				}
				str += "        ";//8个空格
				for (int k = 0; k < 8; k++)
					str[j - 8 + k] = chartemp[k];
			}

		}

		mystream.open("Sav.dat", ios::out | ios::binary);//新开存档文件保存str
		char* p = &str[0];
		mystream.write(p, str.length());
		mystream.close();
	}
	else//文件不存在
	{
		mystream.clear();//清除异常重新打开
		mystream.open("Sav.dat", ios::out | ios::binary);

		for (int i = 0; i < 4; i++)
		{
			if (g_pUser[i])
			{
				mystream.write((char*)&g_pUser[i]->ID, sizeof(int));
				mystream.write((char*)&g_pUser[i]->gold, sizeof(int));
			}
		}
	}
}


afx_msg LRESULT CDlgGame::OnEndofgame(WPARAM wParam, LPARAM lParam)//游戏结算
{
	if (g_pCom)
	{ 
		delete g_pCom;
		g_pCom = 0;
	}

	m_Static_Tip.SetWindowText(_T("请等待主机开始新一局游戏"));
	string data;//把得分数据保存在data里

	if (GetScore(1) + GetScore(3) > GetScore(2) + GetScore(4))//判定胜负
	{
		if (g_order == 1 || g_order == 3)
		{
			data += "You Win !!!    (～￣▽￣)～ \n\n";

			if (m_Check_Sound.GetCheck() == FALSE)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		}	
		else
		{
			data += "You Lose !!!     (Ｔ～Ｔ)\n\n";

			if (m_Check_Sound.GetCheck() == FALSE)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_LOSE), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		}
	}
	else
	{
		if (GetScore(1) + GetScore(3) < GetScore(2) + GetScore(4))
		{
			if (g_order == 1 || g_order == 3)
			{
				data += "You Lose !!!     (Ｔ～Ｔ)\n\n";

				if (m_Check_Sound.GetCheck() == FALSE)
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_LOSE), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
			else
			{
				data += "You Win !!!    (～￣▽￣)～ \n\n";

				if (m_Check_Sound.GetCheck() == FALSE)
					PlaySound(MAKEINTRESOURCE(IDR_WAVE_WIN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
			}
		}
		else//平手
		{
			data += "Draw !!!      (￣ω￣;)\n\n";

			if (m_Check_Sound.GetCheck() == FALSE)
				PlaySound(MAKEINTRESOURCE(IDR_WAVE_LOSE), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
		}
			
	}

	data += "Score（南和北）：" + to_string(GetScore(1) + GetScore(3)) + "\n";
	data += "Score（东和西）：" + to_string(GetScore(2) + GetScore(4)) + "\n\n";
	for (int i = 0; i < 4; i++)
	{
		if (i + 1 == g_order)
			data += " ↓  YOU\n";
		else
			data += "\n";
		data += "ID：" + to_string((g_pUser[i] ? g_pUser[i]->ID : -1)) + "   " + ZUOCI(i + 1) + "   " + "Score：" + to_string(GetScore(i + 1)) + "   " + "GetGold：" + to_string(GainGold(i + 1)) + "\n";
	}

	for(int i = 0;i < 4; i++)
	{ 
		for (int j = 0; j < 16; j++)
			if (g_pGame->Collection[i][j] == 51)//找到猪，决定下一句谁先出牌
			{
				g_pGame->CurrentOrder = i + 1;
				goto L2;
			}
	}		
L2:	
	for (int i = 0; i < 4; i++)//更新gold
	{
		if (g_pUser[i])
			g_pUser[i]->gold += GainGold(i + 1);
	}
	m_iGold_Down = (g_pUser[g_order - 1] ? g_pUser[g_order - 1]->gold : 1000);
	m_iGold_Right = (g_pUser[g_order % 4] ? g_pUser[g_order % 4]->gold : 1000);
	m_iGold_Up = (g_pUser[(g_order + 1) % 4] ? g_pUser[(g_order + 1) % 4]->gold : 1000);
	m_iGold_Left = (g_pUser[(g_order + 2) % 4] ? g_pUser[(g_order + 2) % 4]->gold : 1000);
	UpdateData(FALSE);
	
	if (g_pServer)
	{
		RecardScore();//服务器保存对局到到文件"Recard.txt"
		SaveData();//存ID和Gold到"Sav.dat"

		m_Button_Restart.EnableWindow(TRUE);//主机端可用重开游戏按钮
	}


	CString cstrtemp;//弹出得分对话框
	cstrtemp = data.c_str();
	if (g_Model == 0)
		MessageBox(cstrtemp, _T("本局得分"), MB_OK);


	return 0;
}


int CDlgGame::GetScore(int order)//返回玩家的分数
{
	int i = 0;
	if (order >= g_order)
		i = order - g_order + 1;
	else
		i = order - g_order + 5;

	switch (i)
	{
	case 1:return m_iScore_Down;
	case 2:return m_iScore_Right;
	case 3:return m_iScore_Up;
	default:return m_iScore_Left;
	}
}


void CDlgGame::OnClickedButtonRestart()//开始新一局游戏
{
	for (int i = 1; i < 4; i++)//服务器发出重开信号：“Re”
	{
		g_pServer->Send_Message("Re", g_pUser[i]->receiver);
	}

	SendMessage(WM_RESTART, 0, 0);
}


afx_msg LRESULT CDlgGame::OnRestart(WPARAM wParam, LPARAM lParam)//重新开始游戏
{
	g_state--;//发牌阶段

	m_iScore_Down = 0;//初始化控件关联变量
	m_iScore_Left = 0;
	m_iScore_Right = 0;
	m_iScore_Up = 0;

	m_Check_Sheep.SetCheck(FALSE);
	m_Check_Trans.SetCheck(FALSE);
	m_Check_HeartA.SetCheck(FALSE);
	m_Check_Pig.SetCheck(FALSE);

	char strtemp[1] = { 0 };
	m_szStatic_Collection_Up = (const char*)strtemp;
	m_szStatic_Collection_Down = (const char*)strtemp;
	m_szStatic_Collection_Left = (const char*)strtemp;
	m_szStatic_Collection_Right = (const char*)strtemp;

	m_Button_Restart.EnableWindow(FALSE);

	for (int i = 0; i < 4; i++)//初始化game类
		for (int j = 0; j < 16; j++)
			g_pGame->Collection[i][j] = 0;

	g_pGame->BHeartA = g_pGame->BPig = g_pGame->BSheep = g_pGame->BTrans = FALSE;

	g_pGame->PlayNum = 0;
	g_pGame->Round = 0;
	g_pGame->PlayedCollection = 0;

	UpdateData(FALSE);

	for (int i = 1; i < 5; i++)
		SetMessage(" ", i);

	if (g_Model != 0)
		g_pCom = new Com;

	m_Static_Tip.SetWindowText(_T("准备开始游戏"));

	if (g_order == 1)//主机端
	{
		g_pGame->Deal();//发牌
		SendMessage(WM_UPDATEMYCARD, 0, 0);//根据myCardg更新按钮位图和可用

		AfxBeginThread(ThreadShowDown_Server, (LPVOID)this);//创建亮牌线程
	}

	return 0;
}



void CDlgGame::OnClickedCheckSound()
{
	PlaySound(0, 0, SND_ASYNC);//按下静音键停止播放
}
