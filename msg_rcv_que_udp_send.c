//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"
    void main(int argc,void **argv)
    {
        char c;
        char file_name[100]={"//home//max//Desktop//MQTT Subscribe+msg_que+udp_git//SMP_PARAMS.txt"};
        init_params(file_name);
        struct window_control * windowc_ontrol=malloc(client_server_params.window_size*(sizeof(struct window_control)));
        windowcontrol=windowc_ontrol;
        pthread_t sender_thread,receiver_thread,win_control_thread,throuput_thread;
        pthread_mutex_init(&lock,NULL);
        pthread_mutex_init(&throughput_counters_lock,NULL);
        msg_rcv_init(&msqid_global,"msgq");
        udp_init_client();
        t0=(struct timeval){0};
        gettimeofday(&t0, 0);
        for(int i=0;i<client_server_params.window_size;i++)
        {
            windowcontrol[i].status=-1;
            windowcontrol[i].seq_num=-1;
            windowcontrol[i].num_of_trys=0;
        }
        messege_resend_counter=0;
        messege_send_counter=0;

        pthread_create((pthread_t *)&sender_thread,NULL,(void *)&client_sender_routine,&t0);
        pthread_create((pthread_t *)&receiver_thread,NULL,(void *)&client_receive_routine,&t0);
        pthread_create((pthread_t *)&win_control_thread,NULL,(void *)&client_win_control_routine,&t0);
        pthread_create((pthread_t *)&throuput_thread,NULL,(void *)&throughput_calculation_routine,NULL);

        pthread_join(sender_thread,NULL);
        pthread_join(receiver_thread,NULL);
        pthread_join(win_control_thread,NULL);
        pthread_join(throuput_thread,NULL);

       // while ( ( c = getchar() ) != EOF );
       // return 0;
    }
