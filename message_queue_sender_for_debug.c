//
// Created by max on 22/10/2021.
//
#include "SMP_MQTT_UDP.h"
#include <stdlib.h>
void main(int argc,void *argv)
{
    char file_name[100]={"//home//max//Desktop//MQTT Subscribe+msg_que+udp_git//SMP_PARAMS.txt"};
    int i;
    int msqid;
    char topic[10];
    char payload[10];
    struct sm_msg msg;
    struct sm_msg_arr arr[10];
   msg_que_create("incapsulation_debug");
    //msg_rcv_init(&msqid,"msgq");
    int sqe_arr[2];

   for( i=0;i<150;i++)
    {
        message_queue_send("topic","payload");
    }
  //  message_encapsulation(arr,200,0,sqe_arr);*/
   // init_params((file_name));
}