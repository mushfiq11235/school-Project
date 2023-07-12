#include "accesspoint.h"

int main(int argc, char** argv){
    if (argc != 2){
        printf("Usage: ./%s <tracefile>\n", argv[0]);
        return EXIT_FAILURE;
    }
    FILE* tracefile = fopen(argv[1], "r");
    if (tracefile == NULL){
        fprintf(stderr, "Error: file not found\n");
        exit(EXIT_FAILURE);
    }
    QoS qos = initialize(100, tracefile, 100, 5);

    run_QoS(&qos);
    print_statistics(qos);

    fclose(tracefile);
    destroy_queue(qos.queue);

    return EXIT_SUCCESS;
}