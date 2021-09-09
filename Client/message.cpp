#include "message.h"

void X_690_BER_Tag::ini_length_zero()    /// Initialized lengths with zeros
{
    uint8_t length = ARRAY_SIZE(parameters.length) - 1;
    for (;length > 0; length--)
    {
        parameters.length[length] = 0;
    }
    parameters.length[length] = 0;
}

///Pre:     number >= 0 and number < (1 << 31)
///Post:    return > 1 and return < 7
uint8_t X_690_BER_Tag::calculating_number_octets_service_part() const
{
    int number = parameters.tag_number;
    if (number >= 0 && number < 31)
        return 1;
    if (number >= 31 && number < 128)
        return 2;
    if (number >= 128 && number < 16384)
        return 3;
    if (number >= 16384 && number < 2097152)
        return 4;
    if (number >= 2097152 && number < 268435456)
        return 5;
    if (number >= 268435456 && number <= 2147483647)
        return 6;
    return 0;   ///ERROR    (Impossible under the current conditions)
}

int X_690_BER_Tag::encoding(uint8_t ** full_data_bit)
{
    ///-------------------------------SERVICE_PART-------------------------------
    #ifndef CHECK
        if (parameters.tag_number >= ARRAY_SIZE(P_C))
        {
            return 1; ///ERROR    the tag number does not match any existing ones
        }
        if (P_C[parameters.tag_number] == 3)
        {
            return 2; ///ERROR    you can't use a reserved tag
        }
    #endif

    uint8_t n_octet_service_part = calculating_number_octets_service_part();

    if (n_octet_service_part == 0)
    {
        return 3; ///ERROR    zero length of the service part
    }

    uint8_t service_part[n_octet_service_part];

    if (n_octet_service_part == 1)
    {
        service_part[0] = (parameters.tag_class << 6) + parameters.tag_number;
        #ifndef CHECK
            service_part[0] += ((P_C[parameters.tag_number] == 1 ? 1 : 0) << 5);
        #endif
    }
    else
    {
        service_part[0] = (parameters.tag_class << 6) + 0x1F;
        #ifndef CHECK
            service_part[0] += ((P_C[parameters.tag_number] == 1 ? 1 : 0) << 5);
        #endif

        for (int i = 0; i < n_octet_service_part - 2; i++)
        {
            uint8_t buffer = ((parameters.tag_number >> (7 * (n_octet_service_part - 2 - i))) << 1);
            service_part[i + 1] = (1 << 7) + (buffer  >> 1);
        }
        {
            uint8_t buffer = (parameters.tag_number << 1);
            service_part[n_octet_service_part - 1] = (buffer  >> 1);
        }
    }
    ///-------------------------------END_SERVICE_PART-------------------------------

    ///-------------------------------LENGTH_PART-------------------------------
    int n_octet_length_part = -1;

    if (parameters.length[0] >= 128)
    {
        for (int i = 0; i < 126; i++)
        {
            if (parameters.length[i] != 0)
            {
                n_octet_length_part = i;
            }
        }
        if (n_octet_length_part == -1)
        {
            parameters.length[0] = 0x80;
        }
        n_octet_length_part += 2;
        if (n_octet_length_part > 127)
        {
            return 4; ///ERROR    the number of octets required for length encoding has exceeded the standard
        }
    }
    else
    {
        n_octet_length_part = 1;
    }

    uint8_t length_part[n_octet_length_part];

    if (n_octet_length_part == 1)
    {
        length_part[0] = parameters.length[0];
    }
    else
    {
        length_part[0] = (1 << 7) + (n_octet_length_part - 1);
        for(int i = 1; i < n_octet_length_part; i++)
        {
            length_part[i] = parameters.length[i - 1];
        }
    }
    ///-------------------------------END_LENGTH_PART-------------------------------

    ///-------------------------------DATA_PART-------------------------------

    uint8_t *data_part = nullptr;
    uint32_t n_octet_data_part = 0;
    if (length_part[0] != 0x80)
    {
        switch(parameters.tag_number) {
            case INTEGER: {
                if(parameters.data == nullptr)
                {
                    return 5; ///ERROR    there is no data to encode
                }
                n_octet_data_part = parameters.length[0]
                                    + (parameters.length[1] << 8)
                                    + (parameters.length[2] << 8)
                                    + (parameters.length[3] << 8);

                data_part = new uint8_t[n_octet_data_part];
                data_part[0] = parameters.data[0];
                data_part[1] = parameters.data[1];
                data_part[2] = parameters.data[2];
                data_part[3] = parameters.data[3];
                break;
            }
            default:
            {
                #ifndef CHECK
                    return 6; ///ERROR
                #endif
            }
        }
    }

    ///-------------------------------END_DATA_PART-------------------------------


    ///-------------------------------CONVERT_BLOCK-------------------------------
    /// It is necessary to write data to the source array during the encoding process
    (*full_data_bit) = new uint8_t [  n_octet_service_part
                                    + n_octet_length_part
                                    + n_octet_data_part];
    int i = 0;
    for (; i < n_octet_service_part; i++)
    {
        (*full_data_bit)[i] = service_part[i];
    }
    int j = 0;
    for (; j < n_octet_length_part; j++)
    {
        (*full_data_bit)[i + j] = length_part[j];
    }

    for (int c = 0; c < n_octet_data_part; c++)
    {
        if(data_part == nullptr)
        {
            return 5; ///ERROR    ATTENTION
            /// I put the same number for the error,
            /// because it should not be in future updates,
            /// and because it means the same thing
        }
        (*full_data_bit)[i + j + c] = data_part[c];
    }

    delete [] data_part;
    ///-------------------------------END_CONVERT_BLOCK-------------------------------
    encod_length = n_octet_service_part + n_octet_length_part + n_octet_data_part;
    return 0;
}

