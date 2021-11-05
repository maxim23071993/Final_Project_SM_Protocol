//
// Created by max on 22/10/2021.
//
#include "SMP_MQTT_UDP.h"
#include <stdlib.h>
int main()
{
    int i;
    char topic[10];
    char payload[10];
    struct sm_msg msg;
    msg_que_create("incapsulation_debug");
    for( i=0;i<200;i++)
    {

        message_queue_send("topic","payload");
    }


    return 0;
}