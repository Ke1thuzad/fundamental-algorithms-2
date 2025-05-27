#include "../include/bigint.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <limits>

class BigIntTest : public ::testing::Test {
protected:
    BigInt zero = BigInt(0);
    BigInt one = BigInt(1);
    BigInt minusOne = BigInt(-1);
    BigInt five = BigInt(5);
    BigInt minusFive = BigInt(-5);
    BigInt ten = BigInt(10);
    BigInt minusTen = BigInt(-10);
    BigInt hundred = BigInt(100);
    BigInt thousand = BigInt(1000);
    BigInt large1 = BigInt("1234567890123456789");
    BigInt large2 = BigInt("9876543210987654321");
    BigInt large3 = BigInt("1000000000000000000");
    BigInt large4 = BigInt("500000000");
    BigInt large5 = BigInt("2000000000");
    BigInt large6 = BigInt("1000000000");
    BigInt large7 = BigInt("10000000000");
    BigInt large8 = BigInt("2363683468346834683851923915823586238528368238562914012402395236582385194194");
    BigInt large9 = BigInt("23577263587623576287356239502350827386183184348324769104");
    BigInt large10 = BigInt("99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
};

TEST_F(BigIntTest, Multiplication_Positives) {
    EXPECT_EQ(BigInt(2) * BigInt(3), BigInt(6));
    EXPECT_EQ(ten * ten, hundred);
    EXPECT_EQ(hundred * hundred, BigInt(10000));
    EXPECT_EQ(BigInt(123) * BigInt(456), BigInt(56088));
    EXPECT_EQ(large4 * BigInt(2), large6);
    EXPECT_EQ(large6 * ten, large7);
    EXPECT_EQ(large6 * large6, large3);
    EXPECT_EQ(large3 * ten, BigInt("10000000000000000000"));
    EXPECT_EQ(BigInt("123") * large6, BigInt("123000000000"));
    EXPECT_EQ(large6 * BigInt("123"), BigInt("123000000000"));
    EXPECT_EQ(large8 * large9, BigInt("55729188170921629439967661263279848554661225778062474035105338322773491062584942105117097753981026139200804165406010537213251382176"));
    EXPECT_EQ(large10 * large10, BigInt("9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"));
}

TEST_F(BigIntTest, Multiplication_FTT_Positives) {
    EXPECT_EQ(large3.fft_multiply(ten), large3 * ten);
    EXPECT_EQ(large8.fft_multiply(large9), large8 * large9);
    EXPECT_EQ(large10.fft_multiply(large10), large10 * large10);
}

TEST_F(BigIntTest, Multiplication_Negatives) {
    EXPECT_EQ(BigInt(-2) * BigInt(-3), BigInt(6));
    EXPECT_EQ(minusTen * minusTen, hundred);
    EXPECT_EQ(minusTen * minusOne, ten);
    EXPECT_EQ(-large1 * -large1, large1 * large1);
}

TEST_F(BigIntTest, Multiplication_MixedSigns) {
    EXPECT_EQ(BigInt(-2) * BigInt(3), BigInt(-6));
    EXPECT_EQ(BigInt(2) * BigInt(-3), BigInt(-6));
    EXPECT_EQ(minusTen * ten, BigInt(-100));
    EXPECT_EQ(ten * minusTen, BigInt(-100));
    EXPECT_EQ(minusTen * one, minusTen);
    EXPECT_EQ(ten * minusOne, minusTen);
    EXPECT_EQ(large1 * minusOne, -large1);
    EXPECT_EQ(minusOne * large1, -large1);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}