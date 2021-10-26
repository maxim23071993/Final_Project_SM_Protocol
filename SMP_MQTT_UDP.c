//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"
/*###############################################*/
//message queu
void msg_que_create(char *topic)
{
    int msqid;
    key_t key;
    char for_ftok[30];
    char for_system[20];

    strcpy(for_system,"touch ");
    strcat(for_system,topic);
    strcat(for_system,".txt");

    strcpy(for_ftok,topic);
    strcat(for_ftok,".txt");

    system(for_system);
    if ((key = ftok(for_ftok, 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    msqid_global=msqid;
    printf("message queue: ready to send messages\n");
}
void message_queue_send( char *msg_payload,char * topic)
{
    struct sm_msg message;
    strcpy(message.payload,msg_payload);
    strcpy(message.topic,topic);
    if (msgsnd(msqid_global, &message, sizeof(struct sm_msg), 0) == -1)
    {
        perror("msgsnd");
    }
}
void read_from_message_queue(struct sm_msg *message,int msqid){
    if (msgrcv(msqid, message, sizeof(struct sm_msg), 0, 0) == -1)
    {
        perror("msgrcv");
        exit(1);
    }
}
void msg_rcv_init(int* msqid){
    key_t key;
    if ((key = ftok("incapsulation_debug.txt", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    if (((*msqid) = msgget(key, PERMS)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }
    printf("message queue: ready to receive messages.\n");
}
struct sm_msg_arr* message_incapsulation()
{

    int rc;
    int i=0;
    struct msqid_ds buf;
    int num_messages;
    rc = msgctl(msqid_global, IPC_STAT, &buf);
    num_messages = buf.msg_qnum;
    //struct sm_msg msg;
    struct sm_msg_arr* arr=(struct sm_msg_arr*)malloc(sizeof(struct sm_msg_arr));
    arr->arr_size=0;


    while(arr->arr_size<SM_MSG_MAX_ARR_SIZE && num_messages!=0)
    {
        read_from_message_queue(arr->msg_arr,msqid_global);

        rc = msgctl(msqid_global, IPC_STAT, &buf);
        num_messages = buf.msg_qnum;

        arr->arr_size++;
    }
    return arr;
}
/*###############################################*/
//udp
void udp_init_client()
{
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&servaddr, 0, sizeof(servaddr));
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr =/*inet_addr("192.168.1.116");//*/INADDR_ANY;
    printf("UDP Client Socket Created\n");
}
void* udp_send(struct sm_msg_arr* message)
{
    sendto(sockfd, message, sizeof(struct sm_msg_arr),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    return 0;
}
void udp_init_server()
{
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("UDP Server Socket Created\n");
}
void* ACK_rcv(){
    int n, len;
    char buffer[MAXLINE];
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
    buffer[n] = '\0';
    printf("ACK from server : %s\n", buffer);
}
void udp_rcv_server(struct sm_msg_arr *message)
{
    int len, n;
    len=sizeof(cliaddr);
    n = recvfrom(sockfd, message,sizeof(struct sm_msg_arr), MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
    printf("Client ");
}
void ACK_send(char * ack){
    int len=sizeof(cliaddr);
    sendto(sockfd, (const char *) ack, strlen(ack),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
    printf(" ACK  %s message sent from server.\n",ack);
}
/*###############################################*/