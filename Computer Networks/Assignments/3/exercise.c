#include "accesspoint.h"

void change_wls(int wls, FILE* tracefile_soccer, FILE* tracefile_zoom){
    QoS q1, q2;
    q1 = initialize(100, tracefile_soccer, 100, wls);
    printf("Soccer stats with WLAN = %d\n", wls);
    run_QoS(&q1);
    print_statistics(q1);

    q2 = initialize(100, tracefile_zoom, 100, wls);
    printf("\nZoom stats with WLAN = %d\n", wls);
    run_QoS(&q2);
    print_statistics(q2);
    printf("\n");

    destroy_queue(q1.queue);
    destroy_queue(q2.queue);
}

int main(int argc, char** argv){
    if (argc != 2){
        printf("Usage: ./%s <num_question>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int num_question = atoi(argv[1]);
    if (num_question < 1 || num_question > 4){
        printf("Error: num_question must be between 1 and 4\n");
        return EXIT_FAILURE;
    }
    FILE* tracefile_soccer = fopen("soccer.txt", "r");
    FILE* tracefile_zoom = fopen("zoom.txt", "r");
    if (tracefile_soccer == NULL || tracefile_zoom == NULL){
        fprintf(stderr, "Error: file not found\n");
        exit(EXIT_FAILURE);
    }
    //-------------------------------------------------------------------------
    //------------------------- FIRST QUESTION --------------------------------
    //-------------------------------------------------------------------------
    if (num_question == 1){
        QoS q1, q2;
        FILE* output_soccer = fopen("soccer.csv", "w"); // The output file for soccer
        FILE* output_zoom = fopen("zoom.csv", "w"); // The output file for zoom
        if (output_soccer == NULL || output_zoom == NULL){
            fprintf(stderr, "Error: could not open file %s\n", argv[1]);
            return EXIT_FAILURE;
        }
        
        for (int capacity = 0; capacity <= 1000; capacity += 100){
            printf("\nFor loop with capacity = %d\n", capacity);
            q1 = initialize(capacity, tracefile_soccer, 100, 5);
            run_QoS(&q1);
            print_statistics(q1);
            fprintf(output_soccer, "%d,%f,%f\n", capacity, get_average_delay(q1), get_loss_rate(q1));

            q2 = initialize(capacity, tracefile_zoom, 100, 5);
            run_QoS(&q2);
            printf("\n");
            print_statistics(q2);
            printf("\n");
            fprintf(output_zoom, "%d,%f,%f\n", capacity, get_average_delay(q2), get_loss_rate(q2));

            rewind(tracefile_soccer);
            rewind(tracefile_zoom);
            destroy_queue(q1.queue);
            destroy_queue(q2.queue);
        }
        fclose(output_soccer);
        fclose(output_zoom);
    }
    //-------------------------------------------------------------------------
    //------------------------- OTHERS QUESTION -------------------------------
    //-------------------------------------------------------------------------
    else if (num_question == 2)
        change_wls(6, tracefile_soccer, tracefile_zoom);
    else if (num_question == 3)
        change_wls(8, tracefile_soccer, tracefile_zoom);
    else
        change_wls(10, tracefile_soccer, tracefile_zoom);


    //-------------------------------------------------------------------------
    //-------------------------   CLOSING  ------------------------------------
    //-------------------------------------------------------------------------
    fclose(tracefile_soccer);
    fclose(tracefile_zoom);

    return EXIT_SUCCESS;

}