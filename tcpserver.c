#include<stdio.h>  // printf scanf 
#include<stdlib.h> // exit
#include<string.h>  // strcpy strlen
#include<sys/socket.h>  // socket 
#include<netinet/in.h>  // struct sockaddr_in 
#include<arpa/inet.h>

int main()
{
	// step1:创建套接字
	int servfd; // 套接字、文件描述符，  用于建立连接
			// AF_INET:协议族 ip+port， tcp
	servfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_DGRAM  ---- UDP
	// step2: 绑定地址
	struct sockaddr_in  servaddr;
	servaddr.sin_family= AF_INET;
	servaddr.sin_addr.s_addr = 	htonl(INADDR_ANY)	;  // 0.0.0.0 => 32位整数
	servaddr.sin_port =  htons(1234);  //端口号 htons htonl
	int ret;
	//ret = bind(servfd, &servaddr, sizeof(servaddr));
	ret = bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if(ret < 0 )
	{
		printf("bind error\n");
		exit(1);  //退出程序
		
	}
	
	// step3: 监听
	listen(servfd, 5);  //把自己变成变动
	
	// step4: 等待客户端的连接--一直等待
	int newfd; // 用于跟连接进来的客户端进行数据收发
	printf("waitng for client .......\n");
	newfd = accept(servfd, NULL, NULL); // 阻塞函数
	printf("client connect successful\n");
	
	char str[1024];
	while(1)
	{
		read(newfd, str, 1024);
		printf("%s\n",str);
	}
	
	close(newfd);
	close(servfd);
	
	return 0;
}
