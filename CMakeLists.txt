cmake_minimum_required(VERSION 3.20)
project(SMP)
find_package( Threads )

ADD_EXECUTABLE(mqtt_subscribe_msg_que_send mqtt_subscribe_msg_que_send.c SMP_MQTT_UDP.c)
ADD_EXECUTABLE(udp_rcv_mqtt_publish udp_rcv_mqtt_publish.c SMP_MQTT_UDP.c)
ADD_EXECUTABLE(msg_rcv_que_udp_send msg_rcv_que_udp_send.c SMP_MQTT_UDP.c)
ADD_EXECUTABLE(udp_client_for_debug udp_client_for_debug.c SMP_MQTT_UDP.c)
ADD_EXECUTABLE(MQTTClient_publish MQTTClient_publish.c SMP_MQTT_UDP.c)
ADD_EXECUTABLE(message_queue_sender_for_debug message_queue_sender_for_debug.c SMP_MQTT_UDP.c)

SET_TARGET_PROPERTIES(PROPERTIES COMPILE_DEFINITIONS "PAHO_MQTT_IMPORTS=1")

TARGET_LINK_LIBRARIES(message_queue_sender_for_debug  paho-mqtt3c)
TARGET_LINK_LIBRARIES(mqtt_subscribe_msg_que_send paho-mqtt3c)
TARGET_LINK_LIBRARIES(udp_rcv_mqtt_publish ${CMAKE_THREAD_LIBS_INIT} paho-mqtt3c)
TARGET_LINK_LIBRARIES(MQTTClient_publish paho-mqtt3c)
TARGET_LINK_LIBRARIES(msg_rcv_que_udp_send ${CMAKE_THREAD_LIBS_INIT} paho-mqtt3c)


