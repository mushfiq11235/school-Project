#include "bozoons.h"
#include <math.h>
#include <time.h>

// This function returns a random number from an exponential distribution with mean 1/lambda
float random_exponential(float lambda){
    double u = (double)rand() / (double)RAND_MAX;
    return -log(1- u) / lambda;
}

// This function calculates percentages 
float percent(float a, float b){
    return a / b * 100;
}

// This function shows the statistics of the simulation
void print_statistics(
    int M, 
    float S, 
    float Y, 
    float duration, 
    stats_t stats)
{
    float silent_time = stats.silent_time;
    float melodious_time = stats.melodious_time;
    float screechy_time = stats.screeching_time;
    int yodeling_attempts = stats.yodeling_attempts;
    int perfect_yodeling_attempts = stats.perfect_yodeling_attempts;
    
    fprintf(stdout, "M = %d, S = %.3f, Y = %.3f\n", M, S, Y);
    fprintf(stdout, "Total time observing channel = %.3f\n", duration);
    fprintf(stdout, "   Idle time on the channel: %.3f     %.3f%%\n", silent_time, percent(silent_time, duration));
    fprintf(stdout, "   Melodious time on the channel: %.3f     %.3f%%\n", melodious_time, percent(melodious_time, duration));
    fprintf(stdout, "   Screech time on the channel: %.3f       %.3f%%\n", screechy_time, percent(screechy_time, duration));
    fprintf(stdout, "\n");
    fprintf(stdout, "   Attempted yodels: %d\n", yodeling_attempts);
    fprintf(stdout, "   Perfect yodels: %d\n", perfect_yodeling_attempts);
    fprintf(stdout, "   Perfect yodels/Attempted yodels: %.3f (%.3f%%)\n", (float)perfect_yodeling_attempts/(float)yodeling_attempts, percent(perfect_yodeling_attempts, yodeling_attempts));
    fprintf(stdout, "   Perfect yodel time on the channel: %.3f (%.3f%%)\n", stats.perfect_yodeling_time, percent(stats.perfect_yodeling_time, duration));
}

// This function initializes a bozon
void bozon_init(bozon_t* bozon, int id, float time){
    bozon->id = id;
    bozon->state = SLEEPING;
    bozon->end_time = time;
    bozon->start_time = 0;
}

// This function return number of bozons yodeling
int number_yodeling(bozon_t* colony, int M){
    int num = 0;
    for (int i = 0; i < M; i++){
        if (colony[i].state == YODELING){
            num++;
        }
    }
    return num;
}

void start_to_sleep(event_t event, float current_time, bozon_t* last_bozon, bozon_t* colony, int M, float lambda_S, stats_t* stats){
    // Get how many time passed since last bozon
    float diff_from_last = current_time - last_bozon->start_time;

    // Update statistics
    switch (number_yodeling(colony, M))
    {
        case 0:
            break;
        case 1:
            // If there is only one bozon yodeling, it is a melodious time
            stats->melodious_time += diff_from_last;
            if (last_bozon->id == event.bozon->id){
                // If the bozon is the last bozon yodeling, it is a perfect yodel
                stats->perfect_yodeling_attempts++;
                stats->perfect_yodeling_time += diff_from_last;
            }
            break;
        default:
            // If there are more than one bozons yodeling, it is a screech time
            stats->screeching_time += diff_from_last;
            break;
    }

    // Update state of system
    bozon_t* bozon = event.bozon;
    bozon->state = SLEEPING;
    float sleeping_time = random_exponential(lambda_S);
    bozon->end_time += sleeping_time;
    bozon->start_time = current_time;

    last_bozon->id = bozon->id;
    last_bozon->start_time = bozon->start_time;
    last_bozon->end_time = bozon->end_time;
}

void start_to_yodel(event_t event, float current_time, bozon_t* last_bozon, bozon_t* colony, int M, float lambda_Y, stats_t* stats){
    // Get how many time passed since last bozon
    float diff_from_last = current_time - last_bozon->start_time;
    // Update statistics
    stats->yodeling_attempts++;
    switch (number_yodeling(colony, M)){
        case 0:
            // If there is no bozon yodeling, it is a silent time
            stats->silent_time += diff_from_last;
            break;
        case 1:
            // If there is only one bozon yodeling, it is a melodious time
            stats->melodious_time += diff_from_last;
            break;
        default:
            // If there are more than one bozons yodeling, it is a screech time
            stats->screeching_time += diff_from_last;
            break;
    }

    // Update state of system
    bozon_t* bozon = event.bozon;
    bozon->state = YODELING;
    float yodeling_time = random_exponential(lambda_Y);
    bozon->end_time += yodeling_time;
    bozon->start_time = current_time;

    last_bozon->id = bozon->id;
    last_bozon->start_time = bozon->start_time;
    last_bozon->end_time = bozon->end_time;
}

void end_simulation(float current_time, bozon_t* last_bozon, bozon_t* colony, int M, stats_t* stats){
    // Get how many time passed since last bozon
    float diff_from_last = current_time - last_bozon->start_time;
    switch (number_yodeling(colony, M)){
        case 0:
            // If there is no bozon yodeling, it is a silent time
            stats->silent_time += diff_from_last;
            break;
        case 1:
            // If there is only one bozon yodeling, it is a melodious time
            stats->melodious_time += diff_from_last;
            break;
        default:
            // If there are more than one bozons yodeling, it is a screech time
            stats->screeching_time += diff_from_last;
            break;
    }
}

// This function returns the next event
event_t find_next_event(bozon_t* colony, int M, int duration){
    float min_time = duration;
    event_t next_event;
    next_event.start_time = min_time;
    int index = -1;
    // Take the minimum time of all bozons
    for (int i = 0; i < M; i++){
        if (colony[i].end_time < min_time){
            index = i;
            min_time = colony[i].end_time;
        }
    }
    // If it exceeds the duration, it is the end of the simulation
    if (min_time >= duration){
        next_event.event_type = END;
        next_event.start_time = min_time;
    }
    else{
        next_event.bozon = &colony[index];
        next_event.event_type = colony[index].state;
        next_event.start_time = min_time;
    }
    return next_event;
} 

void discrete_event_simulation(int M, float S, float Y, float duration, stats_t* stats){
    srand(time(NULL));
    float lambda_S = 1 / S; // Lambda for sleeping time
    float lambda_Y = 1 / Y; // Lambda for yodeling time

    // Args
    bozon_t last_bozon;
    bozon_init(&last_bozon, -42, 0);

    // All bozons are asleep at the beginning
    bozon_t* colony = malloc(sizeof(bozon_t) * M);
    for (int i = 0; i < M; i++){
        float s = random_exponential(lambda_S);
        bozon_init(&colony[i], i, s);
    }

    // Start the simulation
    float current_time = 0.0;
    event_t next_event;
    while (current_time < duration) {
        // Find the next event
        next_event = find_next_event(colony, M, duration);

        // Update the time
        current_time = next_event.start_time;


        switch (next_event.event_type)
        {
            case SLEEPING:
                start_to_yodel(next_event, current_time, &last_bozon, colony, M, lambda_Y, stats);
                break;
            case YODELING:
                start_to_sleep(next_event, current_time, &last_bozon, colony, M, lambda_S, stats);
                break;
            case END:
                end_simulation(current_time, &last_bozon, colony, M, stats);
                break;
            default:
                break;
        }
    }
    free(colony);
}