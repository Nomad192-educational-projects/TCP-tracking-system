#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdio>
#include <cstdint>

//#define CHECK 0;

class X_690_BER_Tag
{
public:
    enum Tag_class
    {
        UNIVERSAL,          ///	Types that are defined only in X. 690 and have the same meaning in all applications
        APPLICATION,        /// Types whose meaning varies depending on the application
        CONTEXT_SPECIFIC,   /// Types whose meaning depends on this composite type
        PRIVATE             /// Types whose meaning depends on the specific organization
    };

    enum Tag_number
    {
        EOC,                /// 0   0   Primitive   End-Of-Content
        BOOLEAN,			/// 1   1   Primitive
        INTEGER,            /// 2   2   Primitive
        BIT_STRING,         /// 3   3   Two
        STRING_OCTETS,      /// 4   4   Two
        T_NULL,             /// 5   5   Primitive   NULL
        OBJECT_IDENTIFIER,  /// 6   6   Primitive
        OBJECT_DESCRIPTOR,  /// 7   7   Two         Object Descriptor
        EXTERNAL,           /// 8   8   Constructed
        REAL,               /// 9   9   Primitive   Float
        ENUMERATED,         /// 10  A   Primitive
        EMBEDDED_PDV,       /// 11  B   Constructed EMBEDDED PDV
        UTF8STRING,         /// 12  C   Two         UTF8String
        RELATIVE_OID,       /// 13  D   Primitive   RELATIVE-OID
        TIME,               /// 14  E   Primitive
        RESERVED,           /// 15  F   ---------
        SEQUENCE,           /// 16  10  Constructed SEQUENCE and SEQUENCE OF
        SET,                /// 17  11  Constructed SET and SET OF
        NUMERIC_STRING,     /// 18  12  Two         NumericString
        PRINTABLE_STRING,   /// 19  13  Two         PrintableString
        T61STRING,          /// 20  14  Two         T61String
        VIDEOTEX_STRING,    /// 21  15  Two         VideotexString
        IA5_STRING,         /// 22  16  Two         IA5String
        UTC_TIME,           /// 23  17  Two         UTCTime
        GENERALIZED_TIME,   /// 24  18  Two         GeneralizedTime
        GRAPHIC_STRING,     /// 25  19  Two         GraphicString
        VISIBLE_STRING,     /// 26  1A  Two         VisibleString
        GENERAL_STRING,     /// 27  1B  Two         GeneralString
        UNIVERSAL_STRING,   /// 28  1C  Two         UniversalString
        CHARACTER_STRING,   /// 29  1D  Constructed CHARACTER STRING
        BMP_STRING,         /// 30  1E  Two         BMPString

        DATE,				/// 31  1F  Primitive
        TIME_DAY,           /// 32  20  Primitive   TIME OF THE DAY
        DATE_TIME,          /// 33  21  Primitive   DATE-TIME
        DURATION,           /// 34  22  Primitive
        OID_IRI,            /// 35  23  Primitive   OID-IRI
        RELATIVE_OID_IRI    /// 36  24  Primitive   RELATIVE-OID-IRI
    };

private:
    #define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

    /// 0 - Primitive
    /// 1 - Constructed
    /// 2 - Both
    /// 3 - ---------   (Reserved)

    uint8_t P_C[37] = /// Primitive or Constructed
    {0, 0, 0, 2, 2, 0, 0, 2,    ///0  - 7
     1, 0, 0, 1, 1, 0, 0, 3,    ///8  - F
     1, 1, 2, 2, 2, 2, 2, 2,    ///10 - 17
     2, 2, 2, 2, 2, 1, 2,       ///18 - 1E - Standard ASN.1
     0, 0, 0, 0, 0, 0       };  ///1F - 24

    struct Parameters
    {
        enum Tag_class tag_class;
        enum Tag_number tag_number;
        uint8_t length[126];
        uint8_t * data;
    };

    uint32_t encod_length;
    int last_error;

    struct Parameters parameters{};

    void ini_length_zero();

    uint8_t calculating_number_octets_service_part() const;

public:
    X_690_BER_Tag()
    {
        parameters.data = nullptr;
        ini_length_zero();
        encod_length = 0;
        last_error = 0;
    }
    X_690_BER_Tag(Tag_class new_tag_class, Tag_number new_tag_number)
    {
        parameters.data = nullptr;
        ini_length_zero();
        parameters.tag_class = new_tag_class;
        parameters.tag_number = new_tag_number;
        encod_length = 0;
        last_error = 0;
    }
    ~X_690_BER_Tag()
    {
        delete [] parameters.data;
    }
    int get_last_error() const    ///Now this variable is used only for the "native_data_int"
                            /// and "software_data_int" methods,
                            /// it will be good to maintain it for each method
    {
        return last_error;
    }
    int encoding(uint8_t ** full_data_bit);
    uint32_t get_length() const
    {
        return encod_length;
    }
    int decoding(const uint8_t * full_data_bit, uint32_t planned_length);
    /*void set_tag_class(Tag_class new_tag_class)
    {
        parameters.tag_class = new_tag_class;
    }
    void set_tag_number(Tag_number new_tag_number)
    {
        parameters.tag_number = new_tag_number;
    }*/
    int get_tag_class() const
    {
        return parameters.tag_class;
    }

    int get_tag_number() const
    {
        return parameters.tag_number;
    }

    void native_int_data(int int_data);
    void software_int_data(int int_data);

    int native_data_int();
    int software_data_int();

#ifdef CHECK
    void set_length(uint8_t index, uint8_t meaning);
    uint8_t get_length(uint8_t index);
#endif
};

#endif //MESSAGE_H