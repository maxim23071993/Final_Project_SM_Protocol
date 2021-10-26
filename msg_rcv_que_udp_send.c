#include "SMP_MQTT_UDP.h"

    int main(int argc,char* argv[]){
        struct sm_msg_arr *message;
        int msqid;
        message=(struct sm_msg_arr*)malloc(sizeof(struct sm_msg_arr));
        udp_init_client();
        msg_rcv_init(&msqid);
        while(1){
            message=message_incapsulation();
            udp_send(message);
            ACK_rcv();
        }
        free(message);
        printf("UDP: Close Socket.\n");
        close(sockfd);
        printf("message queue: done receiving messages.\n");
        system("rm msgq.txt");
        return 0;
    }
