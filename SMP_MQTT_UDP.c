//
// Created by max on 24/08/2021.
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
    printf(" message queue recvd: %s : %s\n", message->topic,message->payload);
}
void msg_rcv_init(int* msqid){
    key_t key;
    if ((key = ftok("msgq.txt", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    if (((*msqid) = msgget(key, PERMS)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }
    printf("message queue: ready to receive messages.\n");
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
    servaddr.sin_addr.s_addr =/*inet_addr("192.168.1.116"); //*/INADDR_ANY;
    printf("UDP Socket created\n");
}
void* udp_send(struct sm_msg* message)
        {
    sendto(sockfd, message, sizeof(struct sm_msg),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    printf("%s %s  sent over udp.\n",message->topic,message->payload);
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
}
void* ACK_rcv(){
    int n, len;
    char buffer[MAXLINE];
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
}
void udp_rcv_server(struct sm_msg *message)
{
    int len, n;
    len=sizeof(cliaddr);
    n = recvfrom(sockfd, message,sizeof(struct sm_msg), MSG_WAITALL, ( struct sockaddr *) &cliaddr,&len);
    printf("Client : %s  : %s",message->topic,message->payload);
}
void ACK_send(char * ack){
    int len=sizeof(cliaddr);
    sendto(sockfd, (const char *) ack, strlen(ack),
           MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
           len);
    printf(" ACK  %s message sent.\n",ack);
}