#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: chrdevbaseApp.c
作者	  	: 正点原子
版本	   	: V1.0
描述	   	: chrdevbase驱测试APP。
其他	   	: 使用方法：./chrdevbase /dev/chrdevbase <1>|<2>
  			 argv[2] 1:读文件
  			 argv[2] 2:写文件		
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2019/1/30 正点原子团队创建
***************************************************************/

#define LOG() printf("%d", __LINE__);

static char usrdata[] = {"usr data!"};

void case_read(int descripitor){
	char buffer[100];
	if(0 > read(descripitor, buffer, 50)){
		perror("read error");
		LOG();
	}else{
		LOG();
		printf("\nread:\n %s\n", buffer);
	}

	close(descripitor);
}

void case_write(int descripitor, char *buffer){
	if(0 > write(descripitor, buffer, sizeof(buffer))){
		perror("write error");
		LOG();
		close(descripitor);
	}else{
		// printf("case_read()\n");
		// case_read(descripitor);
	}
}

void case_ioctl(int descripitor){
	if(0 > ioctl(descripitor, 1)){
		perror("ioctl error");
		LOG();
	}else{
		system("xxd /dev/globalmem | head -n 5");
	}

	close(descripitor);
}



/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int fd, retvalue;
	char *filename;
	char readbuf[100], writebuf[100];

	// if(argc != 3){
	// 	printf("Error Usage!\r\n");
	// 	return -1;
	// }

	system("sudo rmmod 01d_char_device_driver");
	system("sudo insmod 01d_char_device_driver.ko");

	// system("sudo rm /dev/globalmem");
	// system("sudo mknod /dev/globalmem c 230 0");
	system("sudo chmod 777 /dev/globalmem");

	// filename = argv[1];
	filename = "/dev/globalmem";

	char *choice;
	int int_choice, descripitor;
	while(1){
		if(0 > (descripitor = open(filename, O_RDWR))){
			LOG();
			perror("open error");
			return -1;
		}

		printf(" 1.read\n 2.write\n 3.ioctl\n 4.exit\n");
		scanf("%s", choice);
		int int_choice = atoi(choice);

		switch(int_choice){
			case 1:
				case_read(descripitor);
				break;
			case 2:
				printf("input something to write: ");
				scanf("%s", choice);
				case_write(descripitor, choice);
				break;
			case 3:
				case_ioctl(descripitor);
				break;
			case 4:
				LOG();
				close(descripitor);
				break;
		}

		if(int_choice == 4)
			break;
	}

	system("sudo rmmod 01d_char_device_driver");
	return 0;
}




