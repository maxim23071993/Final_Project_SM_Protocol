//
// Created by Maxim & Shlomi on 24/08/2021.
//
#include "SMP_MQTT_UDP.h"


int main()
{
    struct sm_msg_arr *arr = malloc(client_server_params.window_size* sizeof(struct sm_msg_arr));
    char file_name[100] = {"//home//max//Desktop//MQTT Subscribe+msg_que+udp_git//SMP_PARAMS.txt"};

    init_params(file_name);
    udp_init_server();

    pthread_t server_receiver_thread,server_mqtt_publish_thread;

    pthread_mutex_init(&server_lock, NULL);

    pthread_create((pthread_t *) &server_receiver_thread, NULL, (void *) &server_receive_routine, arr);
    pthread_create((pthread_t *) &server_mqtt_publish_thread, NULL, (void *) &server_mqtt_publish_routine, arr);

    printf("Server waiting for message.\n");

    pthread_join(server_receiver_thread, NULL);
    pthread_join(server_mqtt_publish_thread, NULL);



}
