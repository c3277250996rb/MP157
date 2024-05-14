#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "MQTTClient.h"		//包含MQTT客户端库头文件



/* ########################宏定义##################### */
#define BROKER_ADDRESS	"tcp://mq.tongxinmao.com:18830"	//然也物联平台社区版MQTT服务器地址

/* 客户端id、用户名、密码 *
 * 当您成功申请到然也物联平台的社区版MQTT服务后
 * 然也物联工作人员会给你发送8组用于连接社区版MQTT服务器
 * 的客户端连接认证信息：也就是客户端id、用户名和密码
 * 注意一共有8组，您选择其中一组覆盖下面的示例值
 * 后续我们使用MQTT.fx或MQTTool的时候 也需要使用一组连接认证信息
 * 去连接社区版MQTT服务器！
 */
#define CLIENTID		"txm_1714895439"		//客户端id
// #define USERNAME		"您的用户名"		//用户名
// #define PASSWORD		"您的密码"			//密码

/* 然也物联社区版MQTT服务为每个申请成功的用户
 * 提供了个人专属主题级别，在官方发给您的微信信息中
 * 提到了
 * 以下 dt_mqtt/ 便是笔者的个人主题级别
 * dt_mqtt其实就是笔者申请社区版MQTT服务时注册的用户名
 * 大家也是一样，所以你们需要替换下面的dt_mqtt前缀
 * 换成你们的个人专属主题级别（也就是您申请时的用户名）
 */
#define WILL_TOPIC		"/public/TEST/zat/will"		//遗嘱主题
#define LED_TOPIC		"/public/TEST/zat/led"		//LED主题
#define TEMP_TOPIC		"/public/TEST/zat/temperature"	//温度主题
/* ################################################# */

#define LOG_msg(msg) printf("Line %d: %s\n", __LINE__, msg)
#define LOG() printf("Line %d\n", __LINE__)

// 不可缺失未用到的*context和topicLen参数
static int receive_message(void *context, char *topic_name, int topicLen, MQTTClient_message *message){
	LOG();
	if(!strcmp(topic_name, LED_TOPIC)){ // strcmp: 返回值等于 0，则表示字符串 str1 等于字符串 str2
		LOG();
		char *temp = message -> payload;
		LOG();
		LOG_msg(temp);

		if(!strcmp("0", temp)){ //如果是"0"则LED熄灭
			system("echo none > /sys/class/leds/user-led/trigger");
			system("echo 0 > /sys/class/leds/user-led/brightness");
		}else if(!strcmp("1", temp)){ //如果是"1"则LED常量
			system("echo none > /sys/class/leds/user-led/trigger");
			system("echo 1 > /sys/class/leds/user-led/brightness");
		}

		if(!strcmp("2", temp)){ //如果接收到的消息是"2"则设置LED为呼吸灯模式
			system("echo heartbeat > /sys/class/leds/user-led/trigger");
		}
	}

	LOG();
	/* 释放占用的内存空间 */
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topic_name);

	return 1;
}

// 不可缺失未用到的*context参数
static void connect_lost_log(void *context, char *cause){
	printf("\nConnection lost\n");
	printf("    cause: %s\n", cause);
}
