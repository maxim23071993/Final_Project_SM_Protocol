#include "SMP_MQTT_UDP.h"
int main(int argc,void **argv)
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    //char file_name[100]={"//home//max//Desktop//MQTT Subscribe+msg_que+udp_git//SMP_PARAMS.txt"};
    int rc;
    int ch;
    char  topic[20];
    //init_params(file_name);
    msg_que_create("msgq");
    if(fork()==0){
        system("./msg_rcv_que_udp_send");
    }
    MQTTClient_create(&client, ADDRESS, TOPIC,MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("MQTT Subscribing\n");
    MQTTClient_subscribe(client, TOPIC, QOS);

    do
    {
        ch = getchar();
    } while(ch!='Q' && ch != 'q');

    MQTTClient_unsubscribe(client, topic);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}