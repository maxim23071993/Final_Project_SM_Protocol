//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"
/*####################################################################################################################*/
//message queu
void msg_que_create(char *topic)
{
    //int msqid;
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

    if ((msqid_global = msgget(key, PERMS | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
    //msqid_global=msqid;
    printf("Message Queue: created file: %s.txt\n",topic);
}
void msg_rcv_init(int* msqid,char *topic)
{
    char for_ftok[30];
    strcpy(for_ftok,topic);
    strcat(for_ftok,".txt");
    key_t key;
    if ((key = ftok(for_ftok,'B')) == -1) {
        perror("ftok");
        exit(1);
    }
    if (((*msqid) = msgget(key,PERMS)) == -1) { /* connect to the queue */

        perror("msgget");
        exit(1);
    }
    printf("Message Queue: ready to receive messages from file: %s.txt\n",topic);
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
void message_encapsulation(struct sm_msg_arr *arr,int data_arr_size,int sqe_number,int * sqe_send_arr)
{

    /*struct msqid_ds buf;
    struct sm_msg message;

    /*int num_messages;
    int rc;
    int msqid;
    rc = msgctl(msqid_global, IPC_STAT, &buf);
    num_messages = buf.msg_qnum;
    arr->arr_size=0;
    sqe_send_arr[0]=-1;
    sqe_send_arr[1]=sqe_number;
    arr->sq_number = sqe_number;

    while(1)
    {
        read_from_message_queue(&message,msqid_global);
        switch(strcmp(SMP_SYSTEM_MESSAGE,message.topic))
        {
            case 0:
                sqe_send_arr[0]=atoi(message.payload);
              // break;
                return;
            default:
               strcpy(arr->msg_arr[arr->arr_size].payload,message.payload);
               strcpy(arr->msg_arr[arr->arr_size].topic,message.topic);
               (arr->arr_size)++;
               msgctl(msqid_global, IPC_STAT, &buf);
               if((arr->arr_size)==(data_arr_size) || buf.msg_qnum==0)
                  return;
        }

    }*/
    struct msqid_ds buf;
    struct sm_msg message;
    int message_compare;
    char string[]={"SMP SYS MSG"};
    /*int num_messages;
    rc = msgctl(msqid_global, IPC_STAT, &buf);
    num_messages = buf.msg_qnum;*/
    arr->arr_size=0;
    sqe_send_arr[0]=-1;
    sqe_send_arr[1]=-1;
    arr->sq_number = sqe_number;
    sqe_send_arr[1]=sqe_number;
    while(1)
    {
        read_from_message_queue(&message,msqid_global);
        switch(strcmp(SMP_SYSTEM_MESSAGE,message.topic))
        {
            case 0:
                sqe_send_arr[0]=atoi(message.payload);
                return;

                // operator doesn't match any case constant +, -, *, /
            default:
                strcpy(arr->msg_arr[arr->arr_size].payload,message.payload);
                strcpy(arr->msg_arr[arr->arr_size].topic,message.topic);
                (arr->arr_size)++;
                msgctl(msqid_global, IPC_STAT, &buf);
                if((arr->arr_size)==(data_arr_size) || buf.msg_qnum==0)
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
    cliaddr.sin_addr.s_addr = /*inet_addr(CLIENT_IP);*/INADDR_ANY;
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
    servaddr.sin_addr.s_addr = /*inet_addr(SERVER_IP);;*/INADDR_ANY;
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
    servaddr.sin_addr.s_addr = INADDR_ANY;//*/inet_addr("192.168.1.117");
    servaddr.sin_port = htons(SERVER_PORT);

    // Bind the socket with the server address
    if (bind(server_socket, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("UDP receive socket created\n");


    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET; // IPv4
    cliaddr.sin_addr.s_addr = INADDR_ANY;//*/inet_addr("192.168.1.113");
    cliaddr.sin_port = htons(CLIENT_PORT);
    printf("UDP send socket created\n");

}
int udp_init_server()
{
    server_sockets_creation();
    server_init_respond();
}

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


    struct timeval tv;
    tv.tv_sec = network_params.INIT_TIME_OUT;
    tv.tv_usec = 0;
   setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    gettimeofday(&t0, 0);
    sendto(client_socket, "RTT_INIT", sizeof("RTT_INIT"),MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
    while(num_of_try<network_params.NUM_OF_TRY)
    {
        gettimeofday(&t1, 0);
        if ((timedifference_msec(t0,t1) >=network_params.INIT_TIME_OUT) && (num_of_try <= network_params.NUM_OF_TRY) ) { // max timer val for retransmtion?? // num of trys?
            t0=(struct timeval){0};
            t1=(struct timeval){0};
            printf("No respond from server, trying again to init RTT...\n");
            sendto(client_socket, "RTT_INIT", sizeof("RTT_INIT"), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
            gettimeofday(&t0, 0);
        }
       // n=ACK_rcv();
        n = recvfrom(client_socket, (char *)msg, MAXLINE,SO_RCVTIMEO, (struct sockaddr *) &servaddr,&s_len);
        if(n!=-1) {
            gettimeofday(&t1, 0);
            RTT = timedifference_msec(t0, t1);
            RTO =  RTT+network_params.DELTA*RTT;
            DEV=0;
            client_server_params.rtt=RTT;
            client_server_params.rtt=RTO;
            printf("RTT was init to %f milliseconds.\n", RTT);
            printf("RTO was init to %f milliseconds.\n", RTO);
            printf("Sending params to server...\n");
            for(int i=0;i<network_params.NUM_OF_TRY;i++) {
                sendto(client_socket, &client_server_params, sizeof(client_server_params), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
                //k = ACK_rcv();
                k = recvfrom(client_socket, (char *)msg, MAXLINE,SO_RCVTIMEO, (struct sockaddr *) &servaddr,&s_len);
                if (k != -1){
                return(1);
                }
            }
            printf("Updating operation failed! server not respond!!\n");
            return(-1);

        }
        num_of_try++;
    }
    if(num_of_try ==network_params.NUM_OF_TRY){
        printf("Failed to init RTT,server not respond!");
        exit(1);
    }
}
void server_init_respond() {

    // respond to init msg from client
    int c_len = sizeof(cliaddr);
    int s_len = sizeof(servaddr);
    int num_of_try = 1;
    int n = -1, k = -1;
    char *RTT_ack = {"RTT_INIT_ACK"};
    char RTT_init_msg[20];
    struct smp_client_server_params tmp;

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    if(setsockopt(server_socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv)<0)
    { perror("setsockopt failed:");}

    while (n == -1) {
        n = recvfrom(server_socket, RTT_init_msg, sizeof(RTT_init_msg),MSG_WAITALL, (struct sockaddr *) &cliaddr, &c_len);
        if (n != -1) {
            if (strcmp(RTT_init_msg, "RTT_INIT") == 0) {
                printf("RTT_INIT msg receive from clint\n");
                sendto(server_socket, (const char *) RTT_ack, sizeof(RTT_ack), MSG_CONFIRM,(const struct sockaddr *) &cliaddr, c_len);
                printf("%s message sent to client.\n", RTT_ack);
                k = recvfrom(server_socket, &tmp, sizeof(RTT_SERVER), MSG_WAITALL, (struct sockaddr *) &cliaddr,&c_len);
                if (k != -1) {
                    RTT_SERVER=tmp.rtt;
                    RTO_SERVER=tmp.rto;
                    printf("RTT_SERVER was init to %f millisecond...\n", RTT_SERVER);
                    printf("RTO_SERVER was init to %f millisecond...\n", RTO_SERVER);

                }
                sendto(server_socket, (const char *) RTT_ack, sizeof(RTT_ack), MSG_CONFIRM,
                       (const struct sockaddr *) &cliaddr, c_len);
                return;
            }
        }
        }

}
void Update_Net_Params(float SAMPLE_RTT)
{
    if(SAMPLE_RTT<=network_params.MAX_ALLOW_RTT) {  // filter for faulty RTT measurements.
        RTT = (1-network_params.ALPHA)*RTT+ network_params.ALPHA * SAMPLE_RTT;
        DEV = DEV*(1-network_params.BETA)+network_params.BETA*(abs((SAMPLE_RTT - RTT)));
        RTO = RTT + 4*abs(DEV);
    }
}
void sequence_number_select(int * previous_sqe,int window_size,int time_to_wait_for_sequence_select)
{
    if(*previous_sqe==window_size-1)
        *previous_sqe=-1;
    while(windowcontrol[*previous_sqe+1].status!=(-1))
        usleep(time_to_wait_for_sequence_select);
    (*previous_sqe)++;
}
/*####################################################################################################################*/
//Thread routine
void * sender_routine(void* arg)
{
    int s_len=sizeof(servaddr);
    int c_len=sizeof(cliaddr);
    int sequence_number=(-1);
    int sqe_sender_arr[2];
    struct sm_msg_arr  * arr= malloc(sizeof(struct sm_msg_arr)*client_server_params.window_size);
   // for(int i;i<client_server_params.window_size;i++)
     //   arr[i].msg_arr=malloc(sizeof(struct sm_msg)*client_server_params.smp_msg_arr_size);
    //struct sm_msg_arr   arr[11];


    while(1)
    {
        sequence_number_select(&sequence_number,client_server_params.window_size,TIME_TO_WAIT_FOR_WINDOW);
        message_encapsulation(&(arr[sequence_number]), client_server_params.smp_msg_arr_size, sequence_number,sqe_sender_arr);

        //gettimeofday(arg, 0);

        pthread_mutex_lock(&lock);

        for(int i=0;i<2;i++)
        {
            if(sqe_sender_arr[i]!=(-1))
            {
                sendto(client_socket, &arr[sqe_sender_arr[i]], sizeof(struct sm_msg_arr), MSG_CONFIRM,(const struct sockaddr *) &servaddr, s_len);
                windowcontrol[sqe_sender_arr[i]].status = 1;
                windowcontrol[sqe_sender_arr[i]].seq_num=sequence_number;
                gettimeofday(&(windowcontrol[sqe_sender_arr[i]].t), 0);
            }
        }
        pthread_mutex_unlock(&lock);
    }

    /*
        printf("UDP: Close Socket.\n");
        close(sockfd);
        printf("message queue: done receiving messages.\n");
        system("rm msgq.txt");*/
}
void * receiver_routine(struct timeval t0) {
    int n;
    int s_len = sizeof(servaddr);
    struct timeval tv;
    int ack_seq=1;
    float t;
    float min_t = 0, time_diff = 0;
    char buf[10];
    t1 = (struct timeval) {0};
    //gettimeofday(&t1, 0);
   while (1) {
        pthread_mutex_lock(&lock);
        for (int i = 0; i < 10; i++) {
            gettimeofday(&t1, 0);
            if (windowcontrol[i].status == 1) {
                time_diff = timedifference_msec(windowcontrol[i].t, t1);
                printf("seq.num:%d, time diff:%f\n", windowcontrol[i].seq_num, time_diff);
            }
                if (time_diff < min_t) {
                    min_t = time_diff;
                }
                if (time_diff >= RTO && windowcontrol[i].seq_num!=-1) {
                    printf("DEBUG: Time out occur on msg seq number: %d\n", windowcontrol[i].seq_num);
                    printf("seq.num:%d,Time diff:%f, RTO:%f, RTT:%f\n",windowcontrol[i].seq_num,time_diff,RTO,RTT);
                    windowcontrol[i].status = -1;
                    windowcontrol[i].t.tv_sec=0;
                    windowcontrol[i].t.tv_usec=0;
                    sprintf(buf, "%d", windowcontrol[i].seq_num);
                    message_queue_send(buf,SMP_SYSTEM_MESSAGE);
                    printf("DEBUG: SMP SYS MSG was sent to sender thread with seq number:%s\n", buf);
                }
            }
        pthread_mutex_unlock(&lock);
        tv.tv_sec = min_t;
        tv.tv_usec = 0;
        setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv,sizeof tv); // for non-blocking recvfrom calling
        n = recvfrom(client_socket, &ack_seq, MAXLINE,SO_RCVTIMEO, (struct sockaddr *) &servaddr,&s_len);
        if (n != -1) {
            printf("ACK seq number received: %d\n",ack_seq);
            gettimeofday(&t1, 0);

            pthread_mutex_lock(&lock);

            sampled_rtt = timedifference_msec(windowcontrol[ack_seq].t, t1)/10;
            printf("RTT sample: %f, RTT:%f,RTO:%f\n",sampled_rtt,RTT,RTO);
            Update_Net_Params(sampled_rtt);

            windowcontrol[ack_seq].status = -1;
            windowcontrol[ack_seq].seq_num = -1;
            windowcontrol[ack_seq].t.tv_sec = 0;
            windowcontrol[ack_seq].t.tv_usec = 0;

            pthread_mutex_unlock(&lock);

        }
    }
}
void init_params(char *file_name)
{
    int j;
    char str[50];
    char c;
    int counter=-1;
    char line[75];
    FILE* config_file;
    config_file = fopen(file_name, "r");
    if (!config_file)
    {
        perror("fopen to read config_file.txt failed!\n");
        exit(EXIT_FAILURE);
    }
    while (( c = getc(config_file)) != EOF)
    {
        fgets(line, 75, config_file);
        for(int i=0;i<75;i++)
        {
            if (line[i] == ':')
            {
                for(int k=0;k<50;k++)
                    str[k]='\0';
                counter++;
                j=0;
                while(line[i+1]!='\n')
                {
                    str[j] = line[i + 1];
                    i++;
                    j++;
                }
                switch (counter)
                {
                    case 0:
                        break;
                    case 1:
                        network_params.client_port = atoi(str) ;
                        break;
                    case 2:
                        strcpy(network_params.CLIENT_IP,str);
                        break;
                    case 3:
                        network_params.server_port = atoi(str);
                        break;
                    case 4:
                        strcpy(network_params.SERVER_IP,str);
                        break;
                    case 5:
                        network_params.bandwidth= atoi(str);
                        break;
                    case 6:
                        network_params.NUM_OF_TRY= atoi(str);
                        break;
                    case 7:
                        network_params.INIT_TIME_OUT= atof(str);
                        break;
                    case 8:
                        network_params.MAX_ALLOW_RTT= atof(str);
                        break;
                    case 9:
                        network_params.ALPHA= atof(str);
                        break;
                    case 10:
                        network_params.BETA= atof(str);
                        break;
                    case 11:
                        network_params.GAMMA= atof(str);
                        break;
                    case 12:
                        network_params.DELTA= atof(str);
                        break;
                }
                break;
            }
        }
    }
    client_server_params.smp_msg_arr_size=(network_params.bandwidth/(8*(MAX_PAYLOAD_SIZE+MAX_TOPIC_SIZE))*0.5);
    client_server_params.window_size=network_params.bandwidth/(8*(MAX_PAYLOAD_SIZE+MAX_TOPIC_SIZE));

}
