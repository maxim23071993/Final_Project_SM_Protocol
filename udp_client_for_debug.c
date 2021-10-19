//
// Created by max on 25/08/2021.
//
#include "SMP_MQTT_UDP.h"
int main()
{

    struct sm_msg * message;
    message=(struct sm_msg *)malloc(sizeof(struct sm_msg));
    strcpy(message->topic,"test");
    strcpy(message->payload,"789");
    udp_init_client();
    udp_send(message);
    ACK_rcv();
    return 0;
}