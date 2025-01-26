#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept client connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected!\n");

    // Generate a random number between 1 and 100
    srand(time(0));
    int number_to_guess = rand() % 100 + 1;
    printf("Number to guess: %d (for testing purposes)\n", number_to_guess);  // For testing only

    // Send welcome message
    char welcome_msg[] = "Welcome to 'Guess the Number' game! Try to guess the number between 1 and 100.\n";
    send(new_socket, welcome_msg, strlen(welcome_msg), 0);

    // Game loop: Handle guesses from client
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);  // Clear buffer
        recv(new_socket, buffer, BUFFER_SIZE, 0);  // Receive guess from client

        int guess = atoi(buffer);  // Convert guess to integer

        if (guess < number_to_guess) {
            char response[] = "Too low. Try again.\n";
            send(new_socket, response, strlen(response), 0);
        } else if (guess > number_to_guess) {
            char response[] = "Too high. Try again.\n";
            send(new_socket, response, strlen(response), 0);
        } else {
            char response[] = "Correct! You win!\n";
            send(new_socket, response, strlen(response), 0);
            break;  // End the game
        }
    }

    // Close the socket
    close(new_socket);
    close(server_fd);
    return 0;
}
