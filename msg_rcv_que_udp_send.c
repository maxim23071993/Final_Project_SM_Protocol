#include "SMP_MQTT_UDP.h"
    int main()
    {
        char c;
        pthread_t sender_thread,receiver_thread;
        msg_rcv_init(&msqid_global);
        udp_init_client();
        t0=(struct timeval){0};
        gettimeofday(&t0, 0);
        pthread_create(&sender_thread,NULL,&sender_routine,&t0);
        pthread_create(&receiver_thread,NULL,&receiver_routine,&t0);
        while ( ( c = getchar() ) != EOF );
        return 0;
    }
