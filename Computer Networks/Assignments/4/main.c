#include "bozoons.h"


int main(int argc, char** argv){
    if (argc != 5){
        fprintf(stderr, "Usage: %s <M> <S> <Y> <duration>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int M, duration;
    float S, Y;

    // Get and check arguments
    M = atoi(argv[1]);
    if ((S = atof(argv[2])) <= 0 || (Y = atof(argv[3])) <= 0 || (duration = atoi(argv[4])) <= 0){
        fprintf(stderr, "Values must be positive and non-zero\n");
        exit(EXIT_FAILURE);
    }

    // Run the simulation
    stats_t stats;
    stats.yodeling_attempts = 0;
    stats.perfect_yodeling_attempts = 0;
    stats.perfect_yodeling_time = 0.0;
    stats.silent_time = 0.0;
    stats.melodious_time = 0.0;
    stats.screeching_time = 0.0;
    discrete_event_simulation(M, S, Y, duration, &stats);

    print_statistics(M, S, Y, duration, stats);
}