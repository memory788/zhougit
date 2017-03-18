#include<stdio.h>  // printf scanf 
#include<stdlib.h> // exit
#include<string.h>  // strcpy strlen
#include<sys/socket.h>  // socket 
#include<netinet/in.h>  // struct sockaddr_in 
#include<arpa/inet.h>
#include<wiringPi.h>  // 硬件控制


//404找不到指定资源的头部信息
const static char http_head_error[] = "HTTP/1.1 404 Not Found\r\nContent-type: text/html\r\n\r\n";
//200成功的头部信息
const static char http_head_ok[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
//LED关灯页面
const static char http_body_off[] =
"<html><head><title>RPI_led</title></head>"
"<body><h1>LED 7 is off</h1>"
"<div/><input type='button' value=led_ON onclick=\"location.href='ledon.act';\" />"
"<p><input type='button' value=led_OFF onclick=\"location.href='ledoff.act';\" />"
"</body></html>";
//LED开灯页面
const static char http_body_on[] =
"<html><head><title>RPI_led</title></head>"
"<body><h1>LED 7 is on</h1>"
"<div/><input type='button' value=led_ON onclick=\"location.href='ledon.act';\" />"
"<p><input type='button' value=led_OFF onclick=\"location.href='ledoff.act';\" />"
"</body></html>";


int tcp_serv_init(int serv_port)	
{	
	int port = serv_port;
	// step1:创建套接字
	int servfd; // 套接字、文件描述符，  用于建立连接
			// AF_INET:协议族 ip+port， tcp
	servfd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_DGRAM  ---- UDP
	// step2: 绑定地址
	struct sockaddr_in  servaddr;
	servaddr.sin_family= AF_INET;
	servaddr.sin_addr.s_addr = 	htonl(INADDR_ANY)	;  // 0.0.0.0 => 32位整数
	servaddr.sin_port =  htons(port);  //端口号 htons htonl
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
	return servfd;
}

void led_init()
{
		
	pinMode(8, OUTPUT);   //D0
	pinMode(9, OUTPUT);		// R
	pinMode(7, OUTPUT);		//G
	pinMode(0, OUTPUT);		// B
	
	digitalWrite(9, HIGH);
	digitalWrite(7, HIGH);
	digitalWrite(0, HIGH);
	
}
void http_send_file(int sockfd, char *filename)
{
		
		if(strcmp(filename, "/ledoff.act") == 0)
		{
			write(sockfd, http_head_ok, strlen(http_head_ok));
			write(sockfd, http_body_off, strlen(http_body_off));
			digitalWrite(8, HIGH);
			digitalWrite(9, HIGH);
		}
		
		else if(strcmp(filename, "/ledon.act") == 0)
		{
			write(sockfd, http_head_ok, strlen(http_head_ok));
			write(sockfd, http_body_on, strlen(http_body_on));
			digitalWrite(8,LOW);
			digitalWrite(9,LOW);
		}
		else
		{
			write(sockfd, http_head_error, strlen(http_head_error));
		}
}
void serve(int sockfd)
{
	//printf("serve handle such client......");
	char buff[1024];
	int cnt;
	cnt = read(sockfd, buff, 1024);
	if(strncmp(buff, "GET", 3) == 0)
	{
			char *file = buff + 4;
			char *space = strchr(file,  ' ');
			*space = '\0';
			http_send_file(sockfd, file);
	}
}
int main()
{

	
	int fd = tcp_serv_init(4321);
	
	wiringPiSetup();
	
	led_init();
	
	
	int newfd; 
	
	while(1)
	{	
		printf("waiting for client ......\n");
		newfd = accept(fd, NULL, NULL);
		printf("client connect successful\n");
		serve(newfd);
		close(newfd);
	}
	close(fd);
	return 0;
}	
	
	
	
