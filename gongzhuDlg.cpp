
// gongzhuDlg.cpp: 实现文件
//

#include "pch.h"
#include "gongzhuDlg.h"
#include "gongzhu.h"
#include "CDlgLink.h"
#include "net.h"
#include "afxdialogex.h"
#include "Resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//全局变量定义
int g_id;
int g_order;//开始菜单1为主机，0为客户端
string g_ipAddr;
CDlgLink* g_pCDlgLink = 0;
CgongzhuDlg* g_pCgongzhuDlg = 0;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CgongzhuDlg 对话框



CgongzhuDlg::CgongzhuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GONGZHU_DIALOG, pParent)
	, m_iEdit_Start(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	
}

void CgongzhuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1_START, m_Edit_Start);
	DDX_Text(pDX, IDC_EDIT1_START, m_iEdit_Start);
	DDV_MinMaxInt(pDX, m_iEdit_Start, 1, INT_MAX);
	DDX_Control(pDX, IDC_CHECK1_START, m_Check_Start);
	DDX_Control(pDX, IDC_EDIT1_IP, m_Edit_Ip);
	DDX_Text(pDX, IDC_EDIT1_IP, m_szEdit_Ip);
	DDX_Control(pDX, IDC_BUTTON_RULE, m_Button_Rule);
	DDX_Control(pDX, IDC_BUTTON_PASTE, m_Button_Paste);
}

BEGIN_MESSAGE_MAP(CgongzhuDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CgongzhuDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1_START, &CgongzhuDlg::OnClickedCheck1Start)
	ON_BN_CLICKED(IDC_BUTTON_RULE, &CgongzhuDlg::OnClickedButtonRule)
	ON_BN_CLICKED(IDC_BUTTON_RANK, &CgongzhuDlg::OnClickedButtonRank)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, &CgongzhuDlg::OnClickedButtonPaste)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CgongzhuDlg::OnClickedButtonGenerate)
	ON_EN_CHANGE(IDC_EDIT1_START, &CgongzhuDlg::OnChangeEdit1Start)
END_MESSAGE_MAP()


// CgongzhuDlg 消息处理程序

BOOL CgongzhuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_pCgongzhuDlg = this;

	srand((unsigned)time(0));

	m_Edit_Start.SetWindowText(TEXT("请输入玩家数字ID"));
	m_Edit_Ip.SetWindowText(TEXT("请输入服务器IP地址"));
	m_Check_Start.SetCheck(BST_CHECKED);
	GetDlgItem(IDC_EDIT1_IP)->EnableWindow(FALSE);
	m_Button_Paste.EnableWindow(FALSE);//禁用粘贴按钮

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CgongzhuDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CgongzhuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CgongzhuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CgongzhuDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();//该函数已经调用UpdateData(TRUE);

	if (m_iEdit_Start > 0)
	{
		g_id = m_iEdit_Start;//修改全局变量
		if (m_Check_Start.GetCheck())
			g_order = 1;
		else
			g_order = 0;
		if (!g_order)
		{
			g_ipAddr = (CW2A(m_szEdit_Ip.GetString()));//把CString转std::string

		}

		//g_pCDlgLink = new CDlgLink;
		//g_pCDlgLink->DoModal();
		CDlgLink newdlg;
		g_pCDlgLink = &newdlg;
		g_pCDlgLink->DoModal();

	}

}


void CgongzhuDlg::OnClickedCheck1Start()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_Check_Start.GetCheck()==TRUE)
	{ 
		GetDlgItem(IDC_EDIT1_IP )->EnableWindow(FALSE );
		m_Button_Paste.EnableWindow(FALSE);
	}
	else
	{ 
		GetDlgItem(IDC_EDIT1_IP)->EnableWindow(TRUE); 
		m_Button_Paste.EnableWindow(TRUE);
	}
}



