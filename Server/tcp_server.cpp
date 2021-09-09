#include "tcp_server.h"

int TCP_Server::ini_server()
{
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        //printf("WSAStartup failed with error: %d\n", iResult);
        return 20;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(nullptr, port, &hints, &result);
    if ( iResult != 0 ) {
        //printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 20;
    }
    return 0;
}

int TCP_Server::ini_listen()
{
    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        //printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 20;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        //printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 20;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        //printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 20;
    }

    return 0;
}

int TCP_Server::processing_request(int number_threads)
{
    /// You need to be able to quickly and smoothly shut down the server,
    /// I see this as setting the listening socket to a non-blocking state

    if (number_threads < 0)
    {
        return 20;
    }

    omp_set_num_threads(number_threads);
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        #pragma omp parallel firstprivate(ClientSocket) default(none)//firstprivate(ClientSocket, ListenSocket)
        {
            // Accept a client socket
            ClientSocket = accept(ListenSocket, nullptr, nullptr);
            int data = 0;
            int error = 0;
            int buffer = 0;
            if (ClientSocket != INVALID_SOCKET)
            {
                // Receive until the peer shuts down the connection
                do {

                    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
                    if (iResult > 0) {
                        //printf("Bytes received: %d core: %d\n", iResult, omp_get_thread_num());

                        /*for (int i = 0; i < iResult; i++) {
                            printf("0x%hhX ", recvbuf[i]);
                        }
                        printf("core: %d\n", omp_get_thread_num());*/

                        X_690_BER_Tag in_tag;
                        if (iResult < 3)
                        {
                            //printf("error in length = %d\n", iResult);
                            error = -1;
                        }
                        else
                        {
                            if ((buffer = in_tag.decoding(
                                    reinterpret_cast<const uint8_t *>(recvbuf),
                                    iResult)) != 0)
                            {
                                //printf("error number %d\n", buffer);
                                error = buffer;
                            }
                            else
                            {
                                data = in_tag.software_data_int();
                                if (data == 0 && in_tag.get_last_error() == 1)
                                {
                                    //printf("error number %d\n", 15);
                                    error = 15;
                                }
                                else
                                {
                                    class X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                                                X_690_BER_Tag::Tag_number::INTEGER);
                                    uint8_t* out_message_bit = nullptr;
                                    out_tag.software_int_data(data);

                                    if((buffer = out_tag.encoding(&out_message_bit)) != 0)
                                    {
                                        //printf("error number %d\n", buffer);
                                        error = buffer;
                                    }
                                    else
                                    {
                                        int length = out_tag.get_length();
                                        if (length == 0)
                                        {
                                            //printf("error out length");
                                            error = -2;
                                        }
                                        else
                                        {
                                            // Echo the buffer back to the sender
                                            iSendResult = send(
                                                    ClientSocket,
                                                    reinterpret_cast<const char *>(out_message_bit),
                                                    length,
                                                    0);
                                            if (iSendResult == SOCKET_ERROR)
                                            {
                                                error = 20;
                                                //printf("send failed with error: %d core: %d\n", WSAGetLastError(), omp_get_thread_num());
                                                closesocket(ClientSocket);
                                                WSACleanup();
                                                //return 1;
                                            }
                                            //printf("Bytes sent: %d core: %d\n", iSendResult, omp_get_thread_num());
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else if (iResult == 0)
                    {
                        //printf("Connection closing... core: %d\n\n", omp_get_thread_num());
                    }
                    else
                    {
                        //printf("recv failed with error: %d core: %d\n", WSAGetLastError(), omp_get_thread_num());
                        closesocket(ClientSocket);
                        WSACleanup();
                        //return 1;
                    }

                } while (iResult > 0);

                // shutdown the connection since we're done
                iResult = shutdown(ClientSocket, SD_SEND);
                if (iResult == SOCKET_ERROR)
                {
                    error = 20;
                    //printf("shutdown failed with error: %d core: %d\n", WSAGetLastError(), omp_get_thread_num());
                    closesocket(ClientSocket);
                    WSACleanup();
                    //return 1;
                }
                printf("Core %d: ", omp_get_thread_num());
                if (error == 0)
                {
                    printf("received %d\n", data);
                }
                else
                {
                    printf("error %d\n", error);
                }
            }
            else
            {
                //printf("accept failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                WSACleanup();
                //return 1;
            }
        }
        if (iResult == SOCKET_ERROR)
        {
            return 20;
        }
        if (iSendResult == SOCKET_ERROR)
        {
            return 20;
        }
    }

    return 0;
}

void TCP_Server::close_listen() const
{
    closesocket(ListenSocket);
}

void TCP_Server::clean() const
{
    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
}