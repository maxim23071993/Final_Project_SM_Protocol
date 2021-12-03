//
// Created by Maxim & Shlomi on 24/08/2021.
//
#ifndef MQTT_CLIENTS_SMP_MQTT_UDP_H
#define MQTT_CLIENTS_SMP_MQTT_UDP_H
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
/*####################################################################################################################*/
//MQTT defines and global variables
#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "#"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
/*####################################################################################################################*/
//RTT and RTO estimation defines and global variables
struct timeval t0;
struct timeval t1;
float RTT;
float RTO;
float DEV;
float RTT_SERVER;
float RTO_SERVER;
float sampled_rtt;
struct smp_network_params{
    int CLIENT_PORT;
    char CLIENT_IP[15];
    int SERVER_PORT;
    char SERVER_IP[15];
    int bandwidth;
    int NUM_OF_TRY;//connection retransmission number
    float INIT_TIME_OUT;
    float MAX_ALLOW_RTT;
    float ALPHA;
    float BETA;
    float GAMMA;
    float DELTA;
};
struct smp_network_params network_params;
/*####################################################################################################################*/
//message queue defines and global variables
#define PERMS 0644
#define MAX_PAYLOAD_SIZE 30
#define MAX_TOPIC_SIZE 20
#define SMP_SYSTEM_MESSAGE "SMP SYS MSG"
struct sm_msg {
    char topic[MAX_TOPIC_SIZE];
    char payload[MAX_PAYLOAD_SIZE];
};
int msqid_global;
pthread_mutex_t lock;
/*####################################################################################################################*/
//UDP defines and global variables
#define MAXLINE 1024
#define TIME_TO_WAIT_FOR_WINDOW 100 // useconds

//#define UDP_BANDWIDTH 100000 //100 kbps //+
//#define MAX_UDP_PACKET (UDP_BANDWIDTH/8)
//#define SM_MSG_MAX_ARR_SIZE (MAX_UDP_PACKET/70)

int client_socket;
int server_socket;
//int window_size;

struct sockaddr_in servaddr,cliaddr;

struct sm_msg_arr{
    struct sm_msg  *msg_arr;
    int arr_size;
    int sq_number;
};

struct window_control{
    int seq_num;
    int status; //(-1)-not sent yet,1-sent and waiting to ack
    struct timeval t;
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
int udp_init_server();
int RTT_init_respond();
void Update_Net_Params(float SAMPLE_RTT);
void sequence_number_select(int * previous_sqe,int window_size,int time_to_wait_for_sequence_select);
/*####################################################################################################################*/
//Thread routine
void * sender_routine();
void * receiver_routine();
void init_params(char *file_name);

#endif //MQTT_CLIENTS_SMP_MQTT_UDP_H
