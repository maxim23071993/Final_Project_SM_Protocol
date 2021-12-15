//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"
int mqtt_publish(struct sm_msg *message)
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to create client, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    pubmsg.payload = message->payload;
    pubmsg.payloadlen = (int)strlen(message->payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    if ((rc = MQTTClient_publishMessage(client, message->topic, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to publish message, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for up to %d seconds for publication of %s\n"
           "on topic %s for client with ClientID: %s\n",
           (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
        printf("Failed to disconnect, return code %d\n", rc);
    MQTTClient_destroy(&client);
    return rc;
}

int main()
{
    int s_len=sizeof(servaddr);
    int c_len=sizeof(cliaddr);
    int n;
    struct sm_msg_arr  message;
    char file_name[100]={"//home//shlomi//Desktop//MQTT Subscribe+msg_que+udp_git//SMP_PARAMS.txt"};
    init_params(file_name);
    //message.msg_arr=(struct sm_msg *)malloc(sizeof(struct sm_msg));
    //message=(struct sm_msg *)malloc(sizeof(struct sm_msg));
    udp_init_server();
    printf("Server waiting for message.\n");
    while(1)
    {
        n=recvfrom(server_socket, &message, sizeof(struct sm_msg_arr), MSG_WAITALL, (struct sockaddr *) &cliaddr, &c_len);
        if(n!=-1) {
            sendto(server_socket, &message.sq_number, sizeof(int), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,c_len);
            printf("\n############################   %d   ####################################\n", message.sq_number);
            printf("ACK on message seq %d was sent to client\n", message.sq_number);
            for (int i = 0; i < message.arr_size; i++)
                printf("%s %s \n", message.msg_arr[i].topic, message.msg_arr[i].payload);
            //mqtt_publish(message);
        }
    }
    return 0;
}