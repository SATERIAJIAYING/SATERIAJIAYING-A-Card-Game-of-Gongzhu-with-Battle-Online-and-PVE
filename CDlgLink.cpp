// CDlgLink.cpp: 实现文件
//

#include "pch.h"
#include "CDlgLink.h"
#include "net.h"
#include "gongzhuDlg.h"
#include "gongzhu.h"
#include "afxdialogex.h"
#include "CDlgGame.h"


//全局变量定义  
int g_state = 1;//1连接阶段 2发牌阶段
int g_user_num = 1;
user* g_pUser[4] = { 0 };//下标为order-1
int temp=0,temp1=0, temp2=0;
server* g_pServer =0 ;
client* g_pClient =0 ;
int g_c_port;
CDlgGame* g_pCDlgGame = 0;
int g_Model = 0;

// CDlgLink 对话框

IMPLEMENT_DYNAMIC(CDlgLink, CDialogEx)

CDlgLink::CDlgLink(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1_LINK, pParent)
{

}

CDlgLink::~CDlgLink()
{

}

void CDlgLink::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_STATIC_LINK, m_Static_Link);
	DDX_Control(pDX, IDC_LIST3_LINK, m_List_Link);
	DDX_Control(pDX, IDC_BUTTON_ORDER2, m_Button_Order2);
	DDX_Control(pDX, IDC_BUTTON_ORDER3, m_Button_Order3);
	DDX_Control(pDX, IDC_BUTTON_ORDER4, m_Button_Order4);
	DDX_Control(pDX, IDC_BUTTON1_STARTGAME, m_Button_Startgame);
	DDX_Control(pDX, IDC_CHECK_COM, m_Check_Com);
	DDX_Control(pDX, IDC_COMBO_DEGREE, m_Combo_Degree);
}


BEGIN_MESSAGE_MAP(CDlgLink, CDialogEx)
	ON_MESSAGE(WM_CLIENTLOGIN, &CDlgLink::OnClientlogin)
	ON_MESSAGE(WM_CLOSEBUTTON, &CDlgLink::OnClosebutton)
	ON_MESSAGE(WM_BROADCASTID, &CDlgLink::OnBroadcastid)
	ON_MESSAGE(WM_BROADCASTORDER, &CDlgLink::OnBroadcastorder)
	ON_MESSAGE(WM_UPDATALIST, &CDlgLink::OnUpdatalist)
	ON_BN_CLICKED(IDC_BUTTON_ORDER2, &CDlgLink::OnClickedButtonOrder2)
	ON_BN_CLICKED(IDC_BUTTON_ORDER3, &CDlgLink::OnClickedButtonOrder3)
	ON_BN_CLICKED(IDC_BUTTON_ORDER4, &CDlgLink::OnClickedButtonOrder4)
	ON_MESSAGE(WM_STARTEXORDER, &CDlgLink::OnStartexorder)
	ON_BN_CLICKED(IDC_BUTTON1_STARTGAME, &CDlgLink::OnClickedButton1Startgame)
	ON_MESSAGE(WM_STARTGAME, &CDlgLink::OnStartgame)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_GETIP, &CDlgLink::OnClickedButtonGetip)
	ON_BN_CLICKED(IDC_CHECK_COM, &CDlgLink::OnClickedCheckCom)
	ON_CBN_EDITUPDATE(IDC_COMBO_DEGREE, &CDlgLink::OnEditupdateCombo)
END_MESSAGE_MAP()


// CDlgLink 消息处理程序