int X_690_BER_Tag::decoding(const uint8_t * full_data_bit, const uint32_t planned_length)
{
    if (full_data_bit == nullptr)
    {
        return 7; ///ERROR    there is no data for decoding
    }
    if (planned_length < 2)
    {
        return 8; ///ERROR    the length of the incoming message is less than the minimum
    }

    ///-------------------------------SERVICE_PART-------------------------------
    parameters.tag_class = static_cast<Tag_class>(full_data_bit[0] >> 6);

    uint32_t real_length = 1;
    ///-------------------------------ID_PART-------------------------------
    {
        uint8_t first_id = (full_data_bit[0] << 3);

        int id = 0;

        if (first_id == 0xF8)
        {
            uint8_t buffer;
            while (full_data_bit[real_length] >= 0x80)
            {
                real_length++;
                if (real_length == planned_length - 1)
                {
                    return 9; ///ERROR    the decoded message ended during the processing of the service part
                }
            }
            if (real_length > 6) {
                return 10; ///ERROR    the length of the service part of the decoded message is longer than the standard
            }
            for (int j = 0; j < real_length; j++) {
                buffer = (full_data_bit[real_length - j] << 1);
                id += ((buffer >> 1) << (7 * j));
            }
            real_length++;
        }
        else
        {
            id = (first_id >> 3);
        }
        parameters.tag_number = static_cast<Tag_number>(id);
    }
    ///-------------------------------END_SERVICE_PART-------------------------------

    ///-------------------------------LENGTH_PART-------------------------------
    {
        if (real_length == planned_length)
        {
            return 11; ///ERROR    the decoded message ended before the length was processed
        }
        if (full_data_bit[real_length] == 0x80)
        {
            parameters.length[0] = 0;
        }
        else if (full_data_bit[real_length] > 0x80)
        {

            uint8_t length_message = full_data_bit[real_length] - 0x80;
            int i = 0;
            for (; i < length_message; i++)
            {
                if (real_length + 1 + i == planned_length)
                {
                    return 12; ///ERROR    the decoded message ended in the process of processing the length
                }
                parameters.length[i] = full_data_bit[real_length + 1 + i];
            }
            real_length += i;
        }
        else
        {
            parameters.length[0] = full_data_bit[real_length];
        }
    }
    ///-------------------------------END_LENGTH_PART-------------------------------

    ///-------------------------------DATA_PART-------------------------------
    switch(parameters.tag_number)
    {
        case INTEGER:
        {
            if (real_length + 4 >= planned_length)
            {
                return 13; ///ERROR    the decoded message ended during data processing
            }
            parameters.data = new uint8_t [4];
            parameters.data[0] = full_data_bit[real_length + 1];
            parameters.data[1] = full_data_bit[real_length + 2];
            parameters.data[2] = full_data_bit[real_length + 3];
            parameters.data[3] = full_data_bit[real_length + 4];
            break;
        }
        default:
        {
            printf(" %d\n", parameters.tag_number);
            return 14; ///ERROR    the type of decoded data is not supported
        }
    }
    ///-------------------------------END_DATA_PART-------------------------------
    return 0;
}

void X_690_BER_Tag::native_int_data(const int int_data)
{
    parameters.data = new uint8_t [4];
    parameters.length[0] = 4;
    parameters.data[0] = int_data;
    parameters.data[1] = (int_data >> 8);
    parameters.data[2] = (int_data >> 16);
    parameters.data[3] = (int_data >> 24);
}

void X_690_BER_Tag::software_int_data(const int int_data)
{
    parameters.data = new uint8_t [4];
    parameters.length[0] = 4;
    if (int_data >= 0)
    {
        uint32_t u_data = int_data;
        parameters.data[0] = u_data;
        parameters.data[1] = (u_data >> 8);
        parameters.data[2] = (u_data >> 16);
        parameters.data[3] = (u_data >> 24);
    }
    else
    {
        uint32_t u_data = (-int_data);
        u_data = ~u_data + 1;
        parameters.data[0] = u_data;
        parameters.data[1] = (u_data >> 8);
        parameters.data[2] = (u_data >> 16);
        parameters.data[3] = (u_data >> 24);
    }
}

int X_690_BER_Tag::native_data_int()
{
    if (parameters.data == nullptr)
    {
        last_error = 1;
        return 0; ///ERROR
    }

    int data =  parameters.data[0]
             + (parameters.data[1] << 8)
             + (parameters.data[2] << 16)
             + (parameters.data[3] << 24);
    return data;
}

int X_690_BER_Tag::software_data_int()            /// Here you can think about optimizing using if,
{                                                 /// separating the negative and other numbers
    if (parameters.data == nullptr)
    {
        last_error = 1;
        return 0; ///ERROR
    }

    uint32_t main_data = (parameters.data[3] >> 7);
    main_data = (main_data << 31);
    uint32_t side_data = (parameters.data[3] << 25)
                       + (parameters.data[2] << 17)
                       + (parameters.data[1] << 9)
                       + (parameters.data[0] << 1);

    side_data = (side_data >> 1);
    int data = side_data - main_data;
    return data;
}

#ifdef CHECK
void X_690_BER_Tag::set_length(uint8_t index, uint8_t meaning)
{
    parameters.length[index] = meaning;
}
uint8_t X_690_BER_Tag::get_length(uint8_t index)
{
    return parameters.length[index];
}
#endif