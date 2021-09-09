#ifndef SERVER_H
#define SERVER_H

#include <omp.h>
#include "tcp_server.h"

#define DEFAULT_PORT "27015"

/// It should be noted that
/// that if we do not initialize the "tag_class" and "tag_number" parameters
/// in the "X_690_BR_Tag" class in any way,
/// then "garbage"may appear when getting values from there

class Server {

public:
    static int start(int number_threads);

};

#endif //SERVER_H