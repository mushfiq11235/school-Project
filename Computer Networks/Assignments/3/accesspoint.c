#include "accesspoint.h"

// This initializes the QoS struct with the given parameters.

QoS initialize(int capacity, FILE* tracefile, int speed_internet, int speed_wlan){
    QoS qos;
    qos.queue = create_queue(capacity);
    qos.end = false;
    qos.start_time = clock();
    qos.last_packet_arrival_time = 0;
    qos.sum_delay = 0;
    qos.tracefile = tracefile;
    qos.capacity = capacity;
    qos.speed_internet = speed_internet;
    qos.speed_wlan = speed_wlan;
    qos.packets_dropped = 0;
    qos.packets_dropped_size = 0;
    qos.packets_served = 0;
    qos.packets_served_size = 0;
    qos.packets_total = 0;
    qos.packets_total_size = 0;
    qos.average_delay = 0;  
    qos.loss_rate = 0;
    return qos;
}

//-----------------------------------------------------------------------------
//---------------------STRUCTURE FOR QUEUE FIFO--------------------------------
//-----------------------------------------------------------------------------

queue_t * create_queue(int size){
    queue_t *queue = malloc(sizeof(queue_t));
    if (queue == NULL){
        fprintf(stderr, "Error: malloc failed\n");
        exit(EXIT_FAILURE);
    }
    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;
    return queue;
}

