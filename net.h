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

#define DEFAULT_PORT 5959  //端口
#define BUFFER_LENGTH 64


class user
{
public:
	int ID;//用户ID
	int gold;//用户金币数
	string IP;//用户ip
	string receiver_port;//消息发送端口
	struct sockaddr_in receiver;//存储接受接人的地址

	user(int playerId,int Gold);

};

class server
{
public:
	WSADATA wsaData; //存储启动套接字地址
	SOCKET sSocket; //用于接收消息的套接字
	struct sockaddr_in ser; //服务器地址
	struct sockaddr_in cli; //客户地址
	int cli_length = sizeof(cli);	//客户地址长度
	char recv_buf[BUFFER_LENGTH];	//接收数据的缓冲区长度
	int iSend;	//存储服务器发送字符串长度
	int iRecv;	//储存服务器接收字符串长度

	server();
	~server();
	bool Startup(); //检测是否满足服务器的启动要求,成功返回true，失败返回false
	bool SetServerSocket();	//生成套接字，绑定地址
	void Send_Message( string message, sockaddr_in addr );//发送消息
};

class client
{
public:
	WSADATA wsaData;	//启动套接字
	SOCKET sClient;		//接收和发送时使用的套接字
	struct sockaddr_in ser;	//保存服务器地址
	int ser_length = sizeof(ser);	//保存服务器长度

	char recv_buf[BUFFER_LENGTH]; //接收信息的缓冲区
	int iSend; //发送信息长度
	int iRecv; //接收信息长度

	client();
	~client();
	bool Startup();	//生成套接字
	void SetServerAddress(); //设置服务器地址
	int GeneratePort();		//端口分配
	int Send_Message(string);//发送消息给服务器
};


