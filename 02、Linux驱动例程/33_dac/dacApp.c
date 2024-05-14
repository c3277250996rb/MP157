/***************************************************************
Copyright © ALIENTEK Co., Ltd. 1998-2029. All rights reserved.
文件名		: dacApp.c
作者	  	: 正点原子Linux团队
版本	   	: V1.0
描述	   	: dac测试应用文件。
其他	   	: 无
使用方法	 ：./dacApp
论坛 	   	: www.openedv.com
日志	   	: 初版V1.0 2021/09/24 正点原子Linux团队创建
***************************************************************/
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/ioctl.h"
#include "fcntl.h"
#include "stdlib.h"
#include "string.h"
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

/* 字符串转数字，将浮点小数字符串转换为浮点数数值 */
#define SENSOR_FLOAT_DATA_GET(ret, index, str, member)\
	ret = file_data_read(file_path[index], str);\
	dev->member = atof(str);\
	
/* 字符串转数字，将整数字符串转换为整数数值 */
#define SENSOR_INT_DATA_GET(ret, index, str, member)\
	ret = file_data_read(file_path[index], str);\
	dev->member = atoi(str);\

/* iio框架对应的文件路径 */
static char *file_path[] = {
	"/sys/bus/iio/devices/iio:device0/in_voltage_scale",
	"/sys/bus/iio/devices/iio:device0/in_voltage19_raw",
	"/sys/bus/iio/devices/iio:device1/out_voltage1_scale",
	"/sys/bus/iio/devices/iio:device1/out_voltage1_raw",
};

/* 文件路径索引，要和file_path里面的文件顺序对应 */
enum path_index {
	IN_VOLTAGE_SCALE = 0,
	IN_VOLTAGE_RAW,
	OUT_VOLTAGE1_SCALE,
	OUT_VOLTAGE1_RAW,
};

/*
 * dac数据设备结构体
 */
struct dac_dev{
	int dac_raw, adc_raw;
	float dac_scale, adc_scale;
	float dac_act, adc_act;
};

struct dac_dev stm32dac;

 /*
 * @description			: 读取指定文件内容
 * @param - filename 	: 要读取的文件路径
 * @param - str 		: 读取到的文件字符串
 * @return 				: 0 成功;其他 失败
 */
static int file_data_read(char *filename, char *str)
{
	int ret = 0;
	FILE *data_stream;

    data_stream = fopen(filename, "r"); /* 只读打开 */
    if(data_stream == NULL) {
		printf("can't open file %s\r\n", filename);
		return -1;
	}

	ret = fscanf(data_stream, "%s", str);
    if(!ret) {
        printf("file read error!\r\n");
    } else if(ret == EOF) {
        /* 读到文件末尾的话将文件指针重新调整到文件头 */
        fseek(data_stream, 0, SEEK_SET);  
    }
	fclose(data_stream);	/* 关闭文件 */	
	return 0;
}

 /*
 * @description	: 获取ADC、DAC数据
 * @param - dev : 设备结构体
 * @return 		: 0 成功;其他 失败
 */
static int dac_add_dac_read(struct dac_dev *dev)
{
	int ret = 0;
	char str[50];

	/* 1、获取ADC值 */
	SENSOR_FLOAT_DATA_GET(ret, IN_VOLTAGE_SCALE, str, adc_scale);
	SENSOR_INT_DATA_GET(ret, IN_VOLTAGE_RAW, str, adc_raw);

	/* 转换ADC采集到的实际电压值mV */
	dev->adc_act = (dev->adc_scale * dev->adc_raw)/1000.f;

	/* 2、获取DAC值 */
	SENSOR_FLOAT_DATA_GET(ret, OUT_VOLTAGE1_SCALE, str, dac_scale);
	SENSOR_INT_DATA_GET(ret, OUT_VOLTAGE1_RAW, str, dac_raw);

	/* 转换DAC理论电压值mV */
	dev->dac_act = (dev->dac_scale * dev->dac_raw)/1000.f;
	return ret;
}

 /*
 * @description	: 使能DAC
 * @return 		: 无
 */
void dac_enable(void)
{
	system("echo 0 > /sys/bus/iio/devices/iio:device1/out_voltage1_powerdown");
}

/*
 * @description	: 禁止DAC
 * @return 		: 无
 */
void dac_disable(void)
{
	system("echo 1 > /sys/bus/iio/devices/iio:device1/out_voltage1_powerdown");
}

/*
 * @description			: 设置DAC
 * @param - filename	: 要写的文件路径
 * @param - value 		: DAC原始值
 * @return 				: 无
 */
int dac_set(char *filename, int value)
{
	int ret = 0;
	FILE *data_stream;
	char str[10];

	/* 1、将整形变量转换为字符串 */
	sprintf(str, "%d", value);

	/* 2、打开文件流 */
    data_stream = fopen(filename, "w"); /* 只写打开 */
    if(data_stream == NULL) {
		printf("can't open file %s\r\n", filename);
		return -1;
	}

	/* 3、将文件指针重新调整到文件头 */
	fseek(data_stream, 0, SEEK_SET);  

	/* 4、向文件流写入数据 */
	ret = fwrite(str, sizeof(str), 1, data_stream);
    if(!ret) {
        printf("file read error!\r\n");
    } 

	/* 5、关闭文件 */
	fclose(data_stream);		
	return 0;
}

/*
 * @description		: main主程序
 * @param - argc 	: argv数组元素个数
 * @param - argv 	: 具体参数
 * @return 			: 0 成功;其他 失败
 */
int main(int argc, char *argv[])
{
	int ret = 0;
	unsigned int cmd;
	unsigned char str[100];

	if (argc != 1) {
		printf("Error Usage!\r\n");
		return -1;
	}

	dac_enable();	/* 是能DAC */
	while (1) {
		printf("请输入DAC原始值(0~4095):");
		ret = scanf("%d", &cmd);
		if (ret != 1) {				/* 参数输入错误 */
			fgets(str, sizeof(str), stdin);				/* 防止卡死 */
		} else {					/* 参数输入正确 */
			if((cmd < 0) || (cmd > 4095)) {
				printf("输入错误，请正确输入DAC值，范围:0~4095!\r\n");
				continue;
			}
			dac_set(file_path[OUT_VOLTAGE1_RAW], cmd);
			ret = dac_add_dac_read(&stm32dac);
			if(ret == 0) { 			/* 数据读取成功 */
				printf("DAC原始值：%d，理论电压值：%.3fV\r\n", stm32dac.dac_raw, stm32dac.dac_act);
				printf("ADC原始值：%d，实际电压值：%.3fV\r\n", stm32dac.adc_raw, stm32dac.adc_act);
				printf("\r\n");
			}
		}
	}
	return 0;
}
 