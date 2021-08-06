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



//server�ຯ�����ɹ�����true��ʧ�ܷ���false
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
		::MessageBox(NULL, _T("��ʼ���׽���ʧ��"), _T("����1"), MB_OK);
		return false;
	}
	return true;
}

bool server::SetServerSocket()	//�����׽��֣��󶨵�ַ
{
	//�����׽��֣�ʹ��UPDЭ��
	if ((sSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		::MessageBox(NULL, _T("���ɷ������׽���������ʧ��"), _T("����2"), MB_OK);
		return false;
	}

	ser.sin_family = AF_INET;//Э����
	ser.sin_port = htons(DEFAULT_PORT);//�˿�
	ser.sin_addr.s_addr = htonl(INADDR_ANY);//��ַ

	if (bind(sSocket, (LPSOCKADDR)&ser, sizeof(ser)) == SOCKET_ERROR)	//�󶨷�����
	{
		::MessageBox(NULL, _T("��������ַ��ʧ��"), _T("����3"), MB_OK);
		return false;
	}
	return true;
}

void server::Send_Message(string message, sockaddr_in addr)//������Ϣ
{
	//string ת char*
	char* message1 = new char[message.length() + 1];
	strcpy(message1, message.c_str());

	//������Ϣ
	iSend = sendto(g_pServer->sSocket, message1, message.length() + 1, 0, (SOCKADDR*)&(addr), sizeof(addr));
	//DWORD kk = WSAGetLastError();
	if (iSend == SOCKET_ERROR)//����ʧ��ʱ
	{
		g_pCDlgLink->MessageBox( _T("������Ϣʧ��"), _T("����5"));
		return;
	}

	delete[] message1;
}

//client�ຯ��
client::client()
{
	g_c_port = GeneratePort();//����˿�

	if (Startup() == FALSE)//�����׽���
		return;

	SetServerAddress();//���÷�������ַ
}
client::~client() {}

bool client::Startup()//�����׽���
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		::MessageBox(NULL, _T("��ʼ���׽���ʧ��"), _T("����8"), MB_OK);
		return false;
	}

	sClient = socket(AF_INET, SOCK_DGRAM, 0);
	if (sClient == INVALID_SOCKET)
	{
		::MessageBox(NULL, _T("�����׽���������"), _T("����9"), MB_OK);
		return false;
	}

	return true;
}

void client::SetServerAddress()//������������ַ
{
	ser.sin_family = AF_INET;
	ser.sin_port = htons(DEFAULT_PORT);
	ser.sin_addr.s_addr = inet_addr(g_ipAddr.c_str());
}

int client::GeneratePort()//�˿ڷ���
{
	int x = 1024 + rand() % (5000 - 1024);

	return x;
}

int client::Send_Message(string infor_temp)//������Ϣ��������
{
	char* infor = new char[infor_temp.length() + 1];
	strcpy(infor, infor_temp.c_str());

	//���͵�¼����
	iSend = sendto(sClient, infor, infor_temp.length() + 1, 0, (sockaddr*)&ser, ser_length);

	delete[] infor;
	return iSend;
}