//
// Created by Maxim & Shlomi on 24/08/2021.
//
#ifndef MQTT_CLIENTS_SMP_MQTT_UDP_H
#define MQTT_CLIENTS_SMP_MQTT_UDP_H
#include <sys/wait.h>
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
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
/*####################################################################################################################*/
//MQTT defines and global variables
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "#"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
#define SERVER_IP "192.168.1.112"
#define NUM_OF_TRY 10
/*####################################################################################################################*/
//RTT and RTO estimation defines and global variables
#define NUM_OF_TRY 10
#define INIT_TIME_OUT 1
#define ALPHA 0.5
#define BETA 0.5
#define GAMMA 0.5
#define DELTA 0.1
struct timeval t0;
struct timeval t1;
float RTT;
float RTO;
float DEV;
float RTT_SERVER;
float RTO_SERVER;
/*####################################################################################################################*/
//message queue defines and global variables
#define MAX_PAYLOAD_SIZE 50
#define MAX_TOPIC_SIZE 20
struct sm_msg {
    char topic[MAX_TOPIC_SIZE];
    char payload[MAX_PAYLOAD_SIZE];
};
int msqid_global;
#define PERMS 0644
/*####################################################################################################################*/
//UDP defines and global variables
#define CLIENT_PORT  8080
#define SERVER_PORT  8086

#define MAXLINE 1024
#define UDP_BANDWIDTH 100000 //100 kbps
#define MAX_UDP_PACKET (UDP_BANDWIDTH/8)
#define SM_MSG_MAX_ARR_SIZE (MAX_UDP_PACKET/70)
int client_send_socket;
int client_receive_socket;
int server_send_socket;
int server_receive_socket;
//int receive_socket;
//int send_socket;
struct sockaddr_in servaddr,cliaddr;

struct sm_msg_arr{
    struct sm_msg  msg_arr[SM_MSG_MAX_ARR_SIZE];
    int arr_size;
    int sq_number;
};
/*####################################################################################################################*/
//message queue functions
void msg_que_create(char *topic);
void message_queue_send( char *msg_payload,char * topic);
void msg_rcv_init(int* msqid);
void read_from_message_queue(struct sm_msg *message,int msqid);
int message_encapsulation(struct sm_msg_arr *arr);
/*####################################################################################################################*/
//UDP functions
void client_sockets_creation();
void server_sockets_creation();
int NETWORK_PARAMS_INIT();
void udp_init_client();
void* udp_send(struct sm_msg_arr* message);
int ACK_rcv();
int udp_init_server();
void udp_rcv_server(struct sm_msg_arr *message);
void ACK_send(char * ack);
int RTT_init_respond();
/*####################################################################################################################*/
#endif //MQTT_CLIENTS_SMP_MQTT_UDP_H
