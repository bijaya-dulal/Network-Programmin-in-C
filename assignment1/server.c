#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8081
#define BUFFER_SIZE 1024

 int main(){
   int server_fd, new_socket1 , new_socket2;
   struct sockaddr_in address;
   int addrlen = sizeof(address);
    //create socket
      if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //bind socket to port

    //listen for incoming connections

    //accept clinet

    //game logic

    //close sockets
    return 0;

 }