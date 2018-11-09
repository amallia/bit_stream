/**
 * Copyright 2018-present Antonio Mallia <me@antoniomallia.it>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "bit_stream/bit_istream.hpp"
#include "bit_stream/bit_ostream.hpp"
#include "bit_stream/util.hpp"

TEST(bit_stream, bits) {

    EXPECT_EQ(bits(0b0), 1);
    EXPECT_EQ(bits(0b1), 1);
    EXPECT_EQ(bits(0b10), 2);
    EXPECT_EQ(bits(0b11), 2);
    EXPECT_EQ(bits(0b100), 3);
    EXPECT_EQ(bits((1ULL << 63)), 64);
    EXPECT_EQ(bits((1ULL << 63) - 1), 63);
}

TEST(bit_stream, bytes) {

    EXPECT_EQ(bytes(0), 1);
    EXPECT_EQ(bytes(255), 1);
    EXPECT_EQ(bytes(256), 2);
    EXPECT_EQ(bytes(65535), 2);
    EXPECT_EQ(bytes(65536), 3);
}

TEST(bit_stream, test) {
    {
        auto value = 100;
        std::vector<uint8_t> buffer(bytes(value));
        bit_ostream          bo(buffer.data());
        bit_istream          bi(buffer.data());
        bo.write(value, 8);
        auto decoded_value = bi.read(8);
        EXPECT_EQ(decoded_value, value);
    }
    {
        auto value = 100;
        std::vector<uint8_t> buffer(bytes(value));
        bit_ostream          bo(buffer.data());
        bit_istream          bi(buffer.data());
        bo.write_unary(value);
        auto decoded_value = bi.read_unary();
        EXPECT_EQ(decoded_value, value);
    }
    {
        auto value = 100;
        std::vector<uint8_t> buffer(bytes(value));
        bit_ostream          bo(buffer.data());
        bit_istream          bi(buffer.data());
        bo.write_elias_gamma(value);
        auto decoded_value = bi.read_elias_gamma();
        EXPECT_EQ(decoded_value, value);
    }
    {
        auto value = 100;
        std::vector<uint8_t> buffer(bytes(value));
        bit_ostream          bo(buffer.data());
        bit_istream          bi(buffer.data());
        bo.write_elias_delta(value);
        auto decoded_value = bi.read_elias_delta();
        EXPECT_EQ(decoded_value, value);
    }
    {
        auto value = 100;
        std::vector<uint8_t> buffer(bytes(value));
        bit_ostream          bo(buffer.data());
        bit_istream          bi(buffer.data());
        bo.write_vbyte(value);
        auto decoded_value = bi.read_vbyte();
        EXPECT_EQ(decoded_value, value);
    }
    {
        auto value = (1ULL << 28) - 1;
        std::vector<uint8_t> buffer(bytes(value));
        bit_ostream          bo(buffer.data());
        bit_istream          bi(buffer.data());
        bo.write_vbyte(value);
        auto decoded_value = bi.read_vbyte();
        EXPECT_EQ(decoded_value, value);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