BOOL CDlgLink::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->ModifyStyleEx(0, WS_EX_APPWINDOW);
	srand((unsigned)time(NULL));

	// m_List_Link 风格
	m_List_Link.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES); // 整行选择、网格线
	m_List_Link.InsertColumn(0, _T(""), LVCFMT_RIGHT, 20);//第0列是虚列
	m_List_Link.InsertColumn(1, _T("玩家ID"), LVCFMT_RIGHT, 90);
	m_List_Link.InsertColumn(2, _T("剩余金币"), LVCFMT_RIGHT, 80);
	m_List_Link.InsertColumn(3, _T("座次"), LVCFMT_RIGHT, 50);

	if (g_order)//作为主机
	{
		this->SetWindowText(_T("连接游戏（主机端）"));
		//创建本机user对象
		g_pUser[0] = new user(g_id,GetGold(g_id));
		//列表中插入本机玩家行
		InsertPlayer(0, g_id, 1);//主机玩家默认坐北端

		//禁用调整座次
		m_Button_Order2.EnableWindow(FALSE);
		m_Button_Order3.EnableWindow(FALSE);
		m_Button_Order4.EnableWindow(FALSE);

		m_Button_Startgame.EnableWindow(FALSE);//禁用开始游戏按钮
		m_Check_Com.EnableWindow(FALSE);//禁用机器人复选框
		m_Combo_Degree.EnableWindow(FALSE);//禁用困难复选框

		//创建线程监听
		AfxBeginThread(ThreadServer, (LPVOID)this);
	}


	else//作为客户端登录
	{
		this->SetWindowText(_T("连接游戏（客户端）"));
		m_Button_Startgame.EnableWindow(FALSE);//禁用开始游戏按钮
		m_Button_Startgame.SetWindowText(L"等待服务器开始游戏");
		GetDlgItem(IDC_BUTTON_GETIP)->EnableWindow(FALSE);//禁用查询IP按钮
		m_Combo_Degree.EnableWindow(FALSE);//禁用困难复选框
		m_Combo_Degree.SetCurSel(0);//默认是简单难度
		
		AfxBeginThread(ThreadClientRecv , (LPVOID)this);//创建线程接受监听

	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgLink::InsertPlayer(int i, int PlayerID, int order)//在列表中插入玩家 i行数，从0开始
{

	CString temp;
	temp.Format(_T("%d"), PlayerID);
	if( PlayerID == g_id)
		m_List_Link.InsertItem(i, _T("#"));//给本机标上#
	else
		m_List_Link.InsertItem(i, _T(""));
	m_List_Link.SetItemText(i, 1, temp);
	temp.Format(_T("%d"), g_pUser[order-1]->gold);
	m_List_Link.SetItemText(i, 2, temp);
	switch (order)
	{
	case 1:m_List_Link.SetItemText(i, 3, _T("北")); break;
	case 2:m_List_Link.SetItemText(i, 3, _T("西")); break;
	case 3:m_List_Link.SetItemText(i, 3, _T("南")); break;
	case 4:m_List_Link.SetItemText(i, 3, _T("东"));
	}
}

//	服务器线程执行函数
UINT CDlgLink::ThreadServer(LPVOID lParam)
{
	CDlgLink* pWnd = (CDlgLink*)lParam;       //将窗口指针赋给无类型指针  
	pWnd->RunServer();

	return 1;

}

void CDlgLink::RunServer()
{
	g_pServer = new server;

	while (1)//持续以下步骤
	{
		//初始化recv_buf
L1:		memset(g_pServer->recv_buf, 0, sizeof(g_pServer->recv_buf));

		//接受客户端发出的请求
		g_pServer->iRecv = recvfrom(g_pServer->sSocket, g_pServer->recv_buf, BUFFER_LENGTH, 0, (struct sockaddr*)&g_pServer->cli, &g_pServer->cli_length);
		if (g_pServer->iRecv == SOCKET_ERROR)
		{
			IsThreadEnd = true;
			return;
		}
			


		//获取发送方的地址
		std::string address(inet_ntoa(g_pServer->cli.sin_addr));// 网络地址转换成“.”点隔的字符串格式

		//数据保存在string内
		std::string infor(g_pServer->recv_buf);


		//g_state == 1 连接阶段
		if (g_state == 1)
		{
			if (infor[0] == 'L' )//服务器接收客户端的登录请求  数据格式：'L'+ID+'#'+端口+'#'
			{
				if (g_user_num >= 4)
				{
					g_pServer->Send_Message("C", g_pServer->cli);//如果人数已满 发送"C"回复
					continue;//下一轮循环开始
				}

				infor = infor.substr(1);
				int UserId = atoi(infor.c_str());//获取ID

				for (int i = 0; i < 4; i++)
				{
					if (g_pUser[i] && g_pUser[i]->ID == UserId)//如果ID重复 发送"A"回复
					{
						g_pServer->Send_Message("A", g_pServer->cli);
						goto L1;//下一轮循环开始
						
					}
				}


				unsigned int i;
				for (i = 0; i < infor.length(); i++)//找'#'
				{
					if (infor[i] == '#')
						break;
				}
				int receiver_port_int = atoi((infor.substr(i + 1)).c_str());//获取端口

				//添加用户
				for (int j = 1; j < 4; j++)
				{
					if (!g_pUser[j])
					{
						g_user_num++;
						g_pUser[j] = new user(UserId, GetGold(UserId));
						g_pUser[j]->receiver = g_pServer->cli;
						temp = j;

						if (g_user_num == 4)//人数满了就可用开始游戏按钮
							m_Button_Startgame.EnableWindow(TRUE);

						SendMessage(WM_UPDATALIST, 0, 0);//更新列表
						SendMessage(WM_BROADCASTID, 0, 0);//广播ID
						break;
					}
					if(j == 3)
						MessageBox(_T("玩家加入失败"),_T("错误7"));
				}
			}

			//接受客户端换座请求 数据格式：'E'+原座次+'#'+新座次+'#'
			if (infor[0] == 'E' )
			{
				DealEMsg(g_pServer->recv_buf);//处理换座信息 保存信息到pUser上

				//更新列表,并广播座次变换
				SendMessage(WM_UPDATALIST, 0, 0);

				//广播座次变换
				SendMessage(WM_BROADCASTORDER, 0, 0);

			}

			if (infor[0] == 'D')//接收客户端发送离开信号：‘D’+order+'#'
			{
				int order = atoi(infor.substr(1).c_str());

				for (int i = 1; i < 4; i++)//广播信号
				{
					if (g_pUser[i] && i != order - 1)
						g_pServer->Send_Message(infor, g_pUser[i]->receiver);
				}

				delete g_pUser[order - 1];//释放对应g_pUser内存，指针赋0
				g_pUser[order - 1] = 0;

				g_user_num--;

				m_Button_Startgame.EnableWindow(FALSE);//禁用开始游戏按钮

				SendMessage(WM_UPDATALIST, 0, 0);//更新列表
			}





		}

		if (g_state == 2)//发牌亮牌阶段
		{
	
			if (infor[0] == 'S')//处理亮牌信息
			{
				for (int i = 1; i < 4; i++)//向其他两个客户端广播接收到的消息
				{
					if (g_pUser[i] && (i + 1) != atoi((infor.substr(1, 1)).c_str()))
						g_pServer->Send_Message(infor, g_pUser[i]->receiver);
						
				}

				g_pCDlgGame->SendMessage(WM_DEALSMSG, 0, 0);

				if (atoi(((string(g_pServer->recv_buf)).substr(1, 1)).c_str()) == 4)//处理完第四名玩家的亮牌信息后
					g_pCDlgGame->SendMessage(WM_STARTSTATE3, 0, 0);
				
			}

			
		}
		if (g_state == 3)//出牌阶段
		{
			
			if (infor[0] == 'O')//改变g_pGame->CurrentOrder的信号：‘O’+order+'#'
			{
				g_pGame->CurrentOrder = atoi(infor.substr(1).c_str());
				for (int i = 1; i < 4; i++)//给其他客户端广播
				{ 
					if (g_pUser[i] && i != (g_pGame->CurrentOrder - 1))
						g_pServer->Send_Message(infor, g_pUser[i]->receiver);
		
				}
			}

			if (infor[0] == 'P')//处理出牌信息
			{
				for (int i = 1; i < 4; i++)//广播收到的信息
				{ 
					if (g_pUser[i] && (i + 1) != atoi(infor.substr(1, 1).c_str()))
						g_pServer->Send_Message(infor, g_pUser[i]->receiver);
					
				}
				g_pCDlgGame->SendMessage(WM_DEALPMSG_SER, 0, 0);//处理信息

				if (g_pGame->PlayNum == 4)//回合结算
				{
					g_pCDlgGame->PostMessage(WM_ROUNDSETTLE, 0, 0);
					continue;
				}

				int order = atoi(infor.substr(1, 1).c_str());
				if (g_order == order % 4 + 1)
				{
					g_pCDlgGame->SendMessage(WM_PLAYSER, 0, 0);
					continue;
				}

			}
		}
	}

}


//新用户登录，更新列表（主机端）
afx_msg LRESULT CDlgLink::OnClientlogin(WPARAM wParam, LPARAM lParam)
{
	InsertPlayer(g_user_num - 1, g_pUser[temp]->ID, g_pUser[temp]->gold);
	return 0;
}

//关闭切换座次按钮（客户端）
afx_msg LRESULT CDlgLink::OnClosebutton(WPARAM wParam, LPARAM lParam)
{
	m_Button_Order2.EnableWindow(FALSE);
	m_Button_Order3.EnableWindow(FALSE);
	m_Button_Order4.EnableWindow(FALSE);
	return 0;
}

//向所有客户端广播游戏玩家ID （主机端）
//数据格式：'B'+ID+'#'+gold+'#'+ID...按order顺序保存四名玩家数据，若有空则ID与gold为0
afx_msg LRESULT CDlgLink::OnBroadcastid(WPARAM wParam, LPARAM lParam)
{
	string message("B");
	char tempChar[11] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		if (g_pUser[i])
		{
			itoa(g_pUser[i]->ID, tempChar, 10);
			message += tempChar;
			message += '#';
			itoa(g_pUser[i]->gold, tempChar, 10);
			message += tempChar;
			message += '#';
		}
		else
		{
			itoa( 0, tempChar, 10);
			message += tempChar;
			message += '#';
			itoa( 0, tempChar, 10);
			message += tempChar;
			message += '#';
		}
	}

	for (int i = 1; i < 4; i++)
	{
		if (g_pUser[i])
		{
			g_pServer->Send_Message(message , g_pUser[i]->receiver);
		}
	}

	return 0;
}



