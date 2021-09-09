#include "client.h"

void first_variant()    /// Internal coding/decoding testing
{
    Client client;
    client.test_tag();
}

void second_variant(int value)     /// Sending a specific int value
{
    Client client;              /// you can enable logging - Client client(true);
    printf("Send %d: ", value);
    if (client.send_int(value) != 0)
    {
        printf("error - %d\n", client.get_last_error());
        return;
    }
    printf("OK\n");
}

void third_variant(int base)    /// Comprehensive testing for sending the Int type
{
    printf("Testing sending int based on %d...\n", base);
    if (base == 0) {
        return; ///ERROR
    }
    Client client;
    int i = 0;

    for (; i <= 100; i += base) {
        if (client.send_int(i) != 0)
        {
            printf("error - %d\n", client.get_last_error());
        }
    }
    i = -100;
    for (; i < 0; i += base)
    {
        if (client.send_int(i) != 0)
        {
            printf("error - %d\n", client.get_last_error());
        }
    }

    i = 0;
    for (; i >= 0; i += base) {

        if (client.send_int(i) != 0)
        {
            printf("error - %d\n", client.get_last_error());
        }

        if (i % (128 * base) == 0) {
            double f = i;
            printf("%.6f%\n", (f / 2147483647) / 2 * 100);
        }
    }
    for (; i < 0; i += base) {

        if (client.send_int(i) != 0)
        {
            printf("error - %d\n", client.get_last_error());
        }

        if (i % (128 * base) == 0) {
            double f = 2147483647 + i;
            printf("%.6f%\n", 50 + (f / 2147483647) / 2 * 100);
        }
    }
    printf("OK\n");
}


int main()
{
    //first_variant();
    second_variant(-5);
    //third_variant(1);
    return 0;
}