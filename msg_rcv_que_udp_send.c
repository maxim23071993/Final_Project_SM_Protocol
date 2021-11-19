#include "SMP_MQTT_UDP.h"

    int main(int argc,char* argv[])
    {
        struct sm_msg_arr arr[10];
        arr[0].arr_size=0;
        arr[0].sq_number=0;
        int msqid;
        int s_len=sizeof(servaddr);
        int c_len=sizeof(cliaddr);

        msg_rcv_init(&msqid_global);
        udp_init_client();


        message_encapsulation(&arr,10,1);

               //udp_send(message);
              // for(int i=0;i<1;i++)
               sendto(client_socket, arr, sizeof(struct sm_msg_arr), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
               ACK_rcv();


       /* free(message);
        printf("UDP: Close Socket.\n");
        close(sockfd);
        printf("message queue: done receiving messages.\n");
        system("rm msgq.txt");*/
        return 0;
    }
