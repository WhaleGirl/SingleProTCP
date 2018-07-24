#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		printf("Usage %s [ip] [port]\n",argv[0]);
		return 1;
	}

	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		return 2;
	}

	struct sockaddr_in server_socket;
	struct sockaddr_in client_socket;
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = inet_addr( argv[1]);
	server_socket.sin_port = htons(atoi(argv[2]));

	//绑定端口
	if(bind(sock,(struct sockaddr*)&server_socket,sizeof(struct sockaddr_in))<0)
	{
		perror("bind");
		return 3;
	}
	//由于这是TCP协议，所以是可靠传输，那么这里需要监听对方是否已连接

	if(listen(sock,5)<0)
	{
		perror("listen");
		return 4;
	}

	while(1)
	{
		socklen_t len=sizeof(client_socket);
		//作为服务器，需要先接收再发送,接收来自客户端的消息
		int client_sock = accept(sock,(struct sockaddr*)&client_socket,&len);
		if(client_sock<0)
		{
			//printf("accept error\n");
			perror("accept");
			return 5;
		}

	char buf_ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET,&client_socket.sin_addr,buf_ip,sizeof(buf_ip));
	printf("get new connect,[ip] :%s [port]: %d\n",buf_ip,ntohs(client_socket.sin_port));
		while(1)
		{
			char buf[1024];
			ssize_t s = read(client_sock,buf,sizeof(buf));
			if(s>0)
			{
				buf[strlen(buf)-1] = 0;
				printf("%s",buf);
				write(client_sock,buf,strlen(buf)+1);
			}
			else if (s==0)
			{
				printf("quit\n");
				break;
			}
			else
			{
				perror("read");
				break;
			}
		}
	}
	close(sock);
	return 0;
}
