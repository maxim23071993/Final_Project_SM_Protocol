#include "SMP_MQTT_UDP.h"
    int main()
    {
        char c;
        char file_name[100]={"//home//max//Desktop//MQTT Subscribe+msg_que+udp_git//SMP_PARAMS.txt"};
        init_params(file_name);
        pthread_t sender_thread,receiver_thread;
        pthread_mutex_init(&lock,NULL);
        //msg_que_create("msgq"); ///
        msg_rcv_init(&msqid_global,"msgq");
        udp_init_client();
        t0=(struct timeval){0};
        gettimeofday(&t0, 0);
        for(int i=0;i<10;i++)
        {
            windowcontrol[i].status=-1;
            windowcontrol[i].seq_num=-1;

        }
        pthread_create((pthread_t *)&sender_thread,NULL,(void *)&sender_routine,&t0);
        pthread_create((pthread_t *)&receiver_thread,NULL,(void *)&receiver_routine,&t0);
        pthread_join(sender_thread,NULL);
        pthread_join(receiver_thread,NULL);

        while ( ( c = getchar() ) != EOF );
        return 0;
    }