//广播座次变换（主机端）
afx_msg LRESULT CDlgLink::OnBroadcastorder(WPARAM wParam, LPARAM lParam)
{
	string message(g_pServer->recv_buf);

	for (int i = 1; i < 4; i++)
	{
		if (g_pUser[i] && (i != temp1 - 1 ))
			g_pServer->Send_Message(message, g_pUser[i]->receiver);
	}

	return 0;
}

//根据g_pUser 更新列表信息（客户端和主机端通用）
afx_msg LRESULT CDlgLink::OnUpdatalist(WPARAM wParam, LPARAM lParam)
{
	int num = m_List_Link.GetItemCount();//清空列表
	for (int i = num - 1; i >= 0; i--)
	{
		m_List_Link.DeleteItem(i);
	}

	num = 0;
	for (int i = 0; i < 4; i++)//根据g_pUser插入行
	{
		if (g_pUser[i])
		{
			InsertPlayer(num, g_pUser[i]->ID, i + 1);
			num++;
		}
	}

	return 0;
}

UINT CDlgLink::ThreadClientRecv(LPVOID  lParam)//线程执行函数
{
	CDlgLink* pWnd = (CDlgLink*)lParam;       //将窗口指针赋给无类型指针  
	pWnd->RunClientRecv();      //要执行的函数 

	return 1;
}

