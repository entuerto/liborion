//  test-encoding.cpp
//
//  Created by Tomas Palazuelos on 2016-06-29.
//  Copyright © 2016 Tomas Palazuelos. All rights reserved.
//
#include <orion/Encoding.h>
#include <orion/TestUtils.h>

using namespace orion;
using namespace orion::encoding;
using namespace orion::unittest;

TEST_SUITE(OrionCore)
{
//----------------------------------------------------------------------------
// Tests
//----------------------------------------------------------------------------
TEST(TestBigEndian_to_uint16)
{
   uint8_t b[] = {1, 0};

   uint16_t v16 = BigEndian::to_uint16(b);

   EXPECT(v16 == 256);
}

TEST(TestBigEndian_put_uint16)
{
   std::array<uint8_t, 2> b;
   std::array<uint8_t, 2> result{1, 0};

   uint16_t v16 = 256;

   BigEndian::put_uint16(v16, b);

   EXPECT(b == result);
}

TEST(TestLittleEndian_to_uint16)
{
   uint8_t b[] = {1, 0}; // 256

   uint16_t v16 = LittleEndian::to_uint16(b);

   EXPECT(v16 == 1);
}

TEST(TestLittleEndian_put_uint16)
{
   std::array<uint8_t, 2> b;
   std::array<uint8_t, 2> result{0, 1};

   uint16_t v16 = 256;

   LittleEndian::put_uint16(v16, b);

   EXPECT(b == result);
}

std::array<int64_t, 18> numbers64 = {
   -9223372036854775808ll, // int64_t(-1 << 63),
   -9223372036854775807ll, // int64_t(-1 << 63) + 1,
   -1,
   0,
   1,
   2,
   10,
   20,
   63,
   64,
   65,
   127,
   128,
   129,
   255,
   256,
   257,
   9223372036854775807ll // int64_t(1 << 63) - 1,
};

std::array<int32_t, 18> numbers32 = {
   -2147483648, // (-1 << 31),
   -2147483647, // (-1 << 31) + 1,
   -1,
   0,
   1,
   2,
   10,
   20,
   63,
   64,
   65,
   127,
   128,
   129,
   255,
   256,
   257,
   2147483647 // (1 << 31) - 1,
};

TEST(TestEncVarint32_uint32)
{
   std::array<uint8_t, 10> buf;

   for (uint32_t x: numbers32)  
   {
      int n = enc_varint<uint32_t>(x, buf.data());

      uint32_t y = dec_varint(buf.data(), n);

      EXPECT(x == y);
   }
}

TEST(TestEncVarint64_uint64)
{
   std::array<uint8_t, 12> buf;

   for (uint64_t x: numbers64)  
   {
      int n = enc_varint<uint64_t>(x, buf.data());

      uint64_t y = dec_varint<uint64_t>(buf.data(), n);
 
      EXPECT(x == y);
   }

}
  
TEST(TestEncVarint64_int64)
{
   std::array<uint8_t, 12> buf;

   for (int64_t x: numbers64)  
   {
      int n = enc_varint<int64_t>(x, buf.data());

      int64_t y = dec_varint<int64_t>(buf.data(), n); 

      EXPECT(x == y);
   }

}

/*
TEST(TestEncVarint64_uint64)
{
   std::array<uint8_t, 12> buf;

   std::cout << "-- TestEncVarint64_uint64 -- \n";

   //int64_t x = -9223372036854775807;
   int64_t x = -1;

   std::cout << "int64_t  x = " << x << "\n";
   std::cout << "uint64_t x =  " << uint64_t(x) << "\n"; 

   int n = enc_varint<int64_t>(x, buf.data());

   int64_t y = dec_varint<int64_t>(buf.data(), n);
 
   std::cout << "(n = " << n << ")\t"  
             << "x = " << x << "\t y = " << y << "\n";

   EXPECT(x == int64_t(y));

   //std::cout << (uint64_t(1 << 63) - 1) << "\n";
}
*/
}
