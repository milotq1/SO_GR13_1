#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        printf("Enter a message (q to quit): ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (buffer[0] == 'q' && (buffer[1] == '\n' || buffer[1] == '\0')) {
            break;
        }

        send(client_socket, buffer, strlen(buffer), 0);

        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Server response: %s", buffer);
        }
    }

    close(client_socket);
    return 0;
}
