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

/* ��������MT��㴥��ÿһ�����������Ϣ */
struct touch_point {
    int x;      	//X����
    int y;      	//Y����
    int id;     	//��ӦABS_MT_TRACKING_ID
    int valid;		//������Ч��־λ(=1��ʾ��������Ϣ��������)
};

/* һ���������x�����y���� */
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

     /* ����У�� */
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

    /* calloc()�ڶ��ж�̬�ط��� nmemb ������Ϊ size �������ռ� */
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