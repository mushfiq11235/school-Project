#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BACKLOG 10
#define PORT 2049
#define PORT_SERVER 8989
#define PORT_CLIENT 8990
#define MAX_SIZE_MSG 512
#define MAX_CLIENTS 10
#define WAIT_CLIENTS 5

#define ADDR_SERVER "127.0.0.5"
#define ADDR_CLIENT "127.0.0.6"

void err(const char * mess){
    perror(mess);
    exit(EXIT_FAILURE);
}

void prepare_address(struct sockaddr_in *addr, int port){
    memset((char* )addr, 0, sizeof(struct sockaddr_in));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = htonl(INADDR_ANY);
    // if (inet_pton(AF_INET, addr_ip, &addr->sin_addr) <= 0)
    //     err("inet_pton");
    
}

void create_socket(
    int* fd, 
    const int type, 
    const int port,
    struct sockaddr_in* addr,
    bool reuse
)
{
    // Creation of the socket
    if ((*fd = socket(AF_INET, type, IPPROTO_IP)) == -1)
        err("Error creation socket ");
    // Option to allow address reuse for other clients
    if (reuse){
        int optval = 1;
        // A printf here to see if the option is set
        if (setsockopt(*fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
            err("Error setting the option ");
    }
    // Preparation of the server address
    prepare_address(addr, port);
    // Bind socket
    if (bind(*fd, (struct sockaddr*)addr, sizeof(struct sockaddr_in)) == -1)
        err("Error binding the socket ");
}
#endif