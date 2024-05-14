#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <linux/watchdog.h>
#include "../../include/default.h"

#define watchdog_path "/dev/watchdog"

int main(int argc, char **argv){
	struct watchdog_info info;
	int timeout;
	int t;
	int descriptor;
	int option;

	/* 校验传参 */
	if (2 != argc) {
		fprintf(stderr, "usage: %s <timeout>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* 打开看门狗 */
	if(0 > (descriptor = open(watchdog_path, O_RDWR))){
		LOG();
		fprintf(stderr, "open error: %s: %s\n", watchdog_path, strerror(errno));
		exit(-1);
	}

	/* 打开之后看门狗计时器会开启、先停止它 */
	if(0 > ioctl(descriptor, WDIOC_SETOPTIONS, WDIOS_DISABLECARD)){
		LOG();
		perror("WDIOS_DISABLECARD");
		close(descriptor);
		exit(1);
	}

	timeout = atoi(argv[1]);
	if(1 > timeout){
		timeout = 1;
	}

	/* 设置超时时间 */
	printf("timeout: %ds\n", timeout);
	if(0 > ioctl(descriptor, WDIOC_SETTIMEOUT, &timeout)){
		LOG();
		perror("WDIOC_SETTIMEOUT");
		close(descriptor);
		exit(-1);
	}

	/* 开启看门狗计时器 */
	if(0 > ioctl(descriptor, WDIOC_SETOPTIONS, WDIOS_ENABLECARD)){
		LOG();
		perror("WDIOS_ENABLECARD");
		close(descriptor);
		exit(-1);
	}

	/* 喂狗 */
	t = (timeout * 1000 - 100) * 1000;	//喂狗时间设置us微秒、在超时时间到来前100ms喂狗
	while(1){
		usleep(t);
		ioctl(descriptor, WDIOC_KEEPALIVE, NULL);
	}
}