#include "include.h"
#include "vowelizer.h"

typedef struct{
    int fd_tcp;
    int fd_udp;
    int fd_client;
    struct sockaddr_in addr_local;
    struct sockaddr_in addr_client;
}thread_args;

void* thread_server(void* arg){
    //-----------------------------------------
    //---------------PREPARATION---------------
    //-----------------------------------------
    // Get the arguments
    thread_args* args = (thread_args*)arg;
    int fd_tcp = args->fd_tcp;
    int fd_udp = args->fd_udp;
    int fd_cli_tcp = args->fd_client;
    struct sockaddr_in addr_local = args->addr_local;
    struct sockaddr_in addr_client = args->addr_client;
    struct sockaddr_in addr_recv;
    int addr_len = sizeof(addr_recv);
    // Arrays for messages
    char text[MAX_SIZE_MSG];
    char vowels[MAX_SIZE_MSG];
    char non_vowels[MAX_SIZE_MSG];  

    int mode, option, id;
    //-----------------------------------------
    //------------RECEIVING OPTIONS-------------
    //-----------------------------------------
    // Receive the id of the client
    if (recv(fd_cli_tcp, &id, sizeof(int), 0) == -1)
       err("Error receiving id");
    // Receive the mode
    fprintf(stdout, "Receiving the mode for client %d ...\n", id);
    if (recv(fd_cli_tcp, &mode, sizeof(int), 0) == -1)
        err("Error receiving the mode : ");
    // Receive the option
    fprintf(stdout, "Receiving the option for client %d...\n", id);
    if (recv(fd_cli_tcp, &option, sizeof(int), 0) == -1)
        err("Error receiving the option : ");

    //-----------------------------------------
    //------------RECEIVING MESSAGES-----------
    //-----------------------------------------
    if (mode == 1){
        // Receive the text
        fprintf(stdout, "Receiving the text via TCP for client %d...\n", id);
        memset(text, 0, MAX_SIZE_MSG);
        if (recv(fd_cli_tcp, text, MAX_SIZE_MSG, 0) == -1)
            err("Error receiving the text : ");
    }
    else if (mode == 2){
        // Receive the vowels via UDP
        memset(vowels, 0, MAX_SIZE_MSG);
        fprintf(stdout, "Receiving the vowels via UDP for client %d ...\n", id);
        if (recvfrom(fd_udp, vowels, MAX_SIZE_MSG, 0, (struct sockaddr*)&addr_recv, (socklen_t *)&addr_len) == -1)
            err("Error receiving the vowels : ");
        // Receive the non vowels via TCP
        fprintf(stdout, "Receiving the non vowels via TCP for client %d ...\n", id);
        memset(non_vowels, 0, MAX_SIZE_MSG);
        if (recv(fd_cli_tcp, non_vowels, MAX_SIZE_MSG, 0) == -1)
            err("Error receiving the non vowels via TCP : ");
    }
    //-----------------------------------------
    //------------PROCESSING MESSAGES----------
    //-----------------------------------------
    fprintf(stdout, "Processing for client %d ...\n", id);
    if (option == 1){
        if (mode == 1)
            devoweling_simple(text, vowels, non_vowels);
        else
            envoweling_simple(text, vowels, non_vowels);
    }
    else{
        if (mode == 1)
            devoweling_advanced(text, vowels, non_vowels);
        else
            envoweling_advanced(text, vowels, non_vowels);
    }

    //-----------------------------------------
    //------------SENDING MESSAGES------------
    //-----------------------------------------

    if (mode == 1){
        // Send the vowels via UDP
        fprintf(stdout, "Sending the vowels via UDP for client %d ... : %s\n", id, vowels);
        if (sendto(fd_udp, vowels, strlen(vowels), 0, (struct sockaddr*)&addr_client, sizeof(addr_client)) == -1)
            err("Error sending the vowels via UDP : ");
        sleep(1);
        fprintf(stdout, "Sending the non vowels via TCP for client %d ... : %s\n", id, non_vowels);
        if (send(fd_cli_tcp, non_vowels, strlen(non_vowels), 0) == -1)
            err("Error sending the non vowels via TCP : ");
    }
    else{
        fprintf(stdout, "Sending the concatenation via TCP for client %d ... : %s\n", id, text);
        if (send(fd_cli_tcp, text, strlen(text), 0) == -1)
            err("Error sending the concatenation via TCP : ");
    }
    fprintf(stdout, "Thread for client %d ended\n", id);
    pthread_exit(NULL);
}

int main(int argc, char** argv){
    //-----------------------------------------
    //---------------SOCKETS CREATION----------
    //-----------------------------------------

    int fd_tcp, fd_udp;
    struct sockaddr_in addr_local, addr_client;

    // Create TCP socket
    create_socket(&fd_tcp, SOCK_STREAM, PORT_SERVER, &addr_local, true);
    // Create UDP socket
    create_socket(&fd_udp, SOCK_DGRAM, PORT_SERVER, &addr_local, true);
    // Prepare the client address
    prepare_address(&addr_client, PORT_CLIENT);

    //-----------------------------------------
    //-------------SERVER LOOP-----------------
    //-----------------------------------------
    pthread_t* tid = calloc(MAX_CLIENTS, sizeof(pthread_t));
    if (tid == NULL)
        err("Error allocating memory for the thread id");
    thread_args* args = calloc(MAX_CLIENTS, sizeof(thread_args));
    if (args == NULL)
        err("Error allocating memory for the thread arguments");

    int nb_clients = 0;
    while(1){
        int fd_client;
        // Listen the TCP socket
        fprintf(stdout, "---!!!Waiting for a connection!!!---\n\n");
        if (listen(fd_tcp, BACKLOG) == -1)
            err("Error listening the TCP socket : ");
        // Accept the TCP connection
        if ((fd_client = accept(fd_tcp, NULL, NULL)) == -1)
            err("Error accepting the TCP connection : ");
        fprintf(stdout, "---!!!Connection accepted!!!---\n");

        //-----------------------------------------
        //-------------CLIENT THREAD---------------
        //-----------------------------------------
        // Create the thread arguments
        args[nb_clients].fd_tcp = fd_tcp;
        args[nb_clients].fd_udp = fd_udp;
        args[nb_clients].fd_client = fd_client;
        args[nb_clients].addr_local = addr_local;
        args[nb_clients].addr_client = addr_client;
        // Create the thread
        if (pthread_create(&tid[nb_clients], NULL, thread_server, (void*)&args[nb_clients]) == -1)
            err("Error creating the thread : ");
        // Increment the number of clients
        nb_clients++;
        // Wait for threads to finish When reach WAIT_CLIENTS
        if (nb_clients == WAIT_CLIENTS){
            for (int i = 0; i < nb_clients; i++){
                pthread_join(tid[i], NULL);
            }
            nb_clients = 0;
        }      
    }
    return EXIT_SUCCESS;
}