queue_t* enqueue(queue_t *queue, packet_t packet){
    node_t *node = malloc(sizeof(node_t));
    if (node == NULL){
        fprintf(stderr, "Error: memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    node->packet = packet;
    node->next = NULL;
    if (is_empty(queue)){
        queue->first = node;
        queue->last = node;
    }
    else{
        queue->last->next = node;
        queue->last = node;
    }
    queue->size++;
    return queue;
}

queue_t* dequeue(queue_t *queue){
    if (is_empty(queue)){
        return NULL;
    }
    node_t *node = queue->first;
    queue->first = node->next;
    queue->size--;
    if (queue->size == 0){
        queue->last = NULL;
    }
    free(node);
    return queue;
}

packet_t headq(queue_t *queue){
    if (is_empty(queue)){
        fprintf(stderr, "Error: queue is empty\n");
        exit(EXIT_FAILURE);
    }
    return queue->first->packet;
}


void destroy_queue(queue_t *queue){
    node_t *node = queue->first;
    while (node != NULL){
        node_t *next = node->next;
        free(node);
        node = next;
    }
    free(queue);
}

bool is_empty(queue_t *queue){return queue->size == 0;}
bool is_full(queue_t *queue, int max_size){return queue->size == max_size;}

//-----------------------------------------------------------------------------
//--------------------QUALITY OF SERVICE---------------------------------------
//-----------------------------------------------------------------------------

void run_QoS(QoS *qos){
    // Arguments for time calculations
    float time_elapsed = 0;
    float delta_time = 0.0001;
    float epsilon = 0.00001;

    // Arguments for packet arrival
    packet_t extracted;
    float diff_time_extracted = 0;
    bool can_extract = true;

    // Arguments for packet departure
    packet_t served;
    float transmit_time = 0;
    float diff_time_served = 0;
    bool can_served = true;

    // Main loop until the end of the tracefile and the queue is empty
    while(!(qos->end && is_empty(qos->queue))){
        // printf("elapsed time: %f - capacity : %d - Served : %d - Dropped : %d\n", time_elapsed, qos->capacity, qos->packets_served, qos->packets_dropped);
       
        //-------------------------PART 1: PACKET ARRIVAL-------------------------
        // When there is a packet to extract
        if (!qos->end){
            // If we can extract from file
            if (can_extract){
                // Extract the packet from the tracefile
                if (fscanf(qos->tracefile, "%f %d", &extracted.arrival_time, &extracted.size) != EOF){
                    extracted.id = qos->packets_total;
                    qos->packets_total++;
                    qos->packets_total_size += extracted.size;
                }
                else{
                    // If we reach the end of the tracefile
                    qos->end = true;
                    qos->last_packet_arrival_time = extracted.arrival_time;
                }
                // Cant extract anymore
                can_extract = false;
            }
            
            // If it is time to put in the queue
            if (extracted.arrival_time - time_elapsed < epsilon){
                // If the queue is not full
                if (!is_full(qos->queue, qos->capacity)){
                    // Put the packet in the queue
                    // extracted.start_queue_time = clock();
                    extracted.start_queue_time = time_elapsed;
                    // printf("in -- id : %d - start : %ld\n", extracted.id, extracted.start_queue_time);
                    enqueue(qos->queue, extracted);
                }
                else{
                    // If the queue is full we drop the packet
                    qos->packets_dropped++;
                    qos->packets_dropped_size += extracted.size;
                }
                // Can extract again
                can_extract = true;
            }
        }

        //-------------------------PART 2: PACKET DEPARTURE-------------------------
        // If the queue is not empty
        if (!is_empty(qos->queue)){
            // If we extract from the queue
            if (can_served){
                served = headq(qos->queue);
                // clock_t end_queue_time = clock();
                float end_queue_time = time_elapsed;
                // float elapsed_in_queue = (float)(end_queue_time - served.start_queue_time) / CLOCKS_PER_SEC;
                float elapsed_in_queue = end_queue_time - served.start_queue_time;
                qos->sum_delay += elapsed_in_queue;
                // printf("out -- id : %d - start : %ld - end : %ld\n", served.id, served.start_queue_time, end_queue_time);
                transmit_time = time_elapsed +  (float)(served.size * 8) / (qos->speed_wlan*1000*1000);
            }
            diff_time_served = transmit_time - time_elapsed;
            // printf("diff_time_served : %f - transmit : %f - elapsed : %f\n", diff_time_served, transmit_time, time_elapsed);
            // If it is time to serve the packet
            if (diff_time_served < epsilon){
                can_served = true;
                // free a place in the queue
                dequeue(qos->queue);
                qos->packets_served++;
                qos->packets_served_size += served.size;
            }
            else{
                can_served = false;
            }
        }
        time_elapsed += delta_time;
    }
    qos->end_time = clock();
}

float get_average_delay(QoS qos){
    return (qos.packets_served == 0)? 0: qos.sum_delay / qos.packets_served;
}

float get_loss_rate(QoS qos){
    float loss_rate = (float)qos.packets_dropped / qos.packets_total;
    return loss_rate * 100.0;
}
void print_statistics(QoS qos){
    int wls = qos.speed_wlan * 1000 * 1000;
    float avg_delay = get_average_delay(qos);
    float loss_rate = get_loss_rate(qos);
    float loss_bytes_rate = ((float)qos.packets_dropped_size / qos.packets_total_size) * 100.0;
    printf("Statistics:\n");
    printf("AP Buffer size: %d packets\n", qos.capacity);
    printf("Wireless Link Speed: %d bps\n", wls);
    // printf("End time: %f sec\n", (float)(qos.end_time - qos.start_time) / CLOCKS_PER_SEC);
    printf("End time: %f sec\n", qos.last_packet_arrival_time);
    printf("Incoming traffic: %d pkts - %d bytes\n", qos.packets_total, qos.packets_total_size);
    printf("Outgoing traffic: %d pkts - %d bytes\n", qos.packets_served, qos.packets_served_size);
    printf("Discarded traffic: %d pkts - %d bytes\n", qos.packets_dropped, qos.packets_dropped_size);
    printf("Loss traffic: %.3f%% - %.3f%%\n", loss_rate, loss_bytes_rate);
    printf("Average queuing delay: %f sec\n", avg_delay);
    printf("Summary: %d %d %d %d %d %d %d %d %f %f %f\n", 
            wls, 
            qos.capacity, 
            qos.packets_total, qos.packets_total_size, 
            qos.packets_served, qos.packets_served_size,
            qos.packets_dropped, qos.packets_dropped_size,
            loss_rate, loss_bytes_rate, avg_delay); 
};



