#include "tcp_client.h"

int TCP_Client::ini_client()
{
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        //printf("WSAStartup failed with error: %d\n", iResult);
        return 20;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(address, port, &hints, &result);
    if ( iResult != 0 ) {
        //printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 20;
    }
    return 0;
}

int TCP_Client::open_connection()
{
    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != nullptr ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            //printf("Socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 20;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        //printf("Unable to connect to server!\n");
        WSACleanup();
        return 20;
    }
    return 0;
}

int TCP_Client::send_data(const char *data, const int length)
{
    // Send an initial buffer
    iResult = send(ConnectSocket, data, length, 0);
    if (iResult == SOCKET_ERROR) {
        //printf("Send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 20;
    }

    /*printf("Bytes Sent: %ld\n", iResult);
    for (int i = 0; i < iResult; i++)
    {
        printf("0x%hhX ", data[i]);
    }
    printf("\n");*/

    return 0;
}

int TCP_Client::close_connection()
{
    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        //printf("Shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 20;
    }
    return 0;
}

int TCP_Client::get_answer()
{
    // Receive until the peer closes the connection
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 )
        {
            //printf("Bytes received: %d\n", iResult);
            length_answer = iResult;

            /*for (int i = 0; i < iResult; i++)
            {
                printf("0x%hhX ", recvbuf[i]);
            }
            printf("\n");*/
        }
        else if (iResult == 0)
        {
            //printf("Connection closed\n");
        }
        else
        {
            //printf("recv failed with error: %d\n", WSAGetLastError());
            return 20; /// need return WSAGetLastError()
        }
    } while( iResult > 0 );
    return 0;
}

void TCP_Client::clean() const
{
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();
}