//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"
/*####################################################################################################################*/
//MQTT
int mqtt_publish(struct sm_msg *message)
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
                                MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to create client, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    pubmsg.payload = message->payload;
    pubmsg.payloadlen = (int)strlen(message->payload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    if ((rc = MQTTClient_publishMessage(client, message->topic, &pubmsg, &token)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to publish message, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for up to %d seconds for publication of %s\n"
           "on topic %s for client with ClientID: %s\n",
           (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);

    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS)
        printf("Failed to disconnect, return code %d\n", rc);
    MQTTClient_destroy(&client);
    return rc;
}
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


    struct msqid_ds buf;
    struct sm_msg message;
    char string[]={"SMP SYS MSG"}; // do not remove!!!!!!!
    /*int num_messages;
    rc = msgctl(msqid_global, IPC_STAT, &buf);
    num_messages = buf.msg_qnum;*/
    arr->arr_size=0;
    sqe_send_arr[0]=-1;
    sqe_send_arr[1]=-1;
    arr->sq_number = sqe_number;

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
                if(buf.msg_qnum==0){
                    usleep(TIME_TO_WAIT);
                }
                msgctl(msqid_global, IPC_STAT, &buf);
                if((arr->arr_size)==(data_arr_size) || buf.msg_qnum==0){
                    sqe_send_arr[1]=sqe_number;
                    return;
                }
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
    servaddr.sin_addr.s_addr = inet_addr(network_params.SERVER_IP);;//*/INADDR_ANY;
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
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(SERVER_PORT);

    // Bind the socket with the server address
    if (bind(server_socket, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("UDP receive socket created\n");


    memset(&cliaddr, 0, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET; // IPv4
    cliaddr.sin_addr.s_addr = /*INADDR_ANY;//*/inet_addr(network_params.CLIENT_IP);
    cliaddr.sin_port = htons(CLIENT_PORT);
    printf("UDP send socket created\n");

}
void udp_init_server()
{
    server_sockets_creation();
    server_init_respond();
}
//RTT and RTO estimation
float timedifference_msec(struct timeval b, struct timeval a)
{
    //return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
    struct timeval result;

    (result).tv_sec = (a).tv_sec - (b).tv_sec;
    (result).tv_usec = (a).tv_usec - (b).tv_usec;
    if ((result).tv_usec < 0) {
      --(result).tv_sec;
      (result).tv_usec += 1000000;
    }
    return result.tv_sec*1000.0f+result.tv_usec/1000.0f;
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
    if(gettimeofday(&t0, 0)==-1)
    {
        perror("getimeofday");
    }
    sendto(client_socket, "RTT_INIT", sizeof("RTT_INIT"),MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
    while(num_of_try<network_params.NUM_OF_TRY)
    {
        if(gettimeofday(&t1, 0)==-1)
        {
            perror("getimeofday");

        }        if ((timedifference_msec(t0,t1) >=network_params.INIT_TIME_OUT) && (num_of_try <= network_params.NUM_OF_TRY) ) { // max timer val for retransmtion?? // num of trys?
            t0=(struct timeval){0};
            t1=(struct timeval){0};
            printf("No respond from server, trying again to init RTT...\n");
            sendto(client_socket, "RTT_INIT", sizeof("RTT_INIT"), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
            if(gettimeofday(&t0, 0)==-1)
            {
                perror("getimeofday");

            }        }
       // n=ACK_rcv();
        n = recvfrom(client_socket, (char *)msg, sizeof(msg),SO_RCVTIMEO, (struct sockaddr *) &servaddr,&s_len);
        if(n!=-1) {
            if(gettimeofday(&t1, 0)==-1)
            {
                perror("getimeofday");
            }
            RTT = timedifference_msec(t0, t1);
            RTO =  RTT+network_params.DELTA*RTT;
            DEV=RTO-RTT; // ??
            client_server_params.rtt=RTT;
            client_server_params.rtt=RTO;
            printf("RTT was init to %f milliseconds.\n", RTT);
            printf("RTO was init to %f milliseconds.\n", RTO);
            printf("Sending params to server...\n");
            for(int i=0;i<network_params.NUM_OF_TRY;i++) {
                sendto(client_socket, &client_server_params, sizeof(client_server_params), MSG_CONFIRM, (const struct sockaddr *) &servaddr,s_len);
                //k = ACK_rcv();
                k = recvfrom(client_socket, (char *)msg, sizeof(msg),SO_RCVTIMEO, (struct sockaddr *) &servaddr,&s_len);
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
                k = recvfrom(server_socket, &tmp, sizeof(struct smp_client_server_params), MSG_WAITALL, (struct sockaddr *) &cliaddr,&c_len);
                if (k != -1) {

                    printf("RTT_SERVER was init to %f millisecond...\n", tmp.rtt);
                    printf("RTO_SERVER was init to %f millisecond...\n", tmp.rto);

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
       //RTO = RTT +10*abs(DEV)+0.5*RTT;
        RTO = RTT +10*DEV;

    }
}
void sequence_number_select(int * previous_sqe,int window_size,int time_to_wait_for_sequence_select)
{
    if(*previous_sqe==window_size-1)
        *previous_sqe=-1;
    while(1) {
        pthread_mutex_lock(&lock);

        if (windowcontrol[*previous_sqe + 1].status != (-1)) {
            pthread_mutex_unlock(&lock);
            usleep(time_to_wait_for_sequence_select);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            (*previous_sqe)++;
            return;
        }
    }

}
void init_params(char *file_name) {
    int j;
    char str[50];
    char c;
    int counter = -1;
    char line[75];
    FILE *config_file;
    config_file = fopen(file_name, "r");
    if (!config_file) {
        perror("fopen to read config_file.txt failed!\n");
        exit(EXIT_FAILURE);
    }
    while ((c = getc(config_file)) != EOF) {
        fgets(line, 75, config_file);
        for (int i = 0; i < 75; i++) {
            if (line[i] == ':') {
                for (int k = 0; k < 50; k++)
                    str[k] = '\0';
                counter++;
                j = 0;
                while (line[i + 1] != '\n') {
                    str[j] = line[i + 1];
                    i++;
                    j++;
                }
                switch (counter) {
                    case 0:
                        break;
                    case 1:
                        network_params.client_port = atoi(str);
                        break;
                    case 2:
                        strcpy(network_params.CLIENT_IP, str);
                        break;
                    case 3:
                        network_params.server_port = atoi(str);
                        break;
                    case 4:
                        strcpy(network_params.SERVER_IP, str);
                        break;
                    case 5:
                        network_params.bandwidth = atoi(str);
                        break;
                    case 6:
                        network_params.NUM_OF_TRY = atoi(str);
                        break;
                    case 7:
                        network_params.INIT_TIME_OUT = atof(str);
                        break;
                    case 8:
                        network_params.MAX_ALLOW_RTT = atof(str);
                        break;
                    case 9:
                        network_params.ALPHA = atof(str);
                        break;
                    case 10:
                        network_params.BETA = atof(str);
                        break;
                    case 11:
                        network_params.GAMMA = atof(str);
                        break;
                    case 12:
                        network_params.DELTA = atof(str);
                        break;
                    case 13:
                        network_params.typical_rtt=atoi(str);
                }
                break;
            }
        }
    }
    client_server_params.smp_msg_arr_size = (network_params.bandwidth / (8 * (MAX_PAYLOAD_SIZE + MAX_TOPIC_SIZE)) *network_params.typical_rtt/1000);
    client_server_params.window_size = network_params.bandwidth / (8 * (MAX_PAYLOAD_SIZE + MAX_TOPIC_SIZE));

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
        sequence_number_select(&sequence_number,client_server_params.window_size,TIME_TO_WAIT);
        message_encapsulation(&(arr[sequence_number]), client_server_params.smp_msg_arr_size, sequence_number,sqe_sender_arr);

        //gettimeofday(arg, 0);


        for(int i=0;i<2;i++)
        {
            if(sqe_sender_arr[i]!=(-1))
            {
                pthread_mutex_lock(&lock);
                sendto(client_socket, &arr[sqe_sender_arr[i]], sizeof(struct sm_msg_arr), MSG_CONFIRM,(const struct sockaddr *) &servaddr, s_len);
                windowcontrol[sqe_sender_arr[i]].status = 1;
                windowcontrol[sqe_sender_arr[i]].seq_num=sequence_number;
                if(gettimeofday(&(windowcontrol[sqe_sender_arr[i]].t), 0)==-1)
                {
                    perror("getimeofday");
                }
                pthread_mutex_unlock(&lock);

            }
        }
    }

    /*
        printf("UDP: Close Socket.\n");
        close(sockfd);
        printf("message queue: done receiving messages.\n");
        system("rm msgq.txt");*/
}
void* win_control_routine(struct timeval t0) {
    float time_diff = 0;
    char buf[10];
    struct timeval rt,a,b;
    gettimeofday(&a,0);
    int seq_loss=0;
    float tmp;
    while (1) {
        for (int i = 0; i < client_server_params.window_size; i++) {
            gettimeofday(&a,0);
            time_diff=0;
            if (gettimeofday(&rt, 0) == -1) {
                perror("getimeofday");
            }
            pthread_mutex_lock(&lock);
            if ((windowcontrol[i].status == 1) && (windowcontrol[i].seq_num != -1)) {
                time_diff = timedifference_msec(windowcontrol[i].t, rt);
                //   printf("seq.num:%d, time diff:%f\n", windowcontrol[i].seq_num, time_diff);

                        if (time_diff > RTO) {
                            printf("DEBUG: Time out occur on msg seq number: %d\n", windowcontrol[i].seq_num);
                            printf("seq.num:%d,num_of_trys:%d ,Time diff:%f, RTO:%f, RTT:%f,DEV:%f\n",windowcontrol[i].seq_num, windowcontrol[i].num_of_trys, time_diff, RTO, RTT,DEV);
                            windowcontrol[i].status = -1;
                            windowcontrol[i].t.tv_sec = 0;
                            windowcontrol[i].t.tv_usec = 0;
                            windowcontrol[i].num_of_trys++;
                            sprintf(buf, "%d", windowcontrol[i].seq_num);
                            message_queue_send(buf, SMP_SYSTEM_MESSAGE);
                            printf("DEBUG: SMP SYS MSG was sent to sender thread with seq number:%s\n", buf);
                            //printf("time_diff-RTO:%f\n",(100*(time_diff-RTO)/RTT));

                            Update_Net_Params((RTT+1000*(time_diff-RTO)/RTT)); // need to add new parameter for fast raising.
                           // gettimeofday(&b,0);
                           // printf("net param time:%f\n", timedifference_msec(a,b));

                        }

            }
            pthread_mutex_unlock(&lock);
        }
    }
}
void * receiver_routine(struct timeval t0) {
    int n;
    int s_len = sizeof(servaddr);
    struct timeval tv;
    int ack_seq=1;
    float t;
    //t1 = (struct timeval) {0};
    struct timeval rt;

    //gettimeofday(&t1, 0);
   while (1) {

        //tv.tv_sec = min_t;
        //tv.tv_usec = 0;
        //setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv,sizeof tv); // for non-blocking recvfrom calling
       n = recvfrom(client_socket, &ack_seq, sizeof(int),MSG_WAITALL, (struct sockaddr *) &servaddr,&s_len);
       pthread_mutex_lock(&lock);
       if (n != -1 && windowcontrol[ack_seq].status==1) {
            printf("ACK seq number received: %d\n",ack_seq);
           // gettimeofday(&t1, 0);

           // pthread_mutex_lock(&lock);
            if(gettimeofday(&rt, 0)==-1)
            {
                perror("getimeofday");
            }
            sampled_rtt = timedifference_msec(windowcontrol[ack_seq].t, rt);
            Update_Net_Params(sampled_rtt);
         //  printf("RECIVER RUTINE: seq.num:%d,SAMPED_RTT:%f,RTO:%f,RTT:%f,DEV:%f\n",windowcontrol[ack_seq].seq_num,sampled_rtt, RTO, RTT,DEV);
           windowcontrol[ack_seq].status = -1;
            windowcontrol[ack_seq].seq_num = -1;
            windowcontrol[ack_seq].t.tv_sec = 0;
            windowcontrol[ack_seq].t.tv_usec = 0;
            windowcontrol[ack_seq].num_of_trys=0;
            pthread_mutex_unlock(&lock);

        }
       pthread_mutex_unlock(&lock);

   }
}
