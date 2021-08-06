#pragma once

#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <windows.h>
#include "game.h"
#include <tchar.h>


#pragma comment(lib,"ws2_32.lib")
using namespace std;

#define DEFAULT_PORT 5959  //�˿�
#define BUFFER_LENGTH 64


class user
{
public:
	int ID;//�û�ID
	int gold;//�û������
	string IP;//�û�ip
	string receiver_port;//��Ϣ���Ͷ˿�
	struct sockaddr_in receiver;//�洢���ܽ��˵ĵ�ַ

	user(int playerId,int Gold);

};

class server
{
public:
	WSADATA wsaData; //�洢�����׽��ֵ�ַ
	SOCKET sSocket; //���ڽ�����Ϣ���׽���
	struct sockaddr_in ser; //��������ַ
	struct sockaddr_in cli; //�ͻ���ַ
	int cli_length = sizeof(cli);	//�ͻ���ַ����
	char recv_buf[BUFFER_LENGTH];	//�������ݵĻ���������
	int iSend;	//�洢�����������ַ�������
	int iRecv;	//��������������ַ�������

	server();
	~server();
	bool Startup(); //����Ƿ����������������Ҫ��,�ɹ�����true��ʧ�ܷ���false
	bool SetServerSocket();	//�����׽��֣��󶨵�ַ
	void Send_Message( string message, sockaddr_in addr );//������Ϣ
};

class client
{
public:
	WSADATA wsaData;	//�����׽���
	SOCKET sClient;		//���պͷ���ʱʹ�õ��׽���
	struct sockaddr_in ser;	//�����������ַ
	int ser_length = sizeof(ser);	//�������������

	char recv_buf[BUFFER_LENGTH]; //������Ϣ�Ļ�����
	int iSend; //������Ϣ����
	int iRecv; //������Ϣ����

	client();
	~client();
	bool Startup();	//�����׽���
	void SetServerAddress(); //���÷�������ַ
	int GeneratePort();		//�˿ڷ���
	int Send_Message(string);//������Ϣ��������
};


