#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENTS 5
#define PORT 8080
#define BUFFER_SIZE 1024

pthread_mutex_t client_count_mutex = PTHREAD_MUTEX_INITIALIZER;
int client_count = 0;

void *handle_client(void *client_socket_ptr) {
    int client_socket = *((int *)client_socket_ptr);
    free(client_socket_ptr);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    printf("Client connected\n");

    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("Received request: %s", buffer);
        send(client_socket, buffer, bytes_received, 0);
    }

    close(client_socket);

    pthread_mutex_lock(&client_count_mutex);
    client_count--;
    pthread_mutex_unlock(&client_count_mutex);

    printf("Client disconnected\n");

    return NULL;
}

int main() {
    printf("Server running\n");

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

        if (*client_socket_ptr == -1) {
            perror("accept");
            continue;
        }

        pthread_mutex_lock(&client_count_mutex);
        if (client_count >= MAX_CLIENTS) {
            close(*client_socket_ptr);
            pthread_mutex_unlock(&client_count_mutex);
            continue;
        }
        client_count++;
        pthread_mutex_unlock(&client_count_mutex);

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, client_socket_ptr);
        pthread_detach(client_thread);
    }

    close(server_socket);
    return 0;
}
