#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <omp.h>

#include "message.h"

#define DEFAULT_BUFLEN 4096

class TCP_Server {

private:
    WSADATA wsaData{};
    int iResult{};

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = nullptr;
    struct addrinfo hints{};

    int iSendResult{};
    char recvbuf[DEFAULT_BUFLEN]{};
    int recvbuflen = DEFAULT_BUFLEN;
    char *port;

public:
    explicit TCP_Server(char *port)
    {
        this->port = port;
    }
    int ini_server();
    int ini_listen();
    int processing_request(int number_threads);
    void close_listen() const;
    void clean() const;
    ~TCP_Server()
    {
        closesocket(ListenSocket);
        closesocket(ClientSocket);
        WSACleanup();
    }
};

#endif //TCP_SERVER_H