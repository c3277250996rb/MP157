#include <stdio.h>
#include <stdlib.h>
#include <tslib.h>      //包含tslib.h头文件
#include "../../include/default.h"

int main(int arc, char *argv[]){
	struct ts_dev *descriptor = NULL;
	struct ts_sample object;
	int pressure = 0;


	if(NULL == (descriptor = ts_setup(NULL, 0))){
		LOG();
		perror("setup error");
		exit(-1)
	}


	while(1){
		if(0 > ts_read(ts, &object, 1)){
			LOG();
			perror("read error");
			ts_close(descriptor);
			exit(-1);
		}

		if(object.pressure){
			if(pressure){
				printf("move(%d , %d)\n", object.x, object.y);
			}else{
				printf("down(%d , %d)\n", object.x, object.y);
			}

		}else{
			printf("up\n");

		}

		pressure = object.pressure;
	}

	ts_close(descriptor);
	exit(-1);
}