#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
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

    // Read the welcome message from the server
    read(client_socket, buffer, BUFFER_SIZE);
    printf("%s", buffer);

    // Game loop - client guesses the number
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        printf("Enter your guess: ");
        fgets(buffer, BUFFER_SIZE, stdin);  // Get the user's input

        // Send the guess to the server
        write(client_socket, buffer, strlen(buffer));

        // Read the server's response
        memset(buffer, 0, BUFFER_SIZE);
        read(client_socket, buffer, BUFFER_SIZE);
        printf("%s", buffer);

        // If the client wins, break out of the loop
        if (strstr(buffer, "win") != NULL) {
            break;
        }
    }

    // Close the socket
    close(client_socket);

    return 0;
}