void CDlgLink::RunClientRecv()//线程执行函数调用
{

	g_pClient = new client;

	//(登录请求)数据格式：'L'+ID+'#'+端口+'#'
	string infor_temp = "L" + to_string(g_id) + "#" + to_string(g_c_port) + "#";

	//发送登录请求
	g_pClient->Send_Message(infor_temp);

	//接受服务器回应
	g_pClient->iRecv = recvfrom(g_pClient->sClient, g_pClient->recv_buf, sizeof(g_pClient->recv_buf), 0, (SOCKADDR*)&g_pClient->ser, &g_pClient->ser_length);
	if (g_pClient->iRecv == SOCKET_ERROR)
	{
		MessageBox(_T("未收到服务器响应"), _T("错误10"));
		IsThreadEnd = true;
		SendMessage(WM_CLOSE, 0, 0);
		return;
	}

	//处理服务器回复列表更新
	//数据格式：'B'+ID+'#'+gold+'#'+ID...按order顺序保存四名玩家数据，若有空则ID与gold为0
	if (g_pClient->recv_buf[0] == 'B')
	{
		DealBMsg(g_pClient->recv_buf);//把数据内容保存到pUser
		IsLogon = true;
		goto L2;
	}

	if (g_pClient->recv_buf[0] == 'A')//数据格式："A"
	{
		MessageBox(_T("ID重复"), _T("错误"));
		IsThreadEnd = true;
		SendMessage(WM_CLOSE, 0, 0);
		return;
	}

	if (g_pClient->recv_buf[0] == 'C')//人数已满 "C"
	{
		MessageBox(_T("玩家人数已满"), _T("错误"));
		IsThreadEnd = true;
		SendMessage(WM_CLOSE, 0, 0);
		return;
	}

	else
	{
		MessageBox(_T("服务器响应信息错误"), _T("错误11"));
		IsThreadEnd = true;
		SendMessage(WM_CLOSE, 0, 0);
		return;
	}


L2:	while (1)//持续执行以下
	{
		//初始化recv_buf
		//memset(g_pClient->recv_buf, 0, sizeof(g_pClient->recv_buf));

		int iRecv = recvfrom(g_pClient->sClient, g_pClient->recv_buf, BUFFER_LENGTH, 0, (SOCKADDR*)&g_pClient->ser, &g_pClient->ser_length);
		DWORD kk = GetLastError();
		if (iRecv == SOCKET_ERROR)
		{ 
			IsThreadEnd = true;
			return;
		}

		else
		{
			if (g_state == 1)//游戏连接阶段
			{
				if (g_pClient->recv_buf[0] == 'B')//处理游戏成员更新的信息
				{
					DealBMsg(g_pClient->recv_buf);
				}


				if (g_pClient->recv_buf[0] == 'E')//处理换座信息
				{
					DealEMsg(g_pClient->recv_buf);//处理换座信息 保存信息到pUser上

				//更新列表,并广播座次变换
					SendMessage(WM_UPDATALIST, 0, 0);

				}

				if (g_pClient->recv_buf[0] == 'S' && g_pClient->recv_buf[1] == 't' && g_pClient->recv_buf[2] == 'a' && g_pClient->recv_buf[3] == 'r' && g_pClient->recv_buf[4] == 't')
				{
					g_state++;
					PostMessage(WM_STARTGAME, 0, 0);//开始游戏，打开新建模态对话框
					continue;
				}
					
				if (g_pClient->recv_buf[0] == 'D')//接收客户端发送离开信号：‘D’+order+'#'
				{
					int order = atoi(string(g_pClient->recv_buf).substr(1).c_str());

					if (order == 1)//主机端发送的消息
					{
						MessageBox(_T("服务器已关闭"), _T("连接结束"));
						OnClose();//关闭窗口
					}
					else//客户端发送的信息
					{
						delete g_pUser[order - 1];//释放对应g_pUser内存，指针赋0
						g_pUser[order - 1] = 0;

						SendMessage(WM_UPDATALIST, 0, 0);//更新列表
					}

				}

			}

			if (g_state == 2)//发牌阶段
			{
				while(!g_pGame){}//等待主线程中CDlgGame窗口初始化完成

				
				if (g_pClient->recv_buf[0] == '*')//处理发牌信息 数据格式：“*”+“牌编号”+'!'+“牌编号”+'!'+...+“!”
				{

					string infor(g_pClient->recv_buf);
					infor = infor.substr(1);

					for (int i = 0; i < 13; i++)
					{
						g_pGame->myCard[i].data = atoi(infor.c_str());

						if (i == 12)
							break;
						int j = 0;//找'!'
						for (j; j < 3; j++)
							if (infor[j] == '!')
								break;

						infor = infor.substr(j + 1);
					}
					g_pCDlgGame->SendMessage(WM_UPDATEMYCARD, 0, 0);//根据myCardg更新按钮位图和可用

					Sleep(4999);

					if (g_pCDlgGame->m_Check_Sound.GetCheck() == FALSE)
						PlaySound(MAKEINTRESOURCE(IDR_WAVE_SHOWDOWN), AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);//播放亮牌音乐

					g_pCDlgGame->GetDlgItem(IDC_STATIC_TIP)->SetWindowText(_T("请等待其他玩家选择亮牌"));

				}

				if (g_pClient->recv_buf[0] == 'S')//处理亮牌信息
				{
					g_pCDlgGame->SendMessage(WM_DEALSMSG, 0, 0);

					int inttemp = atoi(((string(g_pClient->recv_buf)).substr(1, 1)).c_str());
					if (inttemp == (g_order - 1))//依次亮牌
					{
						g_pCDlgGame->SendMessage(WM_CLISHOWDOWN, 0, 0);//开始亮牌
						
					}
					continue;
				}

				if (g_pClient->recv_buf[0] == 'P' && g_pClient->recv_buf[1] == 'L' && g_pClient->recv_buf[2] == 'A' && g_pClient->recv_buf[3] == 'Y')
				{
					g_pCDlgGame->SendMessage(WM_STATE3CLIENT, 0, 0);//进入出牌阶段
					continue;
				}
			}

			if (g_state == 3)//出牌阶段
			{

				if (g_pClient->recv_buf[0] == 'O')//改变g_pGame->CurrentOrder的信号：‘O’+order+'#'
				{
					g_pGame->CurrentOrder = atoi(string(g_pClient->recv_buf).substr(1).c_str());
				}

				if (g_pClient->recv_buf[0] == 'P')//接收出牌信息 出牌信号：'P'+order+'#'+cardint+'#'
				{
					g_pCDlgGame->SendMessage(WM_DEALPMSG_CLI, 0, 0);

					if (g_pGame->PlayNum == 4)//回合结算
					{
						g_pCDlgGame->PostMessage(WM_ROUNDSETTLE, 0, 0);
						continue;
					}

					int order = atoi(string(g_pClient->recv_buf).substr(1, 1).c_str());
					if (g_order == order % 4 + 1)
					{
						g_pCDlgGame->SendMessage(WM_PLAYCLI, 0, 0);
						continue;
					}
						
				}

				if (g_pClient->recv_buf[0] == 'R' && g_pClient->recv_buf[1] == 'e')//接收服务器发出重开信号：“Re”
				{
					g_pCDlgGame->SendMessage(WM_RESTART, 0, 0);
					continue; 
				}
			}

		}
	}

}

