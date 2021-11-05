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
        read_from_message_queue(&arr->msg_arr[arr->arr_size],msqid_global);

        rc = msgctl(msqid_global, IPC_STAT, &buf);
        num_messages = buf.msg_qnum;

        arr->arr_size++;
        if(arr->arr_size==150)
        {
            printf("");
        }
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
    servaddr.sin_addr.s_addr = INADDR_ANY;
    NETWORK_PARAMS_INIT();
    printf("UDP Socket created\n");
}

in_addr_tinet_addr( char *string) {
    return 0;
}
void* udp_send(struct sm_msg_arr* message)
{
    sendto(sockfd, message, sizeof(struct sm_msg_arr),MSG_CONFIRM, (const struct sockaddr *) &servaddr,sizeof(servaddr));
    return 0;
}
int udp_init_server()
{
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed!");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    //Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    //servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    RTT_init_respond();
}
void* ACK_rcv(){

    //int c_len=sizeof(cliaddr);
    int s_len=sizeof(servaddr);
    int n;
    char buffer[MAXLINE];
    struct timeval tv;
    tv.tv_sec = INIT_TIME_OUT;
    tv.tv_usec = 0;

    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,SO_RCVTIMEO, (struct sockaddr *) &servaddr,&s_len);

    if(n!=-1)
    {   buffer[n] = '\0';
        printf("Server : %s\n", buffer);
        return 1;
    }
    else{return -1;}
}
void udp_rcv_server(struct sm_msg_arr *message)
{
    int c_len=sizeof(cliaddr);
    int  n;
    n = recvfrom(sockfd, message,sizeof(struct sm_msg_arr), MSG_WAITALL, ( struct sockaddr *) &cliaddr,  c_len);
    printf("Client ");
}
void ACK_send(char * ack){
    int len=sizeof(cliaddr);
    sendto(sockfd, (const char *) ack, strlen(ack),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
    printf(" ACK  %s message sent from server.\n",ack);
}
/*###############################################*/
//RTT and RTO estimation
struct timeval t0;
struct timeval t1;
float timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}
int NETWORK_PARAMS_INIT(){
    //int c_len=sizeof(cliaddr);
    int s_len=sizeof(servaddr);
    int num_of_try =0,n=-1,k=-1;
    char msg[10];

    gettimeofday(&t0, 0);
    sendto(sockfd, "RTT_INIT", sizeof("RTT_INIT"),MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
    while(num_of_try<NUM_OF_TRY)
    {
        gettimeofday(&t1, 0);
        if ((timedifference_msec(t0,t1) >=INIT_TIME_OUT) && (num_of_try <= NUM_OF_TRY) ) { // max timer val for retrunsmtion?? // num of trys?
            t0=(struct timeval){0};
            t1=(struct timeval){0};
            printf("\nif get in\n");
            sendto(sockfd, "RTT_INIT", sizeof("RTT_INIT"), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
            gettimeofday(&t0, 0);
        }
        n=ACK_rcv();
        if(n==1) {
            gettimeofday(&t1, 0);
            RTT = timedifference_msec(t0, t1);
            RTO =  RTT+DELTA*RTT;
            DEV=0;
            printf("RTT was init to %f milliseconds.\n", RTT);
            printf("RTO was init to %f milliseconds.\n", RTO);
            printf("Updating server...\n");
            for(int i=0;i<NUM_OF_TRY;i++) {
                sendto(sockfd, &RTT, sizeof(RTT), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
                k = ACK_rcv();
                if (k != -1) { return(1); }
            }
            printf("Updating operation fai  led! server not respond!!\n");
            return(-1);

        }
        num_of_try++;
    }
    if(num_of_try ==NUM_OF_TRY){
        printf("Failed to init RTT,server not respond!");
        exit(1);
    }
}
int RTT_init_respond() {
    // respond to RTT init msg from client
    int c_len = sizeof(cliaddr);
    int s_len = sizeof(servaddr);
    int num_of_try = 1;
    int n = -1, k = -1;
    char *RTT_ack = {"RTT_INIT_ACK"};
    char RTT_init_msg[20];

    while (n == -1) {
        n = recvfrom(sockfd, RTT_init_msg, sizeof(RTT_init_msg), MSG_WAITALL, (struct sockaddr *) &cliaddr, &c_len);
        if (n != -1) {
            printf("\n RTT_INIT msg receive from clint\n");
            sendto(sockfd, (const char *) RTT_ack, sizeof(RTT_ack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                   c_len);
            printf("%s message sent.\n", RTT_ack);
            k = recvfrom(sockfd, &RTT_SERVER, sizeof(RTT_SERVER), MSG_WAITALL, (struct sockaddr *) &cliaddr,
                         &c_len);
            if (k != -1) { printf("RTT_SERVER was init to %f millisecond...\n", RTT_SERVER); }
            sendto(sockfd, (const char *) RTT_ack, sizeof(RTT_ack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
                   c_len);
            return (1);
        }
    }
}