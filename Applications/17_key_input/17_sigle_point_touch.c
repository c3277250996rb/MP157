#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include "../../include/default.h"

int main(int argc, char *argv[]){
	struct input_event event_object;
	int x, y;
	int value;	//用于记录BTN_TOUCH事件的value,1表示按下,0表示松开,-1表示移动
	int valid;	//用于记录数据是否有效(我们关注的信息发生更新表示有效,1表示有效,0表示无效)
	int descriptor = -1;

    /* 校验传参 */
    if (2 != argc) {
        fprintf(stderr, "usage: %s <input-dev>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 打开文件 */
    if(0 > (descriptor = open(argv[1], O_RDONLY))){
    	LOG();
    	perror("open error");
    	exit(EXIT_FAILURE);
    }

    x = y = 0;
    value = -1;	//初始化<移动>
    valid = 0;	//初始化<无效>

    while(1){
    	if(sizeof(struct input_event) != read(descriptor, &event_object, sizeof(struct input_event))){
    		LOG();
    		perror("read error");
    		exit(EXIT_FAILURE);
    	}

    	switch(event_object.type){
    		case EV_KEY:	//按键事件
    			if(BTN_TOUCH == event_object.code){
    				value = event_object.value;
    				valid = 1;
    			}
    			break;

    		case EV_ABS:	 //绝对位移事件
    			switch(event_object.code){
    	    		case ABS_X:
    	    			x = event_object.value;
    	    			valid = 1;
    	    			break;
    				case ABS_Y:
    					y = event_object.value;
    					valid = 1;
    					break;
    			}
    			break;

    		case EV_SYN:	//同步事件
    			if(SYN_REPORT == event_object.code){
    				if(valid){
    					switch(value){
    						case 1:
    							printf("down(%d , %d)\n", x, y);
    							break;
    						case 0:
    							printf("up\n");
    							break;
    						case -1:
    							printf("move(%d , %d)\n", x, y);
    							break;
    					}

    					valid = 0;	//重置valid
    					value = -1;	//重置value
    				}
    			}
    			break;
    	}
    }

}