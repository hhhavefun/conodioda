//#include<sys/types.h>
#include "pch.h"
#include<stdio.h>
#include <iostream>
#include<winsock2.h>
//#include<Windows.h>
//#include<string>

#pragma comment(lib,"ws2_32.lib")

int main(int argc,char* argv[])
{	
	//加载套接字库
	WSADATA wsad;
	WORD wVersion = MAKEWORD(2, 2);
	int err = WSAStartup(wVersion, &wsad);
	if (err != 0)
	{
		std::cout << "create wsadata error:" <<err<< std::endl;
		return -1;
	}
	if (LOBYTE(wsad.wVersion) != 2 || HIBYTE(wsad.wVersion) != 2)
	{
		std::cout << "wsadata version error." << std::endl;
		return -1;
	}
	else
		std::cout << "create wsadata successfully:."<< std::endl;
	//创建监听socket
	SOCKET sock= INVALID_SOCKET;
	sock= WSASocket(AF_INET, SOCK_RAW, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
	//int listenfd = socket(AF_INET, SOCK_RAW,IPPROTO_IP);
	int listenfd = WSAGetLastError();
	if (listenfd == INVALID_SOCKET) 
	{
		std::cout << "create listen socket error." << std::endl;
		return INVALID_SOCKET;
	}
	else 
		std::cout << "create listen socket successsfully." << std::endl;

	//初始化服务器，bind()绑定端口
	struct sockaddr_in bindaddr;
	bindaddr.sin_family =AF_INET;
	bindaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	bindaddr.sin_port = htons(3000);
	if (bind(sock, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == SOCKET_ERROR)
	{
		std::cout << "bind listen socket error." << std::endl;
		return SOCKET_ERROR;
	}
	else
		std::cout << "bind listen socket successsfully." << std::endl;
	//启动监听
	if (listen(sock, SOMAXCONN) == SOCKET_ERROR) 
	{
		std::cout << "listen error." << std::endl;
		return SOCKET_ERROR;
	}

	while (true)
	{
		struct sockaddr_in clientaddr;
		int clientfd = accept(sock, (struct sockaddr *)&clientaddr, (int *)sizeof(clientaddr));
		if (clientfd != INVALID_SOCKET)
		{
			char recvBuff[1024] = { 0 };
			char *sendBuff = (char*)"sendcheck";
			//从客户端接收
			int ret = recv(clientfd, recvBuff, 1024, 0);
			if (ret > 0) {
				std::cout << "recv operation from client:" << recvBuff << std::endl;
				//响应
				//函数模拟dmm6500返回
				send(clientfd, sendBuff, strlen(sendBuff), 0);
				std::cout << "send success." << std::endl;
			}
			else std::cout << "recv operation error." << std::endl;
			closesocket(clientfd);
		}
	}

	//closesocket(listenfd);
	system("pause");
		return 0;
}


