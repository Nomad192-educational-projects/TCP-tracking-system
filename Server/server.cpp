#include "server.h"

int Server::start(int number_threads)
{
    while(true)
    {
        class TCP_Server server(DEFAULT_PORT);
        if (server.ini_server() == 0)
        {
            if (server.ini_listen() == 0)
            {
                if (server.processing_request(number_threads) == 0)
                {
                    server.close_listen();
                    server.clean();
                    return 0;
                }
                server.close_listen();
                server.clean();
            }
        }
    }
}