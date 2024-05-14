#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../../include/default.h"

static char pwm_path[] = "/sys/class/pwm/pwmchip4/pwm2";//TIM1_CH3

static int pwm_config(const char *attribute, const char *value){
	char config_path[100];
	int length;
	int descriptor;

	sprintf(config_path, "%s/%s", pwm_path, attribute);
	if(0 > (descriptor = open(config_path, O_WRONLY))){
		LOG();
		perror("open error");
		return descriptor;
	}

	length = strlen(value);
	if(length != write(descriptor, value, length)){
		LOG();
		perror("write error");
		close(descriptor);
		return -1;
	}

	close(descriptor);
	return 0;
}

int main(int argc, char **argv){
	/* 校验传参 */
    if (3 != argc) {
        fprintf(stderr, "usage: %s <period> <duty_cycle>\n", argv[0]);
        exit(-1);
    }

    /* 打印配置信息 */
    printf("PWM config: period<%s>, duty<%s>\n", argv[1], argv[2]);

    /* 导出pwm */
    if(access(pwm_path, F_OK)){				//如果pwm2目录不存在, 则导出
    	int descriptor = open("/sys/class/pwm/pwmchip4/export", O_WRONLY);
    	if(0 > descriptor){
    		LOG();
    		perror("open error");
    		exit(-1);
    	}

    	if(1 != write(descriptor, "2", 1)){	//导出TIM1_CH3通道
    		LOG();
    		close(descriptor);
    		exit(-1);
    	}

    	close(descriptor);
    }

    if(pwm_config("period", argv[1]))		exit(-1);	/* 配置PWM周期 */
		LOG();
    if(pwm_config("duty_cycle", argv[2]))	exit(-1);	/* 配置占空比 */
		LOG();
    if(pwm_config("enable", "1"))			exit(-1);	/* 使能pwm */

	LOG_msg("success");
    exit(0);
}