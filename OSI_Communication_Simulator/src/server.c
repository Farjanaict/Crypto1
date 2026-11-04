#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
typedef SOCKET socket_t;
#define CLOSE_SOCKET closesocket
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int socket_t;
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define CLOSE_SOCKET close
#endif

#include <stdio.h>
#include <string.h>
#include "layers.h"

#define PORT 5055

int main(void) {
    socket_t server_fd, client_fd;
    struct sockaddr_in address;
    char packet[OSI_BUFFER_SIZE];
    char original[OSI_BUFFER_SIZE];
    int received = 0;
    int opt = 1;

#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }
#endif

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        printf("Could not create server socket.\n");
        return 1;
    }

#ifndef _WIN32
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#else
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(opt));
#endif

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed. Is port %d already in use?\n", PORT);
        CLOSE_SOCKET(server_fd);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    if (listen(server_fd, 1) == SOCKET_ERROR) {
        printf("Listen failed.\n");
        CLOSE_SOCKET(server_fd);
        return 1;
    }

    printf("OSI SERVER is listening on 127.0.0.1:%d\n", PORT);
    printf("Start the client in another terminal...\n");

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == INVALID_SOCKET) {
        printf("Client connection failed.\n");
        CLOSE_SOCKET(server_fd);
        return 1;
    }
    printf("\nClient connected successfully.\n");

    while (received < (int)sizeof(packet) - 1) {
        char ch;
#ifdef _WIN32
        int n = recv(client_fd, &ch, 1, 0);
#else
        int n = (int)recv(client_fd, &ch, 1, 0);
#endif
        if (n <= 0 || ch == '\n') break;
        packet[received++] = ch;
    }
    packet[received] = '\0';

    if (received > 0) {
        printf("\nPacket received through real TCP socket.\n");
        if (decapsulate(packet, original, sizeof(original)) == 0) {
            printf("\n[APPLICATION] Final message received: %s\n", original);
        } else {
            printf("Could not decode packet.\n");
        }
    } else {
        printf("No data received.\n");
    }

    CLOSE_SOCKET(client_fd);
    CLOSE_SOCKET(server_fd);
#ifdef _WIN32
    WSACleanup();
#endif
    printf("\nServer stopped.\n");
    return 0;
}
