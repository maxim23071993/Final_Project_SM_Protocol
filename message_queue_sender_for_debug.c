//
// Created by max on 22/10/2021.
//
#include "SMP_MQTT_UDP.h"

int main()
{
    struct sm_msg msg;
    msg_que_create("incapsulation_debug");
    for(int i=0;i<10;i++)
    {
        message_queue_send("payload","topic");
    }


    return 0;
}