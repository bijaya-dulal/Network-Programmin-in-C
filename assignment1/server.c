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
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //listen for incoming connections
      if (listen(server_fd, 2) < 0) { // Maximum 2 players at a time
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);
    //accept clinet 1
     if ((new_socket1 = accept(server_fd, (struct sockaddr *)&address, 
                              (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed for Player 1");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Player 1 connected!\n");
//accept clinet 2
     if ((new_socket2 = accept(server_fd, (struct sockaddr *)&address, 
                              (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed for Player 2");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Player 2 connected!\n");
    //game logic

    //close sockets
    close(new_socket1);
    close(new_socket2);
    close(server_fd);

    return 0;

 }