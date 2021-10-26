//
// Created by Maxim & Shlomi on 24/08/2021.
//
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
#define MAX_PAYLOAD_SIZE 50
#define MAX_TOPIC_SIZE 20
struct sm_msg {
    char topic[MAX_TOPIC_SIZE];
    char payload[MAX_PAYLOAD_SIZE];
};
int msqid_global;
#define PERMS 0644
/*###############################################*/
//UDP defines and global variables
#define PORT     8080
#define MAXLINE 1024
#define UDP_THROUPUT 100000 //100 kbps
#define MAX_UDP_PACKET (UDP_THROUPUT/8)
#define SM_MSG_MAX_ARR_SIZE (MAX_UDP_PACKET/70)

int sockfd;
struct sockaddr_in servaddr,cliaddr;

struct sm_msg_arr{
    struct sm_msg  msg_arr[SM_MSG_MAX_ARR_SIZE];
    int arr_size;
    int sq_number;
};
/*###############################################*/
//message queue functions
void msg_que_create(char *topic);
void message_queue_send( char *msg_payload,char * topic);
void msg_rcv_init(int* msqid);
void read_from_message_queue(struct sm_msg *message,int msqid);
struct sm_msg_arr* message_incapsulation();
/*###############################################*/
//UDP functions
void udp_init_client();
void* udp_send(struct sm_msg_arr* message);
void* ACK_rcv();
void udp_init_server();
void udp_rcv_server(struct sm_msg_arr *message);
void ACK_send(char * ack);
/*###############################################*/
#endif //MQTT_CLIENTS_SMP_MQTT_UDP_H
