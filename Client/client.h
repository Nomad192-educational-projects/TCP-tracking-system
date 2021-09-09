#ifndef CLIENT_H
#define CLIENT_H

#include "message.h"
#include "test.cpp"
#include "tcp_client.h"

#define DEFAULT_PORT "27015"
#define DEFAULT_ADD "localhost"

/// It should be noted that
/// that if we do not initialize the "tag_class" and "tag_number" parameters
/// in the "X_690_BR_Tag" class in any way,
/// then "garbage"may appear when getting values from there

class Client {
private:
    bool log_on;
    int last_error;
    void error_handler(int error_number);
public:
    Client()
    {
        log_on = false;
        last_error = 0;
    }
    explicit Client(bool log_on)
    {
        this->log_on = log_on;
        last_error = 0;
    }
    int get_last_error() const
    {
        return last_error;
    }
    void test_tag();
    int send_int(int number);
};

#endif //CLIENT_H