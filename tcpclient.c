#include<stdio.h>  // printf scanf 
#include<stdlib.h> // exit
#include<string.h>  // strcpy strlen
#include<sys/socket.h>  // socket 
#include<netinet/in.h>  // struct sockaddr_in 
#include<arpa/inet.h>

int main()
{
	// step1:创建套接字
	int clientfd; // 套接字、文件描述符，  用于建立连接
			// AF_INET:协议族 ip+port， tcp
	clientfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_DGRAM  ---- UDP
	
	
	struct sockaddr_in myservaddr;
	myservaddr.sin_family = AF_INET;
	myservaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	myservaddr.sin_port = htons(1234);
	connect(clientfd, &myservaddr, sizeof(myservaddr));
	
	char str[1024];
	while(1)
	{
		scanf("%s",str);
		write(clientfd, str, strlen(str)+1);
		
	}
	
	return 0;
}
