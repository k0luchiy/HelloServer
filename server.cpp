#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = { 0 };
    const char* server_hello = "Server received";

    // create a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) == 0) {
        perror("socket failed!");
        exit(EXIT_FAILURE);
    }

    // set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // setup the server address

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind fails");
        exit(EXIT_FAILURE);
    }

    // start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listening failed");
        exit(EXIT_FAILURE);
    }

    // print a message showing that the server is listening on a port
    printf("Server listening on port %d\n", PORT);

    // accept incoming connections
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // accepting the connection
    printf("Connection accepted\n");

    // read data from the client and print it
    ssize_t valread;
    while ((valread = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        // Print the received data
        send(new_socket, server_hello, strlen(server_hello), 0);
        printf("Received data:\n%s\n", buffer);
        // Reset buffer for the next read
        memset(buffer, 0, sizeof(buffer));
    }


    // close the socket
    close(server_fd);
    return 0;
}
