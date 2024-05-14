#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "../../include/default.h"
#include <string.h>

int main(int argc, char *argv[]){
	struct input_event event = {0};
	int descriptor = -1;
	char status[9];

	/* У�鴫�� */
    if (2 != argc) {
        fprintf(stderr, "usage: %s <input-dev>\n", argv[0]);
        exit(-1);
    }

    /* ���ļ� */
    if(0 > (descriptor = open(argv[1], O_RDONLY))){
    	LOG();
    	perror("open error");
    	exit(-1);
    }


    for( ; ; ){
    	/* ѭ����ȡ���� */
    	if(sizeof(struct input_event) != read(descriptor, &event, sizeof(struct input_event))){
    		LOG();
    		perror("read error");
    		exit(-1);
    	}

    	switch(event.value){
    		case 0:
    			strcpy(status, "up");
    			break;
    		case 1:
    			strcpy(status, "down");
    			break;
    		case 2:
    			strcpy(status, "press");
    			break;
            default:
                strcpy(status, "");
    	}

    	printf("����:%d ��ֵ:%d ֵ:%d ״̬:%s\n", event.type, event.code, event.value, status);
    }
}