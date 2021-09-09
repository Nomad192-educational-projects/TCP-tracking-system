#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <cstdlib>
#include <cstdio>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#define DEFAULT_BUFLEN 4096

class TCP_Client {

private:
    WSADATA wsaData{};
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = nullptr;
    struct addrinfo *ptr = nullptr;
    struct addrinfo hints{};

    char recvbuf[DEFAULT_BUFLEN]{};
    int iResult{};
    int recvbuflen = DEFAULT_BUFLEN;
    char *address;
    char *port;
    int length_answer;

public:
    TCP_Client(char *address, char *port)
    {
        this->address = address;
        this->port = port;
        length_answer = -1;
    }
    /*void setAddress(char *new_address)
    {
        address = new_address;
    }
    void setPort(char *new_port)
    {
        port = new_port;
    }
    char * getAddress()
    {
        return address;
    }
    char * getPort()
    {
        return port;
    }*/
    int ini_client();
    int open_connection();
    int send_data(const char * data, int length);
    int close_connection();
    int get_answer();
    int get_length() const
    {
        return length_answer;
    }
    char * get_buf()
    {
        return recvbuf;
    }
    void clean() const;
    ~TCP_Client()
    {
        closesocket(ConnectSocket);
        WSACleanup();
    }
};

#endif //TCP_CLIENT_H