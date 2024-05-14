#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "../../include/default.h"

// #define SERVER_PORT		8080          	//服务器的端口号
// #define SERVER_IP   	"192.168.7.249"	//服务器的IP地址

int main(int argc, char **argv){
	struct sockaddr_in object = {0};
	char buffer[512];
	int server_descriptor;
	int status;
	char ip[20] = {0};
	int port;

	if(3 != argc){
		printf("usage: %s <SERVER_IP> <SERVER_PORT>\n", argv[0]);
		exit(-1);
	}

	strcpy(ip, argv[1]);
	port = atoi(argv[2]);

	/* 打开套接字，得到套接字描述符 */
    server_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (0 > server_descriptor) {
    	LOG();
        perror("socket error");
        exit(EXIT_FAILURE);
    }

	/* 调用connect连接远端服务器 */
    object.sin_family = AF_INET;
    object.sin_port = htons(port);  //端口号
    inet_pton(AF_INET, ip, &object.sin_addr);//IP地址

    if(0 > (status = connect(server_descriptor, (struct sockaddr *) &object, sizeof(object)))){
    	LOG();
		perror("connect error");
        close(server_descriptor);
        exit(EXIT_FAILURE);
    }

    printf("服务器连接成功...\n\n");


    while(1){

    	memset(buffer, 0x0, sizeof(buffer));

		// 接收用户输入的字符串数据
        printf("Please enter a string: ");
        fgets(buffer, sizeof(buffer), stdin);


        if(0 > (status = send(server_descriptor, buffer, strlen(buffer), 0))){
			LOG();
			perror("connect error");
        	close(server_descriptor);
        	exit(EXIT_FAILURE);
        }


        if(0 == strncmp(buffer, "exit", 4)){
        	break;
        }
    }

    close(server_descriptor);
    exit(-1);
}