//处理玩家列表消息（客户端） 
//数据格式：'B'+ID+'#'+gold+'#'+ID...按order顺序保存四名玩家数据，若有空则ID与gold为0
void CDlgLink::DealBMsg(char* recv_)
{
	//先初始化pUser
	for (int i = 0; i < 4; i++)
	{
		delete g_pUser[i];
	}

	int id_temp = 0, gold_temp = 0;
	string infor( recv_ );
	infor = infor.substr(1);
	for (int i = 0; i < 4; i++)
	{
		id_temp = atoi(infor.c_str());//读取ID

		unsigned int j;//消除已读消息至'#'
		for (j = 0; j < infor.length(); j++)
			if (infor[j] == '#')
				break;
		infor = infor.substr(j + 1);

		gold_temp = atoi(infor.c_str());//读取gold

		if (i != 3)
		{
			for (j = 0; j < infor.length(); j++)//消除已读消息至'#'
				if (infor[j] == '#')
					break;
			infor = infor.substr(j + 1);
		}

		if (id_temp != 0)//根据信息初始化g_pUser
		{
			g_pUser[i] = new user(id_temp, gold_temp);
			//g_pUser[i]->gold = gold_temp;
		}

	}

	//根据g_pUser更新列表
	SendMessage( WM_UPDATALIST, 0, 0);
}

