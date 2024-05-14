#include "my_mqtt.h"

int main(int argc, char *argv[]){
	MQTTClient client;
	MQTTClient_connectOptions connectOptions = MQTTClient_connectOptions_initializer;
	MQTTClient_willOptions willOptions = MQTTClient_willOptions_initializer;
	MQTTClient_message messageToPublic = MQTTClient_message_initializer;
	int result;

	//创建mqtt客户端对象
	if(MQTTCLIENT_SUCCESS != (result = MQTTClient_create(&client, BROKER_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL))){
		printf("Failed to create client, return code %d\n", result);
		result = EXIT_FAILURE;
		goto Exit;
	}

	/* 设置回调 */
	if(MQTTCLIENT_SUCCESS != (result = MQTTClient_setCallbacks(client, NULL, connect_lost_log, receive_message, NULL))){
		printf("Failed to set callbacks, return code %d\n", result);
		result = EXIT_FAILURE;
		goto DestoryExit;
	}

	/* 连接MQTT服务器 */
	willOptions.topicName = WILL_TOPIC;	//遗嘱主题
	willOptions.message = "Unexpected disconnection";//遗嘱消息
	willOptions.retained = 1;	//保留消息
	willOptions.qos = 0;		//QoS0

	connectOptions.will = &willOptions;
	connectOptions.keepAliveInterval = 30;	//心跳包间隔时间
	connectOptions.cleansession = 0;			//cleanSession标志
	// connectOptions.username = USERNAME;		//用户名
	// connectOptions.password = PASSWORD;		//密码
	if(MQTTCLIENT_SUCCESS != (result = MQTTClient_connect(client, &connectOptions))){
		printf("Failed to connect, return code %d\n", result);
		result = EXIT_FAILURE;
		goto DestoryExit;
	}

	printf("MQTT服务器连接成功!\n");
	
	/* 发布上线消息 */
	messageToPublic.payload = "Online";	//消息的内容
	messageToPublic.payloadlen = 6;		//内容的长度
	messageToPublic.qos = 0;				//QoS等级
	messageToPublic.retained = 1;		//保留消息
	if(MQTTCLIENT_SUCCESS != (result = MQTTClient_publishMessage(client, WILL_TOPIC, &messageToPublic, NULL))){
		printf("Failed to publish message, return code %d\n", result);
		result = EXIT_FAILURE;
		goto DisConnectExit;
	}

	/* 订阅主题 dt_mqtt/led */
	if(MQTTCLIENT_SUCCESS != (result = MQTTClient_subscribe(client, LED_TOPIC, 0))){
		printf("Failed to subscribe, return code %d\n", result);
		result = EXIT_FAILURE;
		goto DisConnectExit;
	}


	/* 向服务端发布芯片温度信息 */
	while(1){
		LOG_msg("continue.");

		MQTTClient_message temp_message = MQTTClient_message_initializer;
		char temp_string[10] = {0};
		int descripitor;

		//
		descripitor = open("/sys/class/hwmon/hwmon0/temp1_input", O_RDONLY);
		read(descripitor, temp_string, sizeof(temp_string));
		close(descripitor);

		/* 发布温度信息 */
		temp_message.payload = temp_string;	//消息的内容
		temp_message.payloadlen = strlen(temp_string);		//内容的长度
		temp_message.qos = 0;				//QoS等级
		temp_message.retained = 1;		//保留消息
		if(MQTTCLIENT_SUCCESS != (result = MQTTClient_publishMessage(client, TEMP_TOPIC, &temp_message, NULL))){
			printf("Failed to publish message, return code %d\n", result);
			result = EXIT_FAILURE;
			goto UnSubscribeExit;
		}

		LOG_msg(temp_string);
		sleep(10);
	}


UnSubscribeExit:
	if (MQTTCLIENT_SUCCESS != (result = MQTTClient_unsubscribe(client, LED_TOPIC))) {
		printf("Failed to unsubscribe, return code %d\n", result);
		result = EXIT_FAILURE;
	}

DisConnectExit:
	if(MQTTCLIENT_SUCCESS != (result = MQTTClient_disconnect(client, 10000))){
		printf("Failed to disconnect, return code %d\n", result);
		result = EXIT_FAILURE;
	}

DestoryExit:
	MQTTClient_destroy(&client);

Exit:
	return result;
}