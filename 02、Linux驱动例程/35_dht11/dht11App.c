/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: dht11App.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: dht11设备测试APP。
其他	   	: 无
使用方法	 ：./dht11App /dev/dht11
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/08/25 正点原子Linux团队创建
***************************************************************/
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"

int main()
{
    int fd, ret, i = 0;
    unsigned char data[5];


    fd = open("/dev/dht11", 0);

    if(fd < 0)
    {
        perror("open device failed\n");
        exit(1);
    }
    else
        printf("Open success!\n");

    while(1)
    {
        ret = read(fd, &data, sizeof(data));
        if(ret == 0) {	/* 读取到数据 */
			if(data[4] == data[0] + data[1] + data[2] + data[3]) {
				printf("Temp:%d.%d℃,Humi:%d.%d%\r\n", data[2],data[3], data[0],data[1]);
			}
		}
    sleep(1);
    }
}