//数据格式：'E'+原座次+'#'+新座次+'#'
void CDlgLink::DealEMsg(char* recv_)//处理换座信息 更新到pUser中(主机和客户端通用)
{
	string infor(recv_);
	infor = infor.substr(1);
	temp = atoi(infor.c_str());//获取原座次存在temp

	unsigned int i;
	for (i = 0; i < infor.length(); i++)//找'#'
	{
		if (infor[i] == '#')
			break;
	}

	temp1 = atoi((infor.substr(i + 1)).c_str());//获取新座次存在temp1

	//更改g_pUser指向
	user* temg_pUser = g_pUser[temp - 1];
	g_pUser[temp - 1] = g_pUser[temp1 - 1];
	g_pUser[temp1 - 1] = temg_pUser;
}


void CDlgLink::OnClickedButtonOrder2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_pUser[1] &&( g_pUser[1]->ID == g_id))
	{
		MessageBox(_T("不能选择相同的座次"));
		return;
	}

	temp1 = 2;
	SendMessage(WM_STARTEXORDER, 0, 0);
}


void CDlgLink::OnClickedButtonOrder3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_pUser[2] && (g_pUser[2]->ID == g_id))
	{
		MessageBox(_T("不能选择相同的座次"));
		return;
	}

	temp1 = 3;
	SendMessage(WM_STARTEXORDER, 0, 0);
}


