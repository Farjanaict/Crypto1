#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
typedef SOCKET socket_t;
#define CLOSE_SOCKET closesocket
#else
#include <sys/types.h>
#include <sys/socket.h>
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

int main(int argc, char *argv[]) {
    socket_t sock;
    struct sockaddr_in server;
    char message[OSI_BUFFER_SIZE];
    char packet[OSI_BUFFER_SIZE];
    const char *input = (argc > 1) ? argv[1] : "Hello from OSI Communication Simulator!";

#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Winsock initialization failed.\n");
        return 1;
    }
#endif

    if (strlen(input) >= sizeof(message) - 1) {
        printf("Message is too long. Keep it under %d characters.\n", OSI_BUFFER_SIZE - 1);
        return 1;
    }
    strcpy(message, input);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Could not create client socket.\n");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Connecting to OSI server...\n");
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connection failed. Start server.exe first.\n");
        CLOSE_SOCKET(sock);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    printf("Connected to server.\n");
    if (encapsulate(message, packet, sizeof(packet)) != 0) {
        printf("Encapsulation failed. Message may be too long.\n");
        CLOSE_SOCKET(sock);
        return 1;
    }

    {
        char send_buffer[OSI_BUFFER_SIZE + 2];
        int total = 0;
        int length;
        snprintf(send_buffer, sizeof(send_buffer), "%s\n", packet);
        length = (int)strlen(send_buffer);

        while (total < length) {
#ifdef _WIN32
            int n = send(sock, send_buffer + total, length - total, 0);
#else
            int n = (int)send(sock, send_buffer + total, (size_t)(length - total), 0);
#endif
            if (n <= 0) {
                printf("Send failed.\n");
                CLOSE_SOCKET(sock);
                return 1;
            }
            total += n;
        }
    }

    printf("\nPacket sent successfully to 127.0.0.1:%d\n", PORT);
    CLOSE_SOCKET(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
