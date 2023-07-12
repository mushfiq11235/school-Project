#ifndef __ACCESSPOINT_H__
#define __ACCESSPOINT_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

//-----------------------------------------------------------------------------
//---------------------STRUCTURE FOR PACKET-------------------------------------
//-----------------------------------------------------------------------------
typedef struct{
    int id;
    float arrival_time; // time of arrival
    // clock_t start_queue_time; // time of start in queue
    float start_queue_time; // time of start in queue
    int size; // size of packet in bytes
    bool served; // true if packet has been served

} packet_t;


//-----------------------------------------------------------------------------
//---------------------STRUCTURE FOR QUEUE FIFO--------------------------------
//-----------------------------------------------------------------------------
typedef struct node{
    packet_t packet;
    struct node *next;
}node_t;

typedef struct{
    node_t *first;
    node_t *last;
    int size;
}queue_t;

queue_t * create_queue(int size);
queue_t* enqueue(queue_t *queue, packet_t packet);
queue_t* dequeue(queue_t *queue);
packet_t headq(queue_t *queue);
bool is_empty(queue_t *queue);
bool is_full(queue_t *queue, int max_size);
void destroy_queue(queue_t *queue);


//-------------------------------------------------------------------------------
//--------------------Quality of Service-----------------------------------------
//-------------------------------------------------------------------------------
typedef struct{
    bool end;
    queue_t* queue;
    FILE* tracefile;

    // parameters
    int capacity;
    int speed_internet;
    int speed_wlan;

    // statistics
    clock_t start_time;
    clock_t end_time;
    float last_packet_arrival_time;
    int packets_total;
    int packets_total_size;
    int packets_dropped;
    int packets_dropped_size;
    int packets_served;
    int packets_served_size;

    // QoS
    float sum_delay;
    float average_delay;
    float loss_rate;
}QoS;

QoS initialize(int capacity, FILE* tracefile, int speed_internet, int speed_wlan);
void run_QoS(QoS* qos);
float get_average_delay(QoS qos);
float get_loss_rate(QoS qos);
void print_statistics(QoS qos);


#endif