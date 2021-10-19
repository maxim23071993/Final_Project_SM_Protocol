#include "SMP_MQTT_UDP.h"

    int main(int argc,char* argv[]){
        struct sm_msg *message;
        int *msqid;
        message=(struct sm_msg*)malloc(sizeof(struct sm_msg));
        udp_init_client();
        msg_rcv_init(&msqid);
        while(strcmp(message->payload,"close_sm")!=0){
            read_from_message_queue(message,msqid);
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
