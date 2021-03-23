#pragma once

#include <cassert>
#include <cfloat>
#include <limits>
#include <cmath>

namespace FundamentalTypes {
  namespace VoidType {
    // Void
    //  A type with an empty set of values.
    //  - Is an incomplete type that cannot be completed (consequently, objects of type void are disallowed).
    //  - There are no arrays of void, nor references to void.
    //  - However, pointers to void and functions returning type void are permitted.

    // std::nullptr_t (defined in header <cstddef>, since c++11; typedef decltype(nullptr) nullptr_t;)
    // - Is the type of the null pointer literal, nullptr.
    // - Is a distinct type that is not itself a pointer type or a pointer to member type.
    // - Its values are null pointer constant (see NULL)
    // - May be implicitly converted to any pointer and pointer to member type.

    void test() {
      assert(sizeof(void*) == sizeof(std::nullptr_t));
    }
  }

 namespace DataModels {
    // 32-bit OS:
    //  1. LP32 or 2/4/4 (int is 16-bit, long and pointer are 32-bit)   - Win16 API
    //  2. ILP32 or 4/4/4 (int, long, and pointer are 32-bit);          - Win32 API Unix and Unix-like systems (Linux, macOS)

    // 64-bit OS:
    //  1. LLP64 or 4/4/8 (int and long are 32-bit, pointer is 64-bit)  - Win64 API
    //  2. LP64 or 4/8/8 (int is 32-bit, long and pointer are 64-bit)   - Unix and Unix-like systems (Linux, macOS)

    void test() {
      assert(sizeof(int) == 4);
      assert(sizeof(long) == 8);
      assert(sizeof(void*) == 8);
    }
  }

  namespace IntegerTypes {
// Modifiers:
//  1. Signedness:
//    - signed - target type will have signed representation (this is the default if omitted)
//    - unsigned - target type will have unsigned representation
//  2. Size:
//    - short - target type will be optimized for space and will have width of at least 16 bits.
//    - long - target type will have width of at least 32 bits.
//    - long long - target type will have width of at least 64 bits (since C++11).

//  NOTE: integer arithmetic is defined differently for the signed and unsigned integer types.
//  See arithmetic operators, in particular integer overflows.

  std::size_t x;
//  Is the unsigned integer type of the result of:
//  - the sizeof operator
//  - the sizeof... operator
//  - the alignof operator (since C++11)

//    See also Fixed width integer types     (since C++11)

    void print_sizeof_s() {
      // todo: add asserts for DataModels for __WIN32__, __APPLE__, __LINUX__

      printf("sizeof(char)                = %lld\n", sizeof(char));
      printf("sizeof(signed char)         = %lld\n", sizeof(signed char));
      printf("sizeof(unsigned char)       = %lld\n", sizeof(unsigned char));
      printf("sizeof(short)               = %lld\n", sizeof(short));
      printf("sizeof(unsigned short)      = %lld\n", sizeof(unsigned short));
      printf("sizeof(int)                 = %lld\n", sizeof(int));
      printf("sizeof(unsigned int)        = %lld\n", sizeof(unsigned int));
      printf("sizeof(long)                = %lld\n", sizeof(long));
      printf("sizeof(unsigned long)       = %lld\n", sizeof(unsigned long));
      printf("sizeof(long long)           = %lld\n", sizeof(long long));
      printf("sizeof(unsigned long long)  = %lld\n", sizeof(unsigned long long));
      printf("sizeof(float)               = %lld\n", sizeof(float));
      printf("sizeof(double)              = %lld\n", sizeof(double));
      printf("sizeof(long double)         = %lld\n", sizeof(long double));
      printf("sizeof(std::size_t)         = %lld\n", sizeof(std::size_t));
    }

    template <class T>
    using limits = std::numeric_limits<T>;

