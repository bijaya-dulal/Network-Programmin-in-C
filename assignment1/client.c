#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024


int main(){
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE]={0};

    //create socket
     if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection to the server failed");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    // Placeholder for communication with server
    printf("You can now communicate with the server.\n");

    // Close the socket
    close(client_socket);

    return 0;

}