void CDlgLink::OnClickedButtonOrder4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_pUser[3] && (g_pUser[3]->ID == g_id))
	{
		MessageBox(_T("不能选择相同的座次"));
		return;
	}

	temp1 = 4;
	SendMessage(WM_STARTEXORDER, 0, 0);
}

//本机按下换座按钮 发送'E'+原order+'#'+新order+‘#’
afx_msg LRESULT CDlgLink::OnStartexorder(WPARAM wParam, LPARAM lParam)
{
	for (temp = 1; temp < 4; temp++)
	{
		if ( g_pUser[temp] && g_pUser[temp]->ID == g_id)
		{
			temp++;
			break;
		}
	}

	user* usertemp = g_pUser[temp - 1];//更改pUser指向
	g_pUser[temp - 1]= g_pUser[temp1 - 1];
	g_pUser[temp1 - 1] = usertemp;

	SendMessage(WM_UPDATALIST, 0, 0);//更新列表

	string message = "E";
	char chartemp[3] = { 0 };
	itoa(temp, chartemp, 10);
	message += chartemp;
	message += "#";
	itoa(temp1, chartemp, 10);
	message += chartemp;
	message += "#";

	g_pClient->Send_Message(message);//发送数据给主机

	return 0;
}


void CDlgLink::OnClickedButton1Startgame()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 1; i < 4; i++)
	{
		if (g_pUser[i])
			g_pServer->Send_Message("Start", g_pUser[i]->receiver);//向客户端发送开始游戏广播
	}

	g_state++;//state改为2（改变接收线程处理消息的方式）

	this->ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//隐藏窗口


	//创建新的模块对话框
	g_pCDlgGame = new CDlgGame;
	g_pCDlgGame->DoModal();
}



//开始游戏，打开模态对话框（客户端）
afx_msg LRESULT CDlgLink::OnStartgame(WPARAM wParam, LPARAM lParam)
{
	for ( g_order = 2; g_order <= 4; g_order++)//根据座次给g_order赋值
	{
		if (g_pUser[g_order - 1] && g_pUser[g_order - 1]->ID == g_id)
			break;
		if (g_order == 4)
		{
			::MessageBox(NULL, _T("初始化玩家座次失败"), _T("错误12"), MB_OK);
			return 0; 
		}
			
	}

	if (m_Check_Com.GetCheck() == TRUE)
	{
		g_Model = m_Combo_Degree.GetCurSel() + 1;
	}

	this->ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);//隐藏窗口

	//g_pCDlgGame = new CDlgGame;
	//g_pCDlgGame->DoModal();
	CDlgGame newdlg;
	g_pCDlgGame = &newdlg;
	g_pCDlgGame->DoModal();

	return 0;
}



