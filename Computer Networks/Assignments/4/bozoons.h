#ifndef __BOZONS_H__
#define __BOZONS_H__

#include <stdio.h>
#include <stdlib.h>

#define SLEEPING 0
#define YODELING 1
#define END 2
#define INTERVAL_SIZE 100000000

typedef struct {
    int id;
    int state;
    float end_time;
    float start_time;
} bozon_t;

typedef struct{
    int event_type;
    bozon_t* bozon;
    float start_time;
} event_t;

typedef struct{
    float silent_time;
    float melodious_time;
    float screeching_time;
    float perfect_yodeling_time;
    int yodeling_attempts;
    int perfect_yodeling_attempts;
}stats_t;

void discrete_event_simulation(int M, float S, float Y, float duration, stats_t* stats);
void print_statistics(
    int M, 
    float S, 
    float Y, 
    float duration, 
    stats_t stats);

#endif