#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to handle a client
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char *response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, world!";

    // Read client request
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Client request:\n%s\n", buffer);

    // Send HTTP response
    write(client_socket, response, strlen(response));
    printf("Response sent to client.\n");

    // Close client socket
    close(client_socket);
    exit(0);  // Exit child process
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Ignore SIGCHLD to prevent zombie processes
    signal(SIGCHLD, SIG_IGN);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
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

    // Listen for connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept and handle multiple clients
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                 (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Fork a new process to handle the client
        if (fork() == 0) {
            close(server_fd);  // Close the server socket in child process
            handle_client(new_socket);  // Handle the client in the child process
        }
        close(new_socket);  // Parent closes the client socket
    }

    close(server_fd);  // Close the server socket
    return 0;
}
