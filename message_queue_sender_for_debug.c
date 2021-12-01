//
// Created by max on 22/10/2021.
//
#include "SMP_MQTT_UDP.h"
#include <stdlib.h>
void main1()
{
    int i;
    int msqid;
    char topic[10];
    char payload[10];
    struct sm_msg msg;
    struct sm_msg_arr arr[10];
   //msg_que_create("incapsulation_debug");
    msg_rcv_init(&msqid,"msgq");
    int sqe_arr[2];

  /* for( i=0;i<10;i++)
    {
        message_queue_send("topic","payload");
    }*/
    message_encapsulation(arr,200,0,sqe_arr);
}