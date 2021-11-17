#include "SMP_MQTT_UDP.h"

    int main(int argc,char* argv[])
    {
        int c_len = sizeof(cliaddr);
        int s_len = sizeof(servaddr);
        struct sm_msg_arr *message;
        int msqid;
        message=(struct sm_msg_arr*)malloc(sizeof(struct sm_msg_arr));
        udp_init_client();
        msg_rcv_init(&msqid);
       while(1){
           if(message_encapsulation(message)==1)
           {
               //udp_send(message);
               sendto(client_send_socket, (const char *)message, sizeof(message), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
               ACK_rcv();
           }
        }
       /* free(message);
        printf("UDP: Close Socket.\n");
        close(sockfd);
        printf("message queue: done receiving messages.\n");
        system("rm msgq.txt");*/
        return 0;
    }