void CDlgLink::OnClose()
{
	//客户端或者服务器发送离开信号：‘D’+order+'#'
	if (g_pServer)//如果是主机端
	{
		for(int i=1;i<4;i++)
			if(g_pUser[i])
				g_pServer->Send_Message("D1#",g_pUser[i]->receiver);
	}
	else//客户端
	{
		if(IsLogon)
		{ 
			int i;//定位自己的order=i+1
			for (i = 1; i < 4; i++)
			{
				if (g_pUser[i] && g_pUser[i]->ID == g_id)
					break;
			}

			g_pClient->Send_Message("D" + to_string(i + 1) + "#");
		}
		if (g_pCom)
			delete g_pCom;
	}

	for (int i = 0; i < 4; i++)
	{
		delete g_pUser[i];
	}

	if (g_pClient)
		closesocket(g_pClient->sClient);
	else
		closesocket(g_pServer->sSocket);

	while (IsThreadEnd == false) {}//等待线程终止

	WSACleanup();

	delete g_pClient;
	delete g_pServer;
	//delete g_pCDlgGame;

	CDialogEx::OnClose();

}


//数据：'S'+order+boolSheep+boolTrans+boolHeartA+boolPig  如"S31010"
void CDlgLink::DealSMsg(char* recv_)//处理亮牌信息
{
	string strtemp = recv_;
	int order = atoi((strtemp.substr(1, 1)).c_str());

	if (atoi((strtemp.substr(2, 1)).c_str()))
	{
		g_pGame->BSheep = TRUE;
		g_pCDlgGame->SetMessage("亮方块J", order);
	}
	if (atoi((strtemp.substr(3, 1)).c_str()))
	{
		g_pGame->BTrans = TRUE;
		g_pCDlgGame->SetMessage("亮梅花10", order);
	}
	if (atoi((strtemp.substr(4, 1)).c_str()))
	{
		g_pGame->BHeartA = TRUE;
		g_pCDlgGame->SetMessage("亮红桃A", order);
	}
	if (atoi((strtemp.substr(5, 1)).c_str()))
	{
		g_pGame->BPig = TRUE;
		g_pCDlgGame->SetMessage("亮黑桃Q", order);
	}
}

void CDlgLink::GetHostAddress(CString& strIPAddr)
{
	char HostName[100];
	gethostname(HostName, sizeof(HostName));// 获得本机主机名.

	hostent* hn;
	hn = gethostbyname(HostName);//根据本机主机名得到本机ip

	strIPAddr = inet_ntoa(*(struct in_addr*)hn->h_addr_list[0]);//把ip换成字符串形式
}

void CDlgLink::OnClickedButtonGetip()
{
	CString str1, str2, str3; 
	GetHostAddress(str1);
	str2 = "服务器的IP地址为：";
	str3 = "\n\n已复制到剪贴板";

	if (OpenClipboard())//复制到剪贴板
	{
		HGLOBAL clipbuffer;
		char* buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, str1.GetLength() + 1);
		buffer = (char*)GlobalLock(clipbuffer);
		string STDStr(CW2A(str1.GetString()));
		strcpy(buffer, STDStr.c_str());
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}

	MessageBox(str2 + str1 + str3, _T("服务器IP地址查询"), MB_OK);
}

afx_msg void CDlgLink::OnClickedCheckCom()
{
	if (m_Check_Com.GetCheck() == false)
		m_Combo_Degree.EnableWindow(FALSE);
	else
		m_Combo_Degree.EnableWindow(TRUE);


}


afx_msg void CDlgLink::OnEditupdateCombo()
{
	if (m_Combo_Degree.GetCurSel() < 0 || m_Combo_Degree.GetCurSel() >2)//下拉框编辑非法
	{
		MessageBox(_T("请从下拉菜单中选择其中一个难度"), _T("更改难度错误"));
		m_Combo_Degree.SetCurSel(0);
	}


}