#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024
#define MAX_NUMBER 100  // Range for the random number

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    int number_to_guess, client_guess;

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
    if (listen(server_fd, 1) < 0) { // Only one client allowed
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept the client connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected!\n");

    // Generate the random number to guess
    srand(time(NULL));  // Seed the random number generator
    number_to_guess = rand() % MAX_NUMBER + 1;  // Generate a number between 1 and MAX_NUMBER
    printf("The number to guess has been generated (hidden to the client).\n");

    // Inform the client to start the game
    char start_msg[] = "Welcome to the Guess the Number Game! Guess a number between 1 and 100.\n";
    write(new_socket, start_msg, strlen(start_msg));

    // Game logic - client keeps guessing
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        write(new_socket, "Make your guess: ", 18);
        read(new_socket, buffer, BUFFER_SIZE);
        client_guess = atoi(buffer);  // Convert the guess to an integer
        printf("Client guessed: %d\n", client_guess);

        if (client_guess == number_to_guess) {
            write(new_socket, "Correct! You win!\n", 18);
            break;
        } else if (client_guess < number_to_guess) {
            write(new_socket, "Too low. Try again.\n", 20);
        } else {
            write(new_socket, "Too high. Try again.\n", 21);
        }
    }

    // Close the socket
    close(new_socket);
    close(server_fd);

    return 0;
}
