#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 2048

// Function to handle a client
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    char *response;

    // Read the HTTP request from the client
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Request:\n%s\n", buffer);

    // Parse the request line (method and path)
    char method[10], path[1024];
    sscanf(buffer, "%s %s", method, path);

    // Handle different routes
    if (strcmp(path, "/") == 0) {
        // Response for "/"
        response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 27\r\n"
            "\r\n"
            "<h1>Welcome to my server!</h1>";
    } else if (strcmp(path, "/about") == 0) {
        // Response for "/about"
        response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 21\r\n"
            "\r\n"
            "<h1>About Page</h1>";
    } else {
        // Response for unknown paths (404)
        response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 22\r\n"
            "\r\n"
            "<h1>404 Not Found</h1>";
    }

    // Send the response to the client
    write(client_socket, response, strlen(response));

    // Close the connection
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

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

    // Listen for incoming connections
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

        // Handle the client
        handle_client(new_socket);
    }

    close(server_fd);
    return 0;
}
