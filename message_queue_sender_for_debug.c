//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"
void main()
{
    int i;
    int msqid;
    char topic[10];
    char payload[10];
    struct sm_msg msg;
    struct sm_msg_arr arr[10];
    msg_que_create("msgq");
    //msg_rcv_init(&msqid,"msgq");
    int sqe_arr[2];

   for( i=0;i<10;i++)
    {
       sprintf(payload,"%d",i);
     //  sleep(0.01);
        message_queue_send("topic",payload);
    }
  //  message_encapsulation(arr,200,0,sqe_arr);*/
   // init_params((file_name));
}