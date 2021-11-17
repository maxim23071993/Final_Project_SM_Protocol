//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"
/*####################################################################################################################*/
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
void message_encapsulation(struct sm_msg_arr *arr,int data_arr_size,int sqe_number)
{
    int rc;
    struct msqid_ds buf;
    struct sm_msg message;
    /*int num_messages;
    rc = msgctl(msqid_global, IPC_STAT, &buf);
    num_messages = buf.msg_qnum;*/
    while(strcmp("SMP SYS MSG",message.topic))
    {
        read_from_message_queue(&message,msqid_global);
        switch(strcmp("SMP SYS MSG",message.topic))
        {
            case 0:
                WINDOW_CONTROL[atoi(message.payload)]=0;
                WINDOW_CONTROL[sqe_number]=0;
                return;

                // operator doesn't match any case constant +, -, *, /
            default:
              strcpy(arr[sqe_number].msg_arr[arr[sqe_number].arr_size].payload,message.payload);
              strcpy(arr[sqe_number].msg_arr[arr[sqe_number].arr_size].topic,message.topic);
              arr[sqe_number].arr_size++;
              if((arr[sqe_number].arr_size)==10)
                  return;

        }
    }
}
/*####################################################################################################################*/
//udp
void client_sockets_creation()
{
   if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed!");
        exit(EXIT_FAILURE);
    }
    memset(&cliaddr, 0, sizeof(cliaddr));

    //Filling server information
    cliaddr.sin_family = AF_INET; // IPv4
    cliaddr.sin_addr.s_addr = INADDR_ANY;
    cliaddr.sin_port = htons(CLIENT_PORT);


    // Bind the socket with the server address
    //if (bind(client_receive_socket, (const struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0)
    if (bind(client_socket, (const struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0)

    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP receive socket created\n");

    memset(&servaddr, 0, sizeof(servaddr));
    //Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;//inet_addr(SERVER_IP);
    servaddr.sin_port = htons(SERVER_PORT);
    printf("UDP send socket created\n");

}
void udp_init_client()
{
    client_sockets_creation();
    NETWORK_PARAMS_INIT();
}
void server_sockets_creation()
{
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed!");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));


    //Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.1.117");
    servaddr.sin_port = htons(SERVER_PORT);

    // Bind the socket with the server address
    if (bind(server_socket, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("UDP receive socket created\n");


    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET; // IPv4
    cliaddr.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.1.108");
    cliaddr.sin_port = htons(CLIENT_PORT);
    printf("UDP send socket created\n");

}
int udp_init_server()
{
    server_sockets_creation();
    RTT_init_respond();
}
int ACK_rcv()
{
    int s_len=sizeof(servaddr);
    int n;
    char buffer[MAXLINE];
    struct timeval tv;
    tv.tv_sec = INIT_TIME_OUT;
    tv.tv_usec = 0;

    //setsockopt(client_receive_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

   // n = recvfrom(client_receive_socket, (char *)buffer, MAXLINE,SO_RCVTIMEO, (struct sockaddr *) &servaddr,&s_len);
    n = recvfrom(client_socket, (char *)buffer, MAXLINE,MSG_WAITALL, (struct sockaddr *) &servaddr,&s_len);


    if(n!=-1)
    {   buffer[n] = '\0';
        printf("Server : %s\n", buffer);
        return 1;
    }
    else{return -1;}
}
void ACK_send(char * ack){
    int len=sizeof(cliaddr);
    sendto(server_socket, (const char *) ack, strlen(ack),MSG_CONFIRM, (const struct sockaddr *) &cliaddr,len);
    printf(" ACK  %s message sent from server.\n",ack);
}
/*####################################################################################################################*/
//RTT and RTO estimation
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
    sendto(client_socket, "RTT_INIT", sizeof("RTT_INIT"),MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
    while(num_of_try<NUM_OF_TRY)
    {
        gettimeofday(&t1, 0);
        if ((timedifference_msec(t0,t1) >=INIT_TIME_OUT) && (num_of_try <= NUM_OF_TRY) ) { // max timer val for retransmtion?? // num of trys?
            t0=(struct timeval){0};
            t1=(struct timeval){0};
            printf("\nif get in\n");
            sendto(client_socket, "RTT_INIT", sizeof("RTT_INIT"), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
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
                sendto(client_socket, &RTT, sizeof(RTT), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
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
        n = recvfrom(server_socket, RTT_init_msg, sizeof(RTT_init_msg), MSG_WAITALL, (struct sockaddr *) &cliaddr, &c_len);
        if (n != -1) {
            printf("\n RTT_INIT msg receive from clint\n");
            sendto(server_socket, (const char *) RTT_ack, sizeof(RTT_ack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,c_len);
            printf("%s message sent.\n", RTT_ack);
            k = recvfrom(server_socket, &RTT_SERVER, sizeof(RTT_SERVER), MSG_WAITALL, (struct sockaddr *) &cliaddr,&c_len);
            if (k != -1) { printf("RTT_SERVER was init to %f millisecond...\n", RTT_SERVER); }
            sendto(server_socket, (const char *) RTT_ack, sizeof(RTT_ack), MSG_CONFIRM, (const struct sockaddr *) &cliaddr,c_len);
            return (1);
        }
    }
}