#include "include.h"

typedef struct{
    int id;
    pthread_mutex_t* mutex;
}thread_args;

int choose_mode(int id){
    int t = 0;
    int mode = 0;
    do {
        fprintf(stdout, "\nChoose mode for client %d:\n", id);
        fprintf(stdout, "1. Devoweling\n");
        fprintf(stdout, "2. Envoweling\n");
        fprintf(stdout, "3. Exit\n\n");
        mode = getc(stdin) - '0';
        getchar();
        t++;
    }while(mode < 1 || mode > 3 && (t < 3));
    printf("\n");
    if (t >= 3){
        fprintf(stderr, "Number of tries exceeded.\n");
        exit(EXIT_FAILURE);
    }
    return mode;
}

int choose_option(int id){
    int option = 0;
    int t = 0;
    do{
        fprintf(stdout, "\nChoose an option for client %d:\n", id);
        fprintf(stdout, "1. Simple mode\n");
        fprintf(stdout, "2. Advanced mode\n\n");
        option = getc(stdin) - '0';
        getchar();
        t++;
    }while((option < 1 || option > 2) && (t < 3));
    printf("\n");
    if (t >= 3){
        fprintf(stderr, "Number of tries exceeded.\n");
        exit(EXIT_FAILURE);
    }
    return option;
}

// Thread function for client
void* thread_client(void* arg){
    //-----------------------------------------
    //---------------PREPARATION---------------
    //-----------------------------------------
    // Get the arguments
    thread_args* args = (thread_args*)arg;
    int id = args->id;
    pthread_mutex_t* mutex = args->mutex;
    struct sockaddr_in addr_server;
    struct sockaddr_in addr_local;
    int addr_len = sizeof(addr_server);
    // Arrays for messages
    char text[MAX_SIZE_MSG];
    char vowels[MAX_SIZE_MSG];
    char non_vowels[MAX_SIZE_MSG]; 
    // Choose the mode
    int fd_tcp, fd_udp;

        //-----------------------------------------
        //----------------SOCKETS------------------
        //-----------------------------------------
        // CREATION
        // Create TCP socket
        create_socket(&fd_tcp, SOCK_STREAM, PORT_CLIENT, &addr_local, true);
        // Prepare the server address
        prepare_address(&addr_server, PORT_SERVER);
    pthread_mutex_lock(mutex);
        // Create UDP socket
        create_socket(&fd_udp, SOCK_DGRAM, PORT_CLIENT, &addr_local, true);

        //-----------------------------------------
        //----------------CONNECTION---------------
        //-----------------------------------------
        // Connect to the TCP server
        int option = choose_option(id);
        int mode = choose_mode(id);
        if (mode == 3){
            close(fd_tcp);
            close(fd_udp);
            pthread_mutex_unlock(mutex);
            pthread_exit(NULL);
        }
        if (connect(fd_tcp, (struct sockaddr*)&addr_server, sizeof(addr_server)) == -1)
            err("Error connection to the server ");
        // Send the id to the server
        if (send(fd_tcp, &id, sizeof(int), 0) == -1)
            err("Error sending the id to the server ");
        // Send the mode to the server
        if (send(fd_tcp, &mode, sizeof(int), 0) == -1)
            err("Error sending the mode to the server ");
        // Send the option to the server
        if (send(fd_tcp, &option, sizeof(int), 0) == -1)
            err("Error sending the option to the server ");

        //-----------------------------------------
        //---------------SEND----------------------
        //-----------------------------------------
        if (mode == 1){
            // Demand to enter the text
            fprintf(stdout, "Enter the text for client %d: ", id);
            // Get the text
            getchar();
            memset(text, 0, MAX_SIZE_MSG);
            fgets(text, MAX_SIZE_MSG, stdin);
            // Send to TCP server
            if (send(fd_tcp, text, strlen(text), 0) == -1)
                err("Error sending the text to the TCP server ");
        }
        else{
            // Demand to enter vowels
            fprintf(stdout, "Enter the vowels for client %d: ", id);
            getchar();
            memset(vowels, 0, MAX_SIZE_MSG);
            fgets(vowels, MAX_SIZE_MSG, stdin);
            // Send to UDP server
            if (sendto(fd_udp, vowels, strlen(vowels), 0, (struct sockaddr*)&addr_server, sizeof(addr_server)) == -1)
                err("Error sending the vowels to the UDP server ");
            // Demand to enter non-vowels
            fprintf(stdout, "Enter the non-vowels for client %d: ", id);
            memset(non_vowels, 0, MAX_SIZE_MSG);
            fgets(non_vowels, MAX_SIZE_MSG, stdin);
            // Send to TCP server
            if (send(fd_tcp, non_vowels, strlen(non_vowels), 0) == -1)
                err("Error sending the non-vowels to the TCP server ");
        }

        //-----------------------------------------
        //----------------RECEIVE------------------
        //-----------------------------------------
        if (mode == 1){
            fprintf(stdout, "waiting for vowels for client %d ...\n", id);
            memset(vowels, 0, MAX_SIZE_MSG);
            // Receive vowels from UDP server
            if (recvfrom(fd_udp, vowels, MAX_SIZE_MSG, 0, (struct sockaddr*)&addr_server, (socklen_t*)&addr_len) == -1)
                err("Error receiving the vowels from the UDP server ");
            // Receive non vowels from TCP server
            fprintf(stdout, "waiting for non-vowels for client %d...\n", id);
            memset(non_vowels, 0, MAX_SIZE_MSG);
            if (recv(fd_tcp, non_vowels, MAX_SIZE_MSG, 0) == -1)
                err("Error receiving the non-vowels from the server ");
        }
        else{
            // Receive text from TCP server
            fprintf(stdout, "waiting for text for client %d ...\n", id);
            memset(text, 0, MAX_SIZE_MSG);
            if (recv(fd_tcp, text, MAX_SIZE_MSG, 0) == -1)
                err("Error receiving the text from the server ");
        }
        // Print the result
        fprintf(stdout, "Text : %s\n", text);
        fprintf(stdout, "Vowels : %s\n", vowels);
        fprintf(stdout, "Non-vowels : %s\n", non_vowels);
        close(fd_udp);
    pthread_mutex_unlock(mutex);
    close(fd_tcp);
    pthread_exit(NULL);
}

int main(int argc, char** argv){

    if (argc != 2){
        fprintf(stderr, "Usage : %s <number of clients>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int nb_clients = atoi(argv[1]);
    if (nb_clients < 1 || nb_clients > MAX_CLIENTS){
        fprintf(stderr, "Usage : %s <number of clients>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //-------------------------------------------------------------
    //---------------------SOCKETS CREATION------------------------
    //-------------------------------------------------------------

    // VARIABLES
    // Decriptors for the TCP and UDP sockets
    int fd_tcp, fd_udp;

    //-------------------------------------------------------------
    //---------------------THREADS CREATION------------------------
    //-------------------------------------------------------------
    pthread_t* tid = calloc(nb_clients, sizeof(pthread_t));
    thread_args* args = calloc(nb_clients, sizeof(thread_args));
    pthread_mutex_t mutex;

    // Create the mutex
    pthread_mutex_init(&mutex, NULL);

    // Initialize the arguments for the threads and create them
    for (int i = 0; i < nb_clients; i++){
        args[i].id = i;
        args[i].mutex = &mutex;
        pthread_create(&tid[i], NULL, thread_client, &args[i]);
    }

    // Wait for the threads to finish
    for (int i = 0; i < nb_clients; i++){
        pthread_join(tid[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}