void CgongzhuDlg::OnClickedButtonRule()
{
	MessageBox(_T("发牌：\n使用去掉大小王后的52张牌，由系统发给四个玩家。\n\n牌的大小：\n大小按如下顺序递减：A,K,Q,J,10,9,8,7,6,5,4,3,2,\n黑桃Q代表“猪”；红方片J代表“羊”；黑梅花10代表“变压器”\n\n亮牌：\n也叫卖牌，开始出牌前，玩家依次将手中的猪，羊，变压器亮出来，这些牌的分值加倍。\n\n分牌：\n包括猪，羊，变压器，所有红桃。\n“猪”是-100分，如果亮过则是-200分。\n“羊”是+100分，如果亮过是+200分，而且所有红桃的分数加倍。\n得到“变压器”的玩家，手中其他牌的分数之和乘2，如果黑梅花10（变压器）卖过，则乘以4；\n如果玩家手中除了黑梅花10（变压器),没有其他分牌，则得+50分，如果卖过则的+100分；\n如果玩家手中除了黑梅花10（变压器),还有其他分牌，且其他分数之和为0，则总分数记为0。\n\n出牌：\n一次只能出一张。第一局游戏由得到黑梅花2的一方首先出牌，并且必须出黑梅花2，以后每局由上一局得到猪牌的一方首先出牌。\n后面的玩家必须和首家出相同花色的牌，如果没有该花色，可以垫其他花色的牌，一轮出牌后，最大方得到该轮的所有分牌，垫其他花色者算最小。\n亮过的牌不能在该花色首轮出牌时出，但在出其他花色是可以垫，玩家该花色只有一张牌时，则不论是否亮过都必须出。\n\n满红：\n如果一方在一轮游戏结束时将全部红桃收到自己手中，称为满红，注意要收齐13张红桃（包括2/3/4),则得到+200分，如果红桃A亮过牌，则得+400分。\n\n满贯：\n如果一方在一轮游戏结束时将全部含有分值的牌都收齐了，称为满贯，一副牌的情况下，得800分，如果有亮牌，则相应加倍计算。\n\n单人计分规则：\n梅花10为变压器，若某人获得变压器，则该人分牌的所有分数是原来的两倍；\n黑桃Q为猪牌，分值-100；\n红桃2/3/4为0分，5~10每张-10分，J/Q/K/A分别为-20/-30/-40/-50分。\n若单人红桃收齐（包括2/3/4),则全部转为正分（即200分）；\n若单人收到黑桃Q及全部红桃，则黑桃Q变为+100分，方片J为+100分；\n若收齐了全部红桃，及黑桃Q，方块J，梅花10，则变压器按四倍计算，即得分为1600分。\n\n拱猪玩法：\n主机端开设服务器，非主机端玩家输入主机IPv4地址连接到游戏（主机端和客户端须连接至同一个联网的局域网），客户端可以选择用机器人托管（可以开一个主机端和三个托管的客户端实现单人人机对战）。位置相对为一伙（主机端为北座，客户端可自由调换位置），共组成两队，队内选手分汇总为本队分数。若在一局中，所有分牌全部出完，该局游戏结束。\n每名玩家可选择一个ID，用来存储金币，金币会在一局游戏结算时根据队伍得分增加或减少，新玩家默认分配1000金币，金币存档保存在服务器的本地文件“Sav.dat”。\n\n常见问题：\n牌的大小与按钮不匹配——右击\"gongzhu.exe\"->\"属性\"->\"兼容性\"->\"更改高DPI设置\"->勾选\"代替高DPI缩放行为\"->\"缩放执行：\"改为\"系统\"->\"确定\"x2"), _T("游戏规则"));
}




void CgongzhuDlg::OnClickedButtonRank()
{
	string str;
	fstream mystream("Sav.dat", ios::in | ios::binary);//判断文件是否存在
	if (mystream.good())//文件存在
	{
		int IDtemp = 0, Goldtemp = 0, maxnum=50, num = 0;
		Player* pPlayer = new Player[maxnum];

		for (int i = 0; ; i++)//读文件，存进数组中
		{
			mystream.read((char*)&IDtemp, sizeof(int));
			mystream.read((char*)&Goldtemp, sizeof(int));

			if (mystream.eof())
				break;

			pPlayer[i].gold = Goldtemp;
			pPlayer[i].ID = IDtemp;

			if (i + 1 == maxnum)//数组扩容
			{
				Player* tempplay = new Player[maxnum * 2];
				for (int j = 0; j < maxnum; j++)
				{
					tempplay[j].gold = pPlayer[i].gold;
					tempplay[j].ID = pPlayer[i].ID;
				}
				delete[] pPlayer;
				pPlayer = tempplay;
				maxnum *= 2;
			}
			num++;
		}
		mystream.close();

		for (int i = 0; i < num - 1; i++)//降序排列
		{
			int k = i;
			int key = pPlayer[i].gold;
			for (int j = i + 1; j < num; j++)  //找出剩余数据中最小的数
			{
				if (pPlayer[j].gold > key)
				{
					k = j;
					key = pPlayer[j].gold;
				}
			}
			if (k != i)
				swap(pPlayer[i], pPlayer[k]);
		}

		for (int i = 0; i < 5; i++)
		{
			if (i + 1 > num)
				break;

			str += to_string(i + 1) + ".  ID:" + to_string(pPlayer[i].ID) + "  Gold:" + to_string(pPlayer[i].gold) + "\n\n";
		}

		delete[]pPlayer;
	}
	
	CString cstr;
	cstr = str.c_str();
	MessageBox(cstr, _T("金币排行"), MB_OK);


}




void CgongzhuDlg::OnClickedButtonPaste()//把复制的内容填写进IP地址栏
{
	
	if (OpenClipboard())
	{
		CString   fromClipboard;
		HANDLE   hData = GetClipboardData(CF_TEXT);
		char* buffer = (char*)GlobalLock(hData);
		fromClipboard = buffer;
		GlobalUnlock(hData);
		CloseClipboard();
		m_Edit_Ip.SetWindowText(fromClipboard);
	}
	else
		MessageBox(_T("错误"), _T("从剪贴板中粘贴"), MB_OK);
}


void CgongzhuDlg::OnClickedButtonGenerate()
{

	int tempint = (rand() % 9 + 1) * 100000000 + (rand() % 10000) * 10000 + rand() % 10000;//随机生成九位数
	CString cstr(to_string(tempint).c_str());

	m_Edit_Start.SetWindowText(cstr);
}


void CgongzhuDlg::OnChangeEdit1Start()
{
	if (IsFirstSet)
		IsFirstSet = false;
	else
	{
		CString cstr;
		m_Edit_Start.GetWindowText(cstr);
		string STDStr(CW2A(cstr.GetString()));
		if (STDStr.length() > 9)
		{
			m_Edit_Start.SetWindowText(_T(""));
			MessageBox(_T("请输入不超过9位的数字ID"), _T("输入非法"), MB_OK);
		}
	}
}
