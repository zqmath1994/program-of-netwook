#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")


int main()
{
	WSADATA wsaData;
	SOCKET Serversocket;
	SOCKET NewSocket;
	SOCKADDR_IN ServerAddr;
	SOCKADDR_IN ClientAddr;
	int ClientAddrLen;
	int ServerPort = 4000;
	char ReceiveBuffer[1024];
	int Result;
	int ReceiveLen;
	if((Result = WSAStartup(MAKEWORD(2,2),&wsaData))!=0){
		printf("Winsock2.2��ʼ������,������Ϊ:%d\n",Result);
		return -1;
	}
	Serversocket =socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(Serversocket<0){
		printf("socketʧ�ܣ�������Ϊ:%d\n,WSAGetLastError()");
		return -1;
	}
	memset(&ServerAddr,0,sizeof(ServerAddr));
	ServerAddr.sin_family= AF_INET;
	ServerAddr.sin_port=htons(ServerPort);
	ServerAddr.sin_addr.s_addr=htons(INADDR_ANY);
	Result=bind(Serversocket,(SOCKADDR*)&ServerAddr,sizeof(ServerAddr));
	if(Result<0){
		printf("�׽��ְ󶨴���,������Ϊ:%d\n",WSAGetLastError());
		return -1;
	}
	Result =listen(Serversocket,5);
	if(Result==SOCKET_ERROR){
		printf("����ʧ�ܣ�������Ϊ:%d\n",WSAGetLastError());
		return -1;
	}
	ClientAddrLen=sizeof(SOCKADDR);
	int SendLength;
	do 
	{
		NewSocket = accept(Serversocket,(SOCKADDR*)&ClientAddr,&ClientAddrLen);
		if(NewSocket<0){
			printf("����ʧ��,������Ϊ:%d\n",WSAGetLastError());
			return -1;
		}
		else do{
			memset(ReceiveBuffer,0,sizeof(ReceiveBuffer));
			if((ReceiveLen=recv(NewSocket,ReceiveBuffer,1024,0))<0)
			{
				printf("������Ϊ:%d\n",WSAGetLastError());
				return -1;
			}
			if(ReceiveLen==0)
			{
				printf("�����������\n");
			}
			else
			{
				ReceiveBuffer[1024]='\0';
				printf("���յ�����:%s\n",ReceiveBuffer);
				SendLength=send(NewSocket,ReceiveBuffer,strlen(ReceiveBuffer),0);
				if(SendLength<0){
					printf("����ʧ�ܣ�������Ϊ%d\n",WSAGetLastError());
					return -1;
				}else
				{
					printf("���յ����ݣ�%s\n",ReceiveBuffer);
				}
			}
		}while(ReceiveLen!=0);
		if(closesocket(NewSocket)==SOCKET_ERROR){
			printf("�׽��ֹرմ���%d\n",WSAGetLastError());
		}
	} while (1);
	if(closesocket(Serversocket)==SOCKET_ERROR){
		printf("�׽��ֹرմ���%d\n",WSAGetLastError());
	}
	if(WSACleanup()==SOCKET_ERROR){
		printf("Winsock�ͷŴ���%d\n",WSAGetLastError());
	}
	return 0;
}