    void print_range_of_values() {
      // The C++ Standard allowed:
      // -  Any signed integer representation for N-bit signed integers: [ -(2^(N-1) - 1 ; 2^(N-1) - 1]
      //
      printf("The number of bits in a byte is accessible as %d\n", CHAR_BIT);
      printf("char                 limits: [%d, %d]\n",     limits<char>::min(), limits<char>::max());
      printf("unsigned char        limits: [%d, %d]\n",     limits<unsigned char>::min(), limits<unsigned char>::max());
      printf("short                limits: [%d, %d]\n",     limits<short>::min(), limits<short>::max());
      printf("unsigned short       limits: [%d, %d]\n",     limits<unsigned short>::min(), limits<unsigned short>::max());
      printf("int                  limits: [%d, %d]\n",     limits<int>::min(), limits<int>::max());
      printf("unsigned int         limits: [%d, %u]\n",     limits<unsigned int>::min(), limits<unsigned int>::max());
      printf("long                 limits: [%ld, %ld]\n",   limits<long>::min(), limits<long>::max());
      printf("unsigned long        limits: [%lu, %lu]\n",   limits<unsigned long>::min(), limits<unsigned long>::max());
      printf("long long            limits: [%lld, %lld]\n", limits<long long>::min(), limits<long long>::max());
      printf("unsigned long long   limits: [%llu, %llu]\n", limits<unsigned long long>::min(), limits<unsigned long long>::max());
      printf("float                limits: [%E, %E]\n",     limits<float>::min(), limits<float>::max());
      printf("double               limits: [%E, %E]\n",     limits<double>::min(), limits<double>::max());
      printf("long double          limits: [%LE, %LE]\n",   limits<long double>::min(), limits<long double>::max());
    }

    void test() {
      print_sizeof_s();
      print_range_of_values();
    }

  }

  namespace BooleanType {
    // Type, capable of holding one of the two values:
    // - true
    // - false

    void test() {
      // The value of sizeof(bool) is implementation defined and might differ from 1.
      std::size_t bool_size = sizeof(bool);
      std::cout << "Bool size is " << bool_size << '\n';
      assert(bool_size == 1); // !!!!: might differ from 1
    }
  }

  namespace CharacterTypes {
    namespace ASCII {
      // Each character - 7 bit
      // Each character is stored in separate byte
      // Total 2^7 = 128 characters
    }

    namespace Latin1_ISO_8859_1 {
      // Extends ASCII to 8 bits per symbol
      // Introduces a new concept of a CODE PAGE (a set of 256 characters)
      // The software has to keep track of which CODE PAGE it is currently on
      // It is impossible to mix languages over pages
    }

    namespace Unicode {
      // An abstract text view (code table), Operates with CODE POINTS
      // Defines 3 character encoding forms:
      // - UCS   (UTF-8)
      // - UCS-2 (UTF-16)
      // - UCS-4 (UTF-32)

      // Has 17 language-planes of 16 bits per each
      // Plane 0:
      // - Basic Multilingual Plane (BMP)
      // - Total 2^16 = 65536 code points per plane
      // The total size of the Unicode code space is 17 × 65,536 = 1,114,112 code points.

      namespace UTF_8 {
        // Max limit is 2^31 (2,147,483,648) code points (32,768 planes).
        // Can encode 2^21 (2,097,152) code points (32 planes) under the current limit of 4 bytes.

        // - Encodes all the characters in a sequence of bytes of varying length [1..4];
        // - CODE POINT -> from 1 to 4 units (8 bit per unit)
        // - Supports 2^21 (2,097,152) code points (4 bytes max)
        // - ASCII characters [0...127] are expressed in 1 byte
        // - Non-ASCII characters set the high-order bit of the first byte to 1, indicating that 1 or more bytes follow
        // - The continuation bytes start with 10

        // | # {bytes} | First CODE POINT | Last CODE POINT |  Byte 1  |  Byte 2  |  Byte 3  |  Byte 4  | Valid bits |
        // -----------------------------------------------------------------------------------------------------------
        // |     1     |      U+0000      |      U+007F     | 0xxxxxxx |          |          |          |     7      |
        // |     2     |      U+0080      |      U+07FF     | 110xxxxx | 10xxxxxx |          |          |     11     |
        // |     3     |      U+0800      |      U+FFFF     | 1110xxxx | 10xxxxxx | 10xxxxxx |          |     16     |
        // |     4     |      U+10000     |      U+10FFFF   | 11110xxx | 10xxxxxx | 10xxxxxx | 10xxxxxx |     21     |
        // -----------------------------------------------------------------------------------------------------------
      }

