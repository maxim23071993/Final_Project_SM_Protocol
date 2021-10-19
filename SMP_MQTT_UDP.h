// Created by Maxim & Shlomi on 24/08/2021.
#ifndef MQTT_CLIENTS_SMP_MQTT_UDP_H
#define MQTT_CLIENTS_SMP_MQTT_UDP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

/*###############################################*/

/*###############################################*/
//MQTT defines and global variables
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "#"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
/*###############################################*/
//message queue defines and global variables
#define MAX_TOPIC_PAYLOAD_SIZE 500
struct sm_msg {
    char topic[20];
    char payload[MAX_TOPIC_PAYLOAD_SIZE];
};
int msqid_global;
#define PERMS 0644
/*###############################################*/
//UDP defines and global variables
#define PORT     8080
#define MAXLINE 1024
#define  MAX_UDP_PACKET 65500
#define MAX_SM_MSG_ARR_SIZE    MAX_UDP_PACKET/(MAX_TOPIC_PAYLOAD_SIZE*2)
int sockfd;
struct sockaddr_in servaddr,cliaddr;
struct sm_msg_arr{
    struct sm_msg arr[MAX_SM_MSG_ARR_SIZE];
    int arr_size;
};
/*###############################################*/
//message queue functions
void msg_que_create(char *topic);
void message_queue_send( char *msg_payload,char * topic);
void msg_rcv_init(int* msqid);
void read_from_message_queue(struct sm_msg *message,int msqid);
/*###############################################*/
//UDP functions
void udp_init_client();
void* udp_send(struct sm_msg* message);
void* ACK_rcv();
void udp_init_server();
void udp_rcv_server(struct sm_msg *message);
void ACK_send(char * ack);
/*###############################################*/
#endif //MQTT_CLIENTS_SMP_MQTT_UDP_H
