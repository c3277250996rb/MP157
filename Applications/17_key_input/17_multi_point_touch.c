#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>
#include "../../include/default.h"

/* 用于描述MT多点触摸每一个触摸点的信息 */
struct touch_point {
    int x;      	//X坐标
    int y;      	//Y坐标
    int id;     	//对应ABS_MT_TRACKING_ID
    int valid;		//数据有效标志位(=1表示触摸点信息发生更新)
};

/* 一个触摸点的x坐标和y坐标 */
struct coordinate {
    int x;
    int y;
};

static int slot = 0;
static struct coordinate xy[9] = {0};

static int single_read(const int descriptor, const int max_slots, struct touch_point *list){
    struct input_event event;
    int i;

    memset(list, 0x0, max_slots * sizeof(struct touch_point));
    for(i = 0; i < max_slots; i++){
        list[i].id = -2;
    }

    while(1){
        if(sizeof(struct input_event) != read(descriptor, &event, sizeof(struct input_event))){
            LOG();
            perror("read error");
            return -1;
        }

        switch (event.type) {
            case EV_ABS:
                switch (event.code) {
                    case ABS_MT_SLOT:
                        slot = event.value;
                        break;
                    case ABS_MT_POSITION_X:
                        xy[slot].x = event.value;
                        list[slot].valid = 1;
                        break;
                    case ABS_MT_POSITION_Y:
                        xy[slot].y = event.value;
                        list[slot].valid = 1;
                        break;
                    case ABS_MT_TRACKING_ID:
                        list[slot].id = event.value;
                        list[slot].valid = 1;
                        break;
                }
                break;

            case EV_SYN:
                if (SYN_REPORT == event.code) {
                    for (i = 0; i < max_slots; i++) {
                        list[i].x = xy[i].x;
                        list[i].y = xy[i].y;
                    }
                }
                return 0;
        }
    }
}

int main(int argc, char *argv[]){
    struct input_absinfo slot;
    struct touch_point *list = NULL;
    int max_slots;
    int descriptor;
    int i;

     /* 参数校验 */
    if (2 != argc) {
        fprintf(stderr,"usage: %s <input_dev>\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    if(0 > (descriptor = open(argv[1], O_RDONLY))){
        LOG();
        exit(EXIT_FAILURE);
    }


    if(0 > ioctl(descriptor, EVIOCGABS(ABS_MT_SLOT), &slot)){
        LOG();
        perror("ioctl error");
        close(descriptor);
        exit(EXIT_FAILURE);
    }

    max_slots = slot.maximum + 1 - slot.minimum;
    printf("max_slots: %d\n", max_slots);

    /* calloc()在堆中动态地分配 nmemb 个长度为 size 的连续空间 */
    list = calloc(max_slots, sizeof(struct touch_point));


    while(1){
        if(0 > single_read(descriptor, max_slots, list)){
            break;
        }

        for(i = 0; i < max_slots; i++){
            if(list[i].valid){
                if(0 <= list[i].id){
                    printf("slot<%d>: down(%d , %d)\n", i, list[i].x, list[i].y);
                }else if(-1 == list[i].id){
                    printf("slot<%d>: up\n", i);
                }else{
                    printf("slot<%d>: move(%d , %d)\n", i, list[i].x, list[i].y);
                }
            }
        }
    }


    close(descriptor);
    free(list);
    exit(EXIT_FAILURE);
}