      namespace UTF_16 {
        // CODE POINT:
        // - 1 unit  = 16 bit [U+0000, U+10000]
        // - 2 units = 32 bit (use surrogate pairs to express code points outside th BMP)
        // At least 2 bytes per character
      }

      namespace UTF_32 {
        // Fixed size encoding (4 bytes)
        // CODE POINT -> 1 unit = 32 bites
      }

      namespace ByteOrderMark {
        // ByteOrderMark (BOM)
        constexpr int bom = 0xFEFF;
        //  1. UTF-8 don't use (skip) it
        //  2. UTF-16 and UTF-32
        //    - Big-Endian:     if BOM will appear in sequence as 0xFE 0xFF
        //    - Little-Endian:  if BOM will appear in sequence as 0xFF 0xFE

        // |   Arch    |    Byte order    |
        // -------------------------------
        // | Intel x86 |        LE        |
        // |  AMD x64  |        LE        |
        // |    ARM    | bi (LE-default)  |
        // |   AVR 32  |        BE        |
        // -------------------------------


        void test() {

        }

      }
    }

    namespace Char {
      // Signed character representation
      signed char sc;

      // Unsigned character representation. Also used to inspect object representations (raw memory).
      unsigned char uc;

      // Character representation which can be most efficiently processed on the target system.
      // 1. Has the same representation and alignment as either signed char or unsigned char, but is always a DISTINCT type.
      // 2. Multibyte characters strings use this type to represent code units.
      char ch;

      // 3. For every value of type unsigned char in range [0, 255], converting the value to char and then back to unsigned char
      // produces the original value. (since C++11)
      void test_char_uchar() {
        unsigned char x = 254;
        char y = static_cast<char>(x);
        assert((x == 254) && (y == -2));
        char z = x;
        assert((x == 254) && (z == -2));
        unsigned char w = y;
        assert((x == w) && (w == 254));
      }

      // 4. The signedness of char depends on the compiler and the target platform:
      // - The defaults for ARM and PowerPC are typically UNSIGNED
      // - The defaults for x86 and x64 are typically SIGNED
      void test_signed_char() {
        // for x86 and x64
        char x = 128;
        assert(x == -128);
      }
    }

    namespace WChar_T {
      // Wide character representation (see wide strings).
      wchar_t w;

      // 1. Required to be large enough to represent any supported character code point (32 bits on systems that support Unicode).
      // 2. It has the same size, signedness, and alignment as one of the integer types, but is a distinct type.

      void test() {
// A notable EXCEPTION is Windows, where wchar_t is 16 bits and holds UTF-16 code units)
#if defined(_MSC_BUILD)
        assert(sizeof(wchar_t) == 2);
#else
        assert(sizeof(wchar_t) == 4);
#endif
      }
    }

    namespace cxx11 {
      // UTF-16 character representation, required to be large enough to represent any UTF-16 code unit (16 bits).
      // It has the same size, signedness, and alignment as std::uint_least16_t, but is a distinct type.
      char16_t ch16;

      // UTF-32 character representation, required to be large enough to represent any UTF-32 code unit (32 bits).
      // It has the same size, signedness, and alignment as std::uint_least32_t, but is a distinct type.
      char32_t ch32;
    }

    namespace cxx20 {
      // UTF-8 character representation, required to be large enough to represent any UTF-8 code unit (8 bits).
      // It has the same size, signedness, and alignment as unsigned char (char or signed char), but is a distinct type.
      // char8_t ch8;
    }

