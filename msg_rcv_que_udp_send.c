#include "SMP_MQTT_UDP.h"

    int main(int argc,char* argv[])
    {
        struct sm_msg_arr data[10];
        int msqid;
        int s_len=sizeof(servaddr);
        int c_len=sizeof(cliaddr);

        msg_rcv_init(&msqid);
        udp_init_client();
       while(1)
       {
           message_encapsulation(data,10,0);

               //udp_send(message);
              // for(int i=0;i<1;i++)
               sendto(client_socket, (const char *)data, sizeof(struct sm_msg_arr), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
               ACK_rcv();

        }
       /* free(message);
        printf("UDP: Close Socket.\n");
        close(sockfd);
        printf("message queue: done receiving messages.\n");
        system("rm msgq.txt");*/
        return 0;
    }
