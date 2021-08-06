#include "pch.h"
#include "net.h"
#include "gongzhuDlg.h"
#include "CDlgLink.h"
#include "gongzhu.h"

user::user(int id,int Gold )
{
	ID = id;
	gold = Gold;

}



//server类函数，成功返回true，失败返回false
server::server()
{
	if (Startup() == FALSE)
		return;
	if (SetServerSocket() == FALSE)
		return;
}
server::~server(){}

bool server::Startup()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		::MessageBox(NULL, _T("初始化套接字失败"), _T("错误1"), MB_OK);
		return false;
	}
	return true;
}

bool server::SetServerSocket()	//生成套接字，绑定地址
{
	//生成套接字，使用UPD协议
	if ((sSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		::MessageBox(NULL, _T("生成服务器套接字描述符失败"), _T("错误2"), MB_OK);
		return false;
	}

	ser.sin_family = AF_INET;//协议族
	ser.sin_port = htons(DEFAULT_PORT);//端口
	ser.sin_addr.s_addr = htonl(INADDR_ANY);//地址

	if (bind(sSocket, (LPSOCKADDR)&ser, sizeof(ser)) == SOCKET_ERROR)	//绑定服务器
	{
		::MessageBox(NULL, _T("服务器地址绑定失败"), _T("错误3"), MB_OK);
		return false;
	}
	return true;
}

void server::Send_Message(string message, sockaddr_in addr)//发送消息
{
	//string 转 char*
	char* message1 = new char[message.length() + 1];
	strcpy(message1, message.c_str());

	//发送消息
	iSend = sendto(g_pServer->sSocket, message1, message.length() + 1, 0, (SOCKADDR*)&(addr), sizeof(addr));
	//DWORD kk = WSAGetLastError();
	if (iSend == SOCKET_ERROR)//发送失败时
	{
		g_pCDlgLink->MessageBox( _T("发送消息失败"), _T("错误5"));
		return;
	}

	delete[] message1;
}

//client类函数
client::client()
{
	g_c_port = GeneratePort();//分配端口

	if (Startup() == FALSE)//启动套接字
		return;

	SetServerAddress();//设置服务器地址
}
client::~client() {}

bool client::Startup()//创建套接字
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		::MessageBox(NULL, _T("初始化套接字失败"), _T("错误8"), MB_OK);
		return false;
	}

	sClient = socket(AF_INET, SOCK_DGRAM, 0);
	if (sClient == INVALID_SOCKET)
	{
		::MessageBox(NULL, _T("生成套接字描述符"), _T("错误9"), MB_OK);
		return false;
	}

	return true;
}

void client::SetServerAddress()//建立服务器地址
{
	ser.sin_family = AF_INET;
	ser.sin_port = htons(DEFAULT_PORT);
	ser.sin_addr.s_addr = inet_addr(g_ipAddr.c_str());
}

int client::GeneratePort()//端口分配
{
	int x = 1024 + rand() % (5000 - 1024);

	return x;
}

int client::Send_Message(string infor_temp)//发送消息给服务器
{
	char* infor = new char[infor_temp.length() + 1];
	strcpy(infor, infor_temp.c_str());

	//发送登录请求
	iSend = sendto(sClient, infor, infor_temp.length() + 1, 0, (sockaddr*)&ser, ser_length);

	delete[] infor;
	return iSend;
}