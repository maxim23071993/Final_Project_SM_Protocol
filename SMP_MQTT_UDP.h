//
// Created by Maxim & Shlomi on 24/08/2021.
//
#ifndef MQTT_CLIENTS_SMP_MQTT_UDP_H
#define MQTT_CLIENTS_SMP_MQTT_UDP_H
#include "MQTTClient.h"
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
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
#include <arpa/inet.h>
#include <inttypes.h>
/*####################################################################################################################*/
//MQTT defines and global variables
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "#"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
/*####################################################################################################################*/
//RTT, RTO and network parameters estimation and defines
struct timeval t0;
struct timeval t1;
float RTT;
float RTO;
float DEV;
float sampled_rtt;


struct smp_network_params{
    int client_port;
    char CLIENT_IP[15];
    int server_port;
    char SERVER_IP[15];
    int bandwidth;
    int NUM_OF_TRY;//connection retransmission number
    float INIT_TIME_OUT;
    float MAX_ALLOW_RTT;
    float ALPHA;
    float BETA;
    float GAMMA;
    float DELTA;
    int typical_rtt;
};
struct smp_network_params network_params;
/*####################################################################################################################*/
//message queue defines and global variables
#define PERMS 0644
#define MAX_PAYLOAD_SIZE 30
#define MAX_TOPIC_SIZE 20
#define MAX_NUM_OF_MESSAGE (30000/(MAX_TOPIC_SIZE+MAX_PAYLOAD_SIZE))
#define SMP_SYSTEM_MESSAGE "SMP SYS MSG"

int msqid_global;
pthread_mutex_t lock;
/*####################################################################################################################*/
//UDP defines and global variables
#define CLIENT_PORT 8080
#define SERVER_PORT 8081
#define TIME_TO_WAIT 10 // useconds

int client_socket;
int server_socket;

struct sockaddr_in servaddr,cliaddr;

struct sm_msg {
    char topic[MAX_TOPIC_SIZE];
    char payload[MAX_PAYLOAD_SIZE];
};
struct sm_msg_arr{
    struct sm_msg  msg_arr[MAX_NUM_OF_MESSAGE];
    int arr_size;
    int sq_number;
};

struct window_control{
    int seq_num;
    int status; //(-1)-not sent yet,1-sent and waiting to ack
    struct timeval t;
    int num_of_trys;
};
struct smp_client_server_params{
    float rtt;
    float rto;
    int window_size;
    int smp_msg_arr_size;
};
struct smp_client_server_params client_server_params;
struct window_control * windowcontrol;
/*####################################################################################################################*/
//MQTT functions
int mqtt_publish(struct sm_msg *message);
/*####################################################################################################################*/
//message queue functions
void msg_que_create(char *topic);
void message_queue_send( char *msg_payload,char * topic);
void msg_rcv_init(int* msqid,char *topic);
void read_from_message_queue(struct sm_msg *message,int msqid);
void message_encapsulation(struct sm_msg_arr *arr,int data_arr_size,int sqe_number,int * sqe_send_arr);
/*####################################################################################################################*/
//UDP functions
void client_sockets_creation();
void server_sockets_creation();
int NETWORK_PARAMS_INIT();
void udp_init_client();
void udp_init_server();
void server_init_respond();
void Update_Net_Params(float SAMPLE_RTT);
void sequence_number_select(int * previous_sqe,int window_size,int time_to_wait_for_sequence_select);
void init_params(char *file_name);
float timedifference_msec(struct timeval x, struct timeval y);

/*####################################################################################################################*/
//Thread routine
void * sender_routine();
void * receiver_routine();
void* win_control_routine();

#endif //MQTT_CLIENTS_SMP_MQTT_UDP_H
