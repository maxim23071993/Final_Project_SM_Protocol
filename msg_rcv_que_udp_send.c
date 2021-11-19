#include "SMP_MQTT_UDP.h"
    int main()
    {
        char c;
        pthread_t sender_thread,receiver_thread;
        msg_rcv_init(&msqid_global);
        udp_init_client();
        pthread_create(&sender_thread,NULL,&sender_routine,NULL);
        pthread_create(&receiver_thread,NULL,&receiver_routine,NULL);
        while ( ( c = getchar() ) != EOF );
        return 0;
    }
