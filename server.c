#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 2048
#define STATIC_DIR "www"  // Directory to store static files

// Function to send a 404 response
void send_404(int client_socket) {
    const char *response =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 22\r\n"
        "\r\n"
        "<h1>404 Not Found</h1>";
    write(client_socket, response, strlen(response));
}

// Function to serve static files
void serve_file(int client_socket, const char *file_path) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s%s", STATIC_DIR, file_path);

    // Check if the file exists
    FILE *file = fopen(full_path, "r");
    if (file == NULL) {
        send_404(client_socket);
        return;
    }

    // Read file content
    char file_buffer[BUFFER_SIZE];
    size_t bytes_read = fread(file_buffer, 1, sizeof(file_buffer), file);
    fclose(file);

    // Create HTTP response
    char response_header[256];
    snprintf(response_header, sizeof(response_header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %zu\r\n"
             "\r\n",
             bytes_read);

    // Send HTTP header and file content
    write(client_socket, response_header, strlen(response_header));
    write(client_socket, file_buffer, bytes_read);
}

// Function to handle a client request
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};

    // Read the client's HTTP request
    read(client_socket, buffer, BUFFER_SIZE);
    printf("Request:\n%s\n", buffer);

    // Parse the request line
    char method[10], path[1024];
    sscanf(buffer, "%s %s", method, path);

    // Serve the requested file
    if (strcmp(method, "GET") == 0) {
        if (strcmp(path, "/") == 0) {
            // Default to index.html if no file is specified
            serve_file(client_socket, "/index.html");
        } else {
            serve_file(client_socket, path);
        }
    } else {
        // Method not supported
        send_404(client_socket);
    }

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
