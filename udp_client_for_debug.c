//
// Created by max on 25/08/2021.
//
#include "SMP_MQTT_UDP.h"
int main()
{
    int c_len = sizeof(cliaddr);
    int s_len = sizeof(servaddr);
    struct sm_msg_arr * message;

    message=(struct sm_msg_arr *)malloc(sizeof(struct sm_msg_arr));
    message->arr_size=1;
    strcpy(message->msg_arr[0].topic,"test");
    strcpy(message->msg_arr[0].payload,"789");
    int len=sizeof(message);
    udp_init_client();
    //sendto(send_socket, (const char *) message, sizeof(message), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,s_len;
//    udp_send(message);
//    ACK_rcv();

    return 0;
}