    void test() {
      Char::test_char_uchar();
      Char::test_signed_char();
      WChar_T::test();

      //  C++ Standard guarantees that
      assert((1 == sizeof(char)) <= sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long));
    }
  }

  namespace FloatingTypes {
    // https://en.wikipedia.org/wiki/IEEE_754

    // single precision floating point type (32 bit)
    float f;
    // sign: 1 bit
    // mantissa: 23 bit
    // exponent: 8 bit

    double d; // double precision floating point type (64 bit)
    // sign: 1 bit
    // mantissa: 52 bit
    // exponent: 11 bit

    // todo: check
    namespace casts {
      typedef union {
        float f;
        struct {
          uint32_t mantissa : 23;
          uint32_t exponent : 8;
          uint32_t sign : 1;
        } parts;
      } float_cast;

      typedef union {
        double d;
        struct {
          uint64_t mantissa : 52;
          uint32_t exponent : 11;
          uint32_t sign : 1;
        } parts;
      } double_cast;
    }

    // Extended precision floating point type. Does not necessarily map to types mandated by IEEE-754.
    // Usually 80-bit x87 floating point type on x86 and x86-64 architectures.
    long double ld;

    template <class T>
    void inf_vs_max() {
      T max = std::numeric_limits<T>::max();
      T inf = std::numeric_limits<T>::infinity();

      if (inf > max) {
        std::cout << __PRETTY_FUNCTION__ << "\n\t";
        std::cout << inf << " is greater than " << max << '\n';
      }
    }

    void test_infinity() {
      assert(std::isinf(INFINITY));
#if not defined(_MSC_BUILD)
      assert(!std::isinf(1));
#endif

      assert(std::numeric_limits<float>::has_infinity);
      assert(std::numeric_limits<double>::has_infinity);
      assert(std::numeric_limits<long double>::has_infinity);

      assert(!std::numeric_limits<int>::has_infinity);
      assert(!std::numeric_limits<char>::has_infinity);

      inf_vs_max<float>();
      inf_vs_max<double>();
      inf_vs_max<long double>();
    }

    void test_negative_zero() {
      // -0.0

      // It compares equal to the positive zero
      float z = 0.0;
      float nz = -0.0;
      assert(z == nz);

#if not defined(_MSC_BUILD)
      // It is meaningful in some arithmetic operations:
      float inf = 1.0f / 0.0f;
      assert(inf == std::numeric_limits<float>::infinity());

      float neg_inf = -1.0f / 0.0f;
      assert(neg_inf == -std::numeric_limits<float>::infinity());
#endif
    }

    void gen_nan(const char* str) {
      double f1 = std::nan(str);
      std::uint64_t f1n;
      std::memcpy(&f1n, &f1, sizeof f1);
      std::cout << "nan(\"" << str << "\") = " << f1 << " (" << std::hex << f1n << ")\n";
    }

    void test_NAN() {
      // 1. Does not compare equal with anything (including itself).
      assert(NAN != NAN);
      // 2. Multiple bit patterns represent NaNs (https://en.cppreference.com/w/cpp/numeric/math/nan)
      gen_nan("1");
      gen_nan("2");
      // 3. C++ takes no special notice of signalling NaNs other than detecting their support by
      // std::numeric_limits::has_signaling_NaN, and treats all NaNs as quiet.
      assert(std::numeric_limits<float>::has_signaling_NaN);
      assert(std::numeric_limits<double>::has_signaling_NaN);
      assert(!std::numeric_limits<int>::has_signaling_NaN);
    }

    void test() {
      assert(sizeof(f) == 4);
      assert(sizeof(d) == 8);
#if defined(_MSC_BUILD)
      assert(sizeof(ld) == 8);
#else
      assert(sizeof(ld) == 16);
#endif
      test_infinity();
      test_negative_zero();
      test_NAN();

      casts::float_cast f1;
      f1.f = 0.15625;
      printf("sign = %u\n", f1.parts.sign);
      printf("exponent = %x\n", f1.parts.exponent);
      printf("mantisa = %x\n", f1.parts.mantissa);

      casts::double_cast d2;
      d2.d = 0.15625;
      printf("sign = %x\n", d2.parts.sign);
      printf("exponent = %x\n", d2.parts.exponent);
      printf("mantisa = %x\n", d2.parts.mantissa);
    }
  }

  void test() {
    VoidType::test();
    IntegerTypes::test();
    BooleanType::test();
    CharacterTypes::test();
    FloatingTypes::test();
  }
}