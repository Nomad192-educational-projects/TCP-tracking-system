#ifndef TEST_CPP
#define TEST_CPP

#include "message.h"
#include <cstdio>
#include <cstdint>

class Test_Tag
{
public:
    static void release()
    {
    #ifdef CHECK
        printf("Release test: ");
        printf("Turn off the test mode!\nERROR\n");
    #else
        printf("Release test: START\n");
        software_int(64);
        printf("Release test: END\n");
    #endif
    }

    static void full()
    {
    #ifdef CHECK
        printf("Full test: TEST MODE START\n");
        t_class();
        number(64);
        class_and_number();
        length();
        printf("Full test: TEST MODE END\n");
    #else
        printf("Full test: normal MODE START\n");
        native_int(64);
        software_int(64);
        software_native_int(64);
        native_software_int(64);
        printf("Full test: normal MODE END\n");
    #endif
    }

#ifdef CHECK
    void t_class ()      /// With blocked checks (presence of the CHECK directive - #define CHECK 0)
    {
        printf("Testing tag class...\n");
        for (int i = 0; i < 4; i++)
        {
            X_690_BER_Tag out_tag(static_cast<X_690_BER_Tag::Tag_class>(i),
                                  X_690_BER_Tag::Tag_number::DATE_TIME);
            out_tag.set_length(0, 25);
            uint8_t* out_message_bit = nullptr;
            uint32_t length = out_tag.encoding(&out_message_bit);

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, &length);
            if (in_tag.get_tag_class() != i)
            {
                printf("Error tag class: i=%llu class=%llu\n", i, in_tag.get_tag_class());
                //return;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            X_690_BER_Tag out_tag(static_cast<X_690_BER_Tag::Tag_class>(i),
                                  X_690_BER_Tag::Tag_number::TIME);
            out_tag.set_length(0, 25);
            uint8_t* out_message_bit = nullptr;
            uint32_t length = out_tag.encoding(&out_message_bit);

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, &length);
            if (in_tag.get_tag_class() != i)
            {
                printf("Error tag class: i=%llu class=%llu\n", i, in_tag.get_tag_class());
                //return;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            X_690_BER_Tag out_tag(static_cast<X_690_BER_Tag::Tag_class>(i),
                                  X_690_BER_Tag::Tag_number::SEQUENCE);
            out_tag.set_length(0, 250);
            uint8_t* out_message_bit = nullptr;
            uint32_t length = out_tag.encoding(&out_message_bit);

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, &length);
            if (in_tag.get_tag_class() != i)
            {
                printf("Error tag class: i=%llu class=%llu\n", i, in_tag.get_tag_class());
                //return;
            }
        }
        printf("OK\n");
    }

    void number (const uint8_t base)   /// With blocked checks
    {
        printf("Testing tag number based on %d...\n", base);
        if(base == 0)
        {
            return; ///ERROR
        }
        if(base < 1)
        {
            for (uint32_t i = 0; i < 2147483647; i++)
            {
                X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::CONTEXT_SPECIFIC,
                                      static_cast<X_690_BER_Tag::Tag_number>(i));
                out_tag.set_length(0, 250);
                uint8_t* out_message_bit = nullptr;
                uint32_t length = out_tag.encoding(&out_message_bit);

                X_690_BER_Tag in_tag;
                in_tag.decoding(out_message_bit, &length);
                if (in_tag.get_tag_number() != i)
                {
                    printf("Error tag number: i=%llu number=%llu\n", i, in_tag.get_tag_number());
                    //return;
                }
                if(i%4194304 == 0)
                {
                    double f = i;
                    printf("%.2f%\n", (f/2147483647)/2 * 100);
                }
            }
            for (uint32_t i = 0; i < 2147483647; i++)
            {
                X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::CONTEXT_SPECIFIC,
                                      static_cast<X_690_BER_Tag::Tag_number>(i));
                out_tag.set_length(0, 25);
                uint8_t* out_message_bit = nullptr;
                uint32_t length = out_tag.encoding(&out_message_bit);

                X_690_BER_Tag in_tag;
                in_tag.decoding(out_message_bit, &length);
                if (in_tag.get_tag_number() != i)
                {
                    printf("Error tag number: i=%llu number=%llu\n", i, in_tag.get_tag_number());
                    //return;
                }
                if(i%4194304 == 0)
                {
                    double f = i;
                    printf("%.2f%\n", 50 + (f/2147483647)/2 * 100);
                }
            }
        }
        else
        {
            for (uint32_t i = 0; i < 2147483647; i+=base)
            {
                X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::CONTEXT_SPECIFIC,
                                      static_cast<X_690_BER_Tag::Tag_number>(i));
                out_tag.set_length(0, 250);
                uint8_t* out_message_bit = nullptr;
                uint32_t length = out_tag.encoding(&out_message_bit);

                X_690_BER_Tag in_tag;
                in_tag.decoding(out_message_bit, &length);
                if (in_tag.get_tag_number() != i)
                {
                    printf("Error tag number: i=%llu number=%llu\n", i, in_tag.get_tag_number());
                    //return;
                }
                if(i%(4194304*base) == 0)
                {
                    double f = i;
                    printf("%.2f%\n", (f/2147483647)/2 * 100);
                }
            }
            for (uint32_t i = base - 1; i < 2147483647; i+=base)
            {
                X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::CONTEXT_SPECIFIC,
                                      static_cast<X_690_BER_Tag::Tag_number>(i));
                out_tag.set_length(0, 25);
                uint8_t* out_message_bit = nullptr;
                uint32_t length = out_tag.encoding(&out_message_bit);

                X_690_BER_Tag in_tag;
                in_tag.decoding(out_message_bit, &length);
                if (in_tag.get_tag_number() != i)
                {
                    printf("Error tag number: i=%llu number=%llu\n", i, in_tag.get_tag_number());
                    //return;
                }
                if((i - (base - 1))%(4194304*base) == 0)
                {
                    double f = i;
                    printf("%.2f%\n", 50 + (f/2147483647)/2 * 100);
                }
            }
            printf("OK\n");
        }
    }

    void class_and_number()    /// With blocked checks
    {
        printf("Testing tag class and number...\n");
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 36; j++)
            {
                if(j == 2)
                {
                    j++;
                }
                X_690_BER_Tag out_tag(static_cast<X_690_BER_Tag::Tag_class>(i),
                                      static_cast<X_690_BER_Tag::Tag_number>(j));

                out_tag.set_length(0, 25);
                uint8_t *out_message_bit = nullptr;
                uint32_t length = out_tag.encoding(&out_message_bit);

                X_690_BER_Tag in_tag;
                in_tag.decoding(out_message_bit, &length);
                if (in_tag.get_tag_class() != i || in_tag.get_tag_number() != j)
                {
                    printf("Error tag class or number: i=%llu class=%llu, j=%d number=%llu\n",
                           i, in_tag.get_tag_class(), j, in_tag.get_tag_number());
                    //return;
                }
            }
        }
        printf("OK\n");
    }

    void length()  /// With blocked checks
    {
        printf("Testing tag length...\n");
        for (uint8_t i = 0; i < 126; i++)
        {
            for(int j = 0; j < 256; j++)
            {

                X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::CONTEXT_SPECIFIC,
                                      X_690_BER_Tag::Tag_number::BIT_STRING);
                for (int c = 0; c < i; c++)
                {
                    out_tag.set_length(c, 0xFF);
                }
                out_tag.set_length(i, j);
                uint8_t* out_message_bit = nullptr;
                uint32_t length = out_tag.encoding(&out_message_bit);

                X_690_BER_Tag in_tag;
                in_tag.decoding(out_message_bit, &length);
                for (int c = 0; c <= i; c++)
                {
                    if (in_tag.get_length(c) != out_tag.get_length(c))
                    {
                        printf("Error tag length: i=%d j=%d c=%d out=%d in=%d\n", i, j, c,
                               out_tag.get_length(c),
                               in_tag.get_length(c));
                        //return;
                    }
                }
            }
            if(i%21 == 0)
            {
                double f = i;
                printf("%.2f%\n", (f/126)/2 * 100);
            }
        }
        for (uint8_t i = 0; i < 126; i++)
        {
            for(int j = 0; j < 256; j++)
            {
                X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                      X_690_BER_Tag::Tag_number::EXTERNAL);
                for (int c = 0; c < i; c++)
                {
                    out_tag.set_length(c, 0xFF);
                }
                out_tag.set_length(i, j);
                uint8_t* out_message_bit = nullptr;
                uint32_t length = out_tag.encoding(&out_message_bit);

                X_690_BER_Tag in_tag;
                in_tag.decoding(out_message_bit, &length);
                for (int c = 0; c <= i; c++)
                {
                    if (in_tag.get_length(c) != out_tag.get_length(c))
                    {
                        printf("Error tag length: i=%d j=%d c=%d out=%d in=%d\n", i, j, c,
                               out_tag.get_length(c),
                               in_tag.get_length(c));
                        //return;
                    }
                }
            }
            if(i%21 == 0)
            {
                double f = i;
                printf("%.2f%\n", 50 + (f/126)/2 * 100);
            }
        }
        printf("OK\n");
    }
#else
    static void native_int(const uint8_t base)    /// With unblocked checks (absence of the CHECK directive - #define CHECK 0)
    {
        printf("Testing tag native int based on %d...\n", base);
        if(base == 0)
        {
            return; ///ERROR
        }
        int i = 0;
        for (; i >= 0; i+=base)
        {
            X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                  X_690_BER_Tag::Tag_number::INTEGER);
            uint8_t* out_message_bit = nullptr;
            out_tag.native_int_data(i);
            out_tag.encoding(&out_message_bit);
            uint32_t length = out_tag.get_length();

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, length);
            int data = in_tag.native_data_int();
            if(data != i)
            {
                printf("Error tag native int: i=%d data=%d\n", i, data);
                //return;
            }
            if(i%(4194304*base) == 0)
            {
                double f = i;
                printf("%.2f%\n", (f/2147483647)/2 * 100);
            }
        }
        for (; i < 0; i+=base)
        {
            X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                  X_690_BER_Tag::Tag_number::INTEGER);
            uint8_t* out_message_bit = nullptr;
            out_tag.native_int_data(i);
            out_tag.encoding(&out_message_bit);
            uint32_t length = out_tag.get_length();

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, length);
            int data = in_tag.native_data_int();
            if(data != i)
            {
                printf("Error tag native int: i=%d data=%d\n", i, data);
                //return;
            }
            if(i%(4194304*base) == 0)
            {
                double f = 2147483647 + i;
                printf("%.2f%\n", 50 + (f/2147483647)/2 * 100);
            }
        }
        printf("OK\n");
    }

    static void software_int(const uint8_t base)  /// With unblocked checks
    {
        printf("Testing tag software int based on %d...\n", base);
        if(base == 0)
        {
            return; ///ERROR
        }
        int i = 0;
        for (; i >= 0; i+=base)
        {
            X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                  X_690_BER_Tag::Tag_number::INTEGER);
            uint8_t* out_message_bit = nullptr;
            out_tag.software_int_data(i);
            out_tag.encoding(&out_message_bit);
            uint32_t length = out_tag.get_length();

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, length);
            int data = in_tag.software_data_int();
            if(data != i)
            {
                printf("Error tag software int: i=%d data=%d\n", i, data);
                //return;
            }
            if(i%(4194304*base) == 0)
            {
                double f = i;
                printf("%.2f%\n", (f/2147483647)/2 * 100);
            }
        }
        for (; i < 0; i+=base)
        {
            X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                  X_690_BER_Tag::Tag_number::INTEGER);
            uint8_t* out_message_bit = nullptr;
            out_tag.software_int_data(i);
            out_tag.encoding(&out_message_bit);
            uint32_t length = out_tag.get_length();

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, length);
            int data = in_tag.software_data_int();
            if(data != i)
            {
                printf("Error tag software int: i=%d data=%d\n", i, data);
                //return;
            }
            if(i%(4194304*base) == 0)
            {
                double f = 2147483647 + i;
                printf("%.2f%\n", 50 + (f/2147483647)/2 * 100);
            }
        }

        printf("OK\n");
    }

    static void software_native_int(const uint8_t base)   /// With unblocked checks
    {
        printf("Testing tag software native int based on %d...\n", base);
        if(base == 0)
        {
            return; ///ERROR
        }
        int i = 0;
        for (; i >= 0; i+=base)
        {
            X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                  X_690_BER_Tag::Tag_number::INTEGER);
            uint8_t* out_message_bit = nullptr;
            out_tag.software_int_data(i);
            out_tag.encoding(&out_message_bit);
            uint32_t length = out_tag.get_length();

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, length);
            int data = in_tag.native_data_int();
            if(data != i)
            {
                printf("Error tag software->native int: i=%d data=%d\n", i, data);
                //return;
            }
            if(i%(4194304*base) == 0)
            {
                double f = i;
                printf("%.2f%\n", (f/2147483647)/2 * 100);
            }
        }
        for (; i < 0; i+=base)
        {
            X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                  X_690_BER_Tag::Tag_number::INTEGER);
            uint8_t* out_message_bit = nullptr;
            out_tag.software_int_data(i);
            out_tag.encoding(&out_message_bit);
            uint32_t length = out_tag.get_length();

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, length);
            int data = in_tag.native_data_int();
            if(data != i)
            {
                printf("Error tag software->native int: i=%d data=%d\n", i, data);
                //return;
            }
            if(i%(4194304*base) == 0)
            {
                double f = 2147483647 + i;
                printf("%.2f%\n", 50 + (f/2147483647)/2 * 100);
            }
        }

        printf("OK\n");
    }

    static void native_software_int(const uint8_t base)   /// With unblocked checks
    {
        printf("Testing tag native software int based on %d...\n", base);
        if (base == 0) {
            return; ///ERROR
        }
        int i = 0;
        for (; i >= 0; i += base) {
            X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                  X_690_BER_Tag::Tag_number::INTEGER);
            uint8_t *out_message_bit = nullptr;
            out_tag.native_int_data(i);
            out_tag.encoding(&out_message_bit);
            uint32_t length = out_tag.get_length();

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, length);
            int data = in_tag.software_data_int();
            if (data != i) {
                printf("Error tag native->software int: i=%d data=%d\n", i, data);
                //return;
            }
            if (i % (4194304 * base) == 0) {
                double f = i;
                printf("%.2f%\n", (f / 2147483647) / 2 * 100);
            }
        }
        for (; i < 0; i += base) {
            X_690_BER_Tag out_tag(X_690_BER_Tag::Tag_class::UNIVERSAL,
                                  X_690_BER_Tag::Tag_number::INTEGER);
            uint8_t *out_message_bit = nullptr;
            out_tag.native_int_data(i);
            out_tag.encoding(&out_message_bit);
            uint32_t length = out_tag.get_length();

            X_690_BER_Tag in_tag;
            in_tag.decoding(out_message_bit, length);
            int data = in_tag.software_data_int();
            if (data != i) {
                printf("Error tag native->software int: i=%d data=%d\n", i, data);
                //return;
            }
            if (i % (4194304 * base) == 0) {
                double f = 2147483647 + i;
                printf("%.2f%\n", 50 + (f / 2147483647) / 2 * 100);
            }
        }

        printf("OK\n");
    }
    #endif
};
////Make the number of errors in the tests///
#endif //TEST_CPP