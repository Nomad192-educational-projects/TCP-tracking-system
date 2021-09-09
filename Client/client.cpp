#include "client.h"

void Client::error_handler(int error_number)
{
    last_error = error_number;
    if(log_on)
    {
        printf("Error number %d: ", error_number);
        switch (error_number)
        {
            case 1:
            {
                printf("the tag number does not match any existing ones");
                break;
            }
            case 2:
            {
                printf("you can't use a reserved tag");
                break;
            }
            case 3:
            {
                printf("zero length of the service part");
                break;
            }
            case 4:
            {
                printf("the number of octets required for length encoding has exceeded the standard");
                break;
            }
            case 5:
            {
                printf("there is no data to encode");
                break;
            }
            case 6:
            {
                printf("the data type for encoding is not supported");
                break;
            }
            case 7:
            {
                printf("there is no data for decoding");
                break;
            }
            case 8:
            {
                printf("the length of the decoded message is less than the minimum");
                break;
            }
            case 9:
            {
                printf("the decoded message ended during the processing of the service part");
                break;
            }
            case 10:
            {
                printf("the length of the service part of the decoded message is longer than the standard");
                break;
            }
            case 11:
            {
                printf("the decoded message ended before the length was processed");
                break;
            }
            case 12:
            {
                printf("the decoded message ended in the process of processing the length");
                break;
            }
            case 13:
            {
                printf("the decoded message ended during data processing");
                break;
            }
            case 14:
            {
                printf("the type of decoded data is not supported");
                break;
            }
            case 15:
            {
                printf("there is no data to convert to Int");
                break;
            }
            case 16:
            {
                printf("the length of the encoded message is zero");
                break;
            }
            case 17:
            {
                printf("the length of the incoming message is less than the minimum");
                break;
            }
            case 18:
            {
                printf("the sent and received data are different");
                break;
            }
            default:
            {
                printf("win network", error_number); /// It is necessary
                                                            /// to implement the double error value approach,
                                                            /// for the possibility of more flexible debugging
                                                            /// and support for network errors
            }
        }
        printf("!\n");
    }
}

void Client::test_tag()
{
    Test_Tag test;
    //test.full();
    test.release();
}

int Client::send_int(const int number)
{
    class X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                            X_690_BER_Tag::Tag_number::INTEGER);
    uint8_t* out_message_bit = nullptr;
    out_tag.software_int_data(number);

    int buffer;

    if((buffer = out_tag.encoding(&out_message_bit)) != 0)
    {
        error_handler(buffer);
        return buffer;
    }

    int length = out_tag.get_length();
    if (length == 0)
    {
        error_handler(16);
        return 16;
    }

    class TCP_Client client(DEFAULT_ADD, DEFAULT_PORT); ///"127.0.0.1" or "localhost" and "27015"


    if ((buffer = client.ini_client()) != 0)
    {
        error_handler(buffer);
        return buffer;
    }
    if ((buffer = client.open_connection()) != 0)
    {
        error_handler(buffer);
        return buffer;
    }
    if ((buffer = client.send_data(reinterpret_cast<const char *>(out_message_bit), length)) != 0)
    {
        error_handler(buffer);
        return buffer;
    }
    if ((buffer = client.close_connection()) != 0)
    {
        error_handler(buffer);
        return buffer;
    }
    if ((buffer = client.get_answer()) != 0)
    {
        error_handler(buffer);
        return buffer;
    }
    client.clean();


    X_690_BER_Tag in_tag;
    uint32_t in_length = client.get_length();
    if (in_length < 3)
    {
        error_handler(17);
        return 17;
    }
    if ((buffer = in_tag.decoding(
            reinterpret_cast<const uint8_t *>(client.get_buf()),
            in_length)) != 0)
    {
        error_handler(buffer);
        return buffer;
    }
    int data = in_tag.software_data_int();
    if (data != number)
    {
        if (in_tag.get_last_error() == 1)
        {
            error_handler(15);
            return 15;
        }
        error_handler(18);
        return 18;
    }
    return 0;
}