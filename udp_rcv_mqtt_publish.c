//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"
int main()
{
    int s_len=sizeof(servaddr);
    int c_len=sizeof(cliaddr);
    int n;
    struct sm_msg_arr  message;
    char file_name[100]={"//home//max//Desktop//MQTT Subscribe+msg_que+udp_git//SMP_PARAMS.txt"};
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