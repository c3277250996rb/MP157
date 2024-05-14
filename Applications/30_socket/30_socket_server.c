#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../../include/default.h"

#define port 8080	//端口号不能发生冲突,不常用的端口号通常大于5000

int main(void){
	struct sockaddr_in server_address = {0};
	struct sockaddr_in client_address = {0};
	char client_ip[20] = {0};
	int srever_descriptor, client_descriptor;
	int lenght = sizeof(client_address);
	char buffer[512];
	int status;


	/* 打开套接字，得到套接字描述符 */
	srever_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (0 > srever_descriptor) {
		LOG();
		perror("socket error");
		exit(EXIT_FAILURE);
	}

	/* 将套接字与指定端口号进行绑定 */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);

	status = bind(srever_descriptor, (struct sockaddr *) &server_address, sizeof(server_address));
	if(0 > status){
		LOG();
		perror("bind error");
		close(srever_descriptor);
		exit(-1);
	}

	/* 使服务器进入监听状态 */
	if(0 > (status = listen(srever_descriptor, 50))){
		LOG();
		perror("listen error");
		close(srever_descriptor);
		exit(-1);
	}

	/* 阻塞等待客户端连接 */
	if(0 > (client_descriptor = accept(srever_descriptor, (struct sockaddr *) &client_address, &lenght))){
		LOG();
		perror("accept error");
		close(srever_descriptor);
		exit(-1);
	}

	printf("有客户端接入...\n");
    inet_ntop(AF_INET, &client_address.sin_addr.s_addr, client_ip, sizeof(client_ip));
    printf("客户端主机的IP地址: %s\n", client_ip);
    printf("客户端进程的端口号: %d\n", client_address.sin_port);

    /* 接收客户端发送过来的数据 */
    while(1){
    	// 接收缓冲区清零
    	memset(buffer, 0x0, sizeof(buffer));

    	// 读数据
    	if(0 >= (status = recv(client_descriptor, buffer, sizeof(buffer), 0))){
			LOG();
			perror("recv error");
			close(srever_descriptor);
			break;
    	}

    	// 将读取到的数据以字符串形式打印出来
		printf("client: %s\n", buffer);

		// 如果读取到"exit"则关闭套接字退出程序
		if(0 == strncmp("exit", buffer, 4)){	//最多比较前 4 个字符。
			printf("server exit...\n");
			close(srever_descriptor);
			break;
		}
    }

    /* 关闭套接字 */
    close(srever_descriptor);
    exit(0);
}