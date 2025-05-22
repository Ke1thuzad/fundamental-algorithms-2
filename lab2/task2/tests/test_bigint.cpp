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
};

TEST_F(BigIntTest, Constructor_LongLong) {
    EXPECT_EQ(BigInt(0), zero);
    EXPECT_EQ(BigInt(-0), zero);
    EXPECT_EQ(BigInt(123), BigInt("123"));
    EXPECT_EQ(BigInt(-456), BigInt("-456"));
    EXPECT_EQ(BigInt(45235237856), BigInt("45235237856"));
}

TEST_F(BigIntTest, Constructor_String) {
    EXPECT_EQ(BigInt("0"), zero);
    EXPECT_EQ(BigInt("-0"), zero);
    EXPECT_EQ(BigInt("123"), BigInt(123));
    EXPECT_EQ(BigInt("-456"), BigInt(-456));
    EXPECT_EQ(BigInt("1234567890123456789"), large1);
    EXPECT_EQ(BigInt("-9876543210987654321"), -large2);
    EXPECT_EQ(BigInt("1000000000"), large6);
    EXPECT_EQ(BigInt("1000000001"), BigInt(1000000001LL));
    EXPECT_EQ(BigInt("999999999"), BigInt(999999999LL));
}

TEST_F(BigIntTest, Addition_Zero) {
    EXPECT_EQ(zero + zero, zero);
    EXPECT_EQ(one + zero, one);
    EXPECT_EQ(zero + one, one);
    EXPECT_EQ(minusOne + zero, minusOne);
    EXPECT_EQ(zero + minusOne, minusOne);
    EXPECT_EQ(ten + zero, ten);
    EXPECT_EQ(large1 + zero, large1);
}

TEST_F(BigIntTest, Addition_Positives) {
    EXPECT_EQ(five + BigInt(7), BigInt(12));
    EXPECT_EQ(ten + five, BigInt(15));
    EXPECT_EQ(BigInt(99) + BigInt(1), hundred);
    EXPECT_EQ(BigInt(999) + BigInt(1), thousand);
    EXPECT_EQ(BigInt(999999999) + BigInt(1), large6);
    EXPECT_EQ(large4 + large4, large6);
    EXPECT_EQ(large6 + large6, large5);
    EXPECT_EQ(large6 + BigInt("1"), BigInt("1000000001"));
    EXPECT_EQ(large1 + large2, BigInt("11111111101111111110"));
}

TEST_F(BigIntTest, Addition_Negatives) {
    EXPECT_EQ(minusFive + BigInt(-7), BigInt(-12));
    EXPECT_EQ(minusTen + minusFive, BigInt(-15));
    EXPECT_EQ(BigInt(-99) + BigInt(-1), BigInt(-100));
    EXPECT_EQ(-large1 + -large2, BigInt("-11111111101111111110"));
    EXPECT_EQ(BigInt("-999999999") + BigInt("-1"), -large6);
}

TEST_F(BigIntTest, Addition_MixedSigns) {
    EXPECT_EQ(ten + minusFive, five);
    EXPECT_EQ(five + minusTen, minusFive);
    EXPECT_EQ(minusTen + five, minusFive);
    EXPECT_EQ(minusFive + ten, five);
    EXPECT_EQ(ten + minusTen, zero);
    EXPECT_EQ(minusTen + ten, zero);
    EXPECT_EQ(large1 + (-large1), zero);
    EXPECT_EQ(large1 + (-large2), large1 - large2);
    EXPECT_EQ(large2 + (-large1), large2 - large1);
}

TEST_F(BigIntTest, UnaryMinus) {
    EXPECT_EQ(-one, minusOne);
    EXPECT_EQ(-minusOne, one);
    EXPECT_EQ(-ten, minusTen);
    EXPECT_EQ(-minusTen, ten);
    EXPECT_EQ(-large1, BigInt("-1234567890123456789"));
    EXPECT_EQ(-(-large1), large1);
}

TEST_F(BigIntTest, Subtraction_Zero) {
    EXPECT_EQ(zero - zero, zero);
    EXPECT_EQ(one - zero, one);
    EXPECT_EQ(zero - one, minusOne);
    EXPECT_EQ(minusOne - zero, minusOne);
    EXPECT_EQ(zero - minusOne, one);
    EXPECT_EQ(large1 - zero, large1);
}

TEST_F(BigIntTest, Subtraction_Positives) {
    EXPECT_EQ(ten - five, five);
    EXPECT_EQ(five - ten, minusFive);
    EXPECT_EQ(hundred - one, BigInt(99));
    EXPECT_EQ(thousand - one, BigInt(999));
    EXPECT_EQ(large6 - one, BigInt("999999999"));
    EXPECT_EQ(large2 - large1, BigInt("8641975320864197532"));
    EXPECT_EQ(large1 - large2, BigInt("-8641975320864197532"));
    EXPECT_EQ(large5 - large6, large6);
}

TEST_F(BigIntTest, Subtraction_Negatives) {
    EXPECT_EQ(minusTen - minusFive, minusFive);
    EXPECT_EQ(minusFive - minusTen, five);
    EXPECT_EQ(BigInt(-100) - BigInt(-1), BigInt(-99));
    EXPECT_EQ(-large1 - (-large2), -large1 + large2);
    EXPECT_EQ(-large2 - (-large1), -large2 + large1);
}

TEST_F(BigIntTest, Subtraction_MixedSigns) {
    EXPECT_EQ(ten - minusFive, BigInt(15));
    EXPECT_EQ(minusTen - five, BigInt(-15));
    EXPECT_EQ(ten - minusTen, BigInt(20));
    EXPECT_EQ(minusTen - ten, BigInt(-20));
}

TEST_F(BigIntTest, Multiplication_Zero) {
    EXPECT_EQ(zero * zero, zero);
    EXPECT_EQ(one * zero, zero);
    EXPECT_EQ(zero * one, zero);
    EXPECT_EQ(ten * zero, zero);
    EXPECT_EQ(zero * ten, zero);
    EXPECT_EQ(large1 * zero, zero);
    EXPECT_EQ(zero * large1, zero);
    EXPECT_EQ(minusOne * zero, zero);
    EXPECT_EQ(zero * minusOne, zero);
}

TEST_F(BigIntTest, Multiplication_One) {
    EXPECT_EQ(one * one, one);
    EXPECT_EQ(ten * one, ten);
    EXPECT_EQ(one * ten, ten);
    EXPECT_EQ(minusOne * one, minusOne);
    EXPECT_EQ(one * minusOne, minusOne);
    EXPECT_EQ(large1 * one, large1);
    EXPECT_EQ(one * large1, large1);
}

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

TEST_F(BigIntTest, Division_ByZero) {
    EXPECT_THROW(one / zero, std::invalid_argument);
    EXPECT_THROW(ten / zero, std::invalid_argument);
    EXPECT_THROW(large1 / zero, std::invalid_argument);
    EXPECT_THROW(minusOne / zero, std::invalid_argument);
    EXPECT_THROW(zero / zero, std::invalid_argument);
}

TEST_F(BigIntTest, Division_ZeroDividend) {
    EXPECT_EQ(zero / one, zero);
    EXPECT_EQ(zero / ten, zero);
    EXPECT_EQ(zero / large1, zero);
    EXPECT_EQ(zero / minusOne, zero);
    EXPECT_EQ(zero / minusTen, zero);
}

TEST_F(BigIntTest, Division_ByOne) {
    EXPECT_EQ(one / one, one);
    EXPECT_EQ(ten / one, ten);
    EXPECT_EQ(large1 / one, large1);
    EXPECT_EQ(minusOne / one, minusOne);
    EXPECT_EQ(minusTen / one, minusTen);
    EXPECT_EQ(-large1 / one, -large1);
}

TEST_F(BigIntTest, Division_ByMinusOne) {
    EXPECT_EQ(one / minusOne, minusOne);
    EXPECT_EQ(ten / minusOne, minusTen);
    EXPECT_EQ(large1 / minusOne, -large1);
    EXPECT_EQ(minusOne / minusOne, one);
    EXPECT_EQ(minusTen / minusOne, ten);
    EXPECT_EQ(-large1 / minusOne, large1);
}

TEST_F(BigIntTest, Division_Positives) {
    EXPECT_EQ(ten / BigInt(2), five);
    EXPECT_EQ(ten / BigInt(3), BigInt(3));
    EXPECT_EQ(hundred / ten, ten);
    EXPECT_EQ(hundred / BigInt(3), BigInt(33));
    EXPECT_EQ(five / ten, zero);
    EXPECT_EQ(BigInt(12345) / BigInt(123), hundred);
    EXPECT_EQ(large6 / ten, BigInt("100000000"));
    EXPECT_EQ(large7 / large6, ten);
    EXPECT_EQ(large5 / BigInt(2), large6);
    EXPECT_EQ(large5 / large6, BigInt(2));
    EXPECT_EQ(large1 / large1, one);
    EXPECT_EQ(large2 / large1, BigInt(8));
    EXPECT_EQ(large1 / large2, zero);
    EXPECT_EQ(large3 / large6, large6);
}

TEST_F(BigIntTest, Division_Negatives) {
    EXPECT_EQ(minusTen / BigInt(-2), five);
    EXPECT_EQ(minusTen / BigInt(-3), BigInt(3));
    EXPECT_EQ(BigInt(-100) / BigInt(-10), ten);
    EXPECT_EQ(minusFive / minusTen, zero);
    EXPECT_EQ(-large1 / -large1, one);
    EXPECT_EQ(-large2 / -large1, BigInt(8));
    EXPECT_EQ(-large3 / -large6, large6);
}

TEST_F(BigIntTest, Division_MixedSigns) {
    EXPECT_EQ(ten / BigInt(-2), minusFive);
    EXPECT_EQ(ten / BigInt(-3), BigInt(-3));
    EXPECT_EQ(minusTen / BigInt(2), minusFive);
    EXPECT_EQ(minusTen / BigInt(3), BigInt(-3));
    EXPECT_EQ(five / minusTen, zero);
    EXPECT_EQ(minusFive / ten, zero);
    EXPECT_EQ(large1 / minusOne, -large1);
    EXPECT_EQ(-large1 / one, -large1);
    EXPECT_EQ(large7 / -large6, minusTen);
    EXPECT_EQ(-large7 / large6, minusTen);
    EXPECT_EQ(large2 / -large1, BigInt(-8));
    EXPECT_EQ(-large2 / large1, BigInt(-8));
}

TEST_F(BigIntTest, CompoundAssignment_Add) {
    BigInt a = ten; a += five; EXPECT_EQ(a, BigInt(15));
    BigInt b = ten; b += minusTen; EXPECT_EQ(b, zero);
    BigInt c = zero; c += large1; EXPECT_EQ(c, large1);
    BigInt d = large1; d += large2; EXPECT_EQ(d, large1 + large2);
    BigInt e = large6; e += one; EXPECT_EQ(e, BigInt("1000000001"));
}

TEST_F(BigIntTest, CompoundAssignment_Subtract) {
    BigInt a = ten; a -= five; EXPECT_EQ(a, five);
    BigInt b = ten; b -= ten; EXPECT_EQ(b, zero);
    BigInt c = zero; c -= large1; EXPECT_EQ(c, -large1);
    BigInt d = large2; d -= large1; EXPECT_EQ(d, large2 - large1);
    BigInt e = large6; e -= one; EXPECT_EQ(e, BigInt("999999999"));
}

TEST_F(BigIntTest, CompoundAssignment_Multiply) {
    BigInt a = ten; a *= five; EXPECT_EQ(a, BigInt(50));
    BigInt b = ten; b *= zero; EXPECT_EQ(b, zero);
    BigInt c = one; c *= large1; EXPECT_EQ(c, large1);
    BigInt d = large6; d *= ten; EXPECT_EQ(d, large7);
    BigInt e = large6; e *= large6; EXPECT_EQ(e, large3);
}

TEST_F(BigIntTest, CompoundAssignment_Divide) {
    BigInt a = ten; a /= BigInt(2); EXPECT_EQ(a, five);
    BigInt b = ten; b /= BigInt(3); EXPECT_EQ(b, BigInt(3));
    BigInt c = large3; c /= large6; EXPECT_EQ(c, large6);
    BigInt d = large1; d /= large2; EXPECT_EQ(d, zero);
    BigInt e = ten; EXPECT_THROW(e /= zero, std::invalid_argument);
    BigInt f = large7; f /= large6; EXPECT_EQ(f, ten);
}

TEST_F(BigIntTest, Increment) {
    BigInt a = zero; ++a; EXPECT_EQ(a, one);
    BigInt b = one; ++b; EXPECT_EQ(b, BigInt(2));
    BigInt c = minusOne; ++c; EXPECT_EQ(c, zero);
    BigInt d = BigInt(-10); ++d; EXPECT_EQ(d, BigInt(-9));
    BigInt e = BigInt("999999999"); ++e; EXPECT_EQ(e, large6);
}

TEST_F(BigIntTest, Decrement) {
    BigInt a = zero; --a; EXPECT_EQ(a, minusOne);
    BigInt b = one; --b; EXPECT_EQ(b, zero);
    BigInt c = BigInt(2); --c; EXPECT_EQ(c, one);
    BigInt d = minusOne; --d; EXPECT_EQ(d, BigInt(-2));
    BigInt e = large6; --e; EXPECT_EQ(e, BigInt("999999999"));
}

TEST_F(BigIntTest, Comparison_Equality) {
    EXPECT_EQ(zero, BigInt(0));
    EXPECT_EQ(one, BigInt(1));
    EXPECT_EQ(minusOne, BigInt(-1));
    EXPECT_EQ(BigInt("12345"), BigInt("12345"));
    EXPECT_EQ(BigInt("-12345"), BigInt("-12345"));
    EXPECT_EQ(BigInt("0"), zero);
    EXPECT_EQ(BigInt("-0"), zero);
    EXPECT_EQ(large1, BigInt("1234567890123456789"));
    EXPECT_EQ(large6, BigInt("1000000000"));

    EXPECT_NE(zero, one);
    EXPECT_NE(one, minusOne);
    EXPECT_NE(ten, BigInt(11));
    EXPECT_NE(large1, large2);
    EXPECT_NE(large1, -large1);
    EXPECT_NE(large6, large5);
}

TEST_F(BigIntTest, Comparison_Inequality) {
    EXPECT_LT(zero, one);
    EXPECT_LT(minusOne, zero);
    EXPECT_LT(minusOne, one);
    EXPECT_LT(five, ten);
    EXPECT_LT(minusTen, minusFive);
    EXPECT_LT(minusTen, five);
    EXPECT_LT(large1, large2);
    EXPECT_LT(-large2, -large1);

    EXPECT_LE(zero, zero);
    EXPECT_LE(zero, one);
    EXPECT_LE(minusOne, zero);
    EXPECT_LE(minusOne, one);
    EXPECT_LE(five, five);
    EXPECT_LE(five, ten);
    EXPECT_LE(-large1, large1);
    EXPECT_LE(-large1, zero);

    EXPECT_GT(one, zero);
    EXPECT_GT(zero, minusOne);
    EXPECT_GT(one, minusOne);
    EXPECT_GT(ten, five);
    EXPECT_GT(minusFive, minusTen);
    EXPECT_GT(five, minusTen);
    EXPECT_GT(large2, large1);
    EXPECT_GT(-large1, -large2);

    EXPECT_GE(zero, zero);
    EXPECT_GE(one, zero);
    EXPECT_GE(zero, minusOne);
    EXPECT_GE(one, minusOne);
    EXPECT_GE(ten, ten);
    EXPECT_GE(ten, five);
    EXPECT_GE(large1, -large1);
    EXPECT_GE(zero, minusOne);
}

TEST_F(BigIntTest, IO_Stream_Output) {
    std::stringstream ss;
    ss << zero; EXPECT_EQ(ss.str(), "0"); ss.str(""); ss.clear();
    ss << one; EXPECT_EQ(ss.str(), "1"); ss.str(""); ss.clear();
    ss << minusOne; EXPECT_EQ(ss.str(), "-1"); ss.str(""); ss.clear();
    ss << ten; EXPECT_EQ(ss.str(), "10"); ss.str(""); ss.clear();
    ss << minusTen; EXPECT_EQ(ss.str(), "-10"); ss.str(""); ss.clear();
    ss << BigInt("12345"); EXPECT_EQ(ss.str(), "12345"); ss.str(""); ss.clear();
    ss << BigInt("-12345"); EXPECT_EQ(ss.str(), "-12345"); ss.str(""); ss.clear();
    ss << large6; EXPECT_EQ(ss.str(), "1000000000"); ss.str(""); ss.clear();
    ss << BigInt("1000000001"); EXPECT_EQ(ss.str(), "1000000001"); ss.str(""); ss.clear();
    ss << BigInt("999999999"); EXPECT_EQ(ss.str(), "999999999"); ss.str(""); ss.clear();
    ss << large1; EXPECT_EQ(ss.str(), "1234567890123456789"); ss.str(""); ss.clear();
    ss << large3; EXPECT_EQ(ss.str(), "1000000000000000000"); ss.str(""); ss.clear();
    ss << large7; EXPECT_EQ(ss.str(), "10000000000"); ss.str(""); ss.clear();
}

TEST_F(BigIntTest, IO_Stream_Input) {
    std::stringstream ss;
    BigInt num;

    ss << "123"; ss >> num; EXPECT_EQ(num, BigInt(123));
    ss.clear(); ss.str(""); num = BigInt(0);

    ss << "-456"; ss >> num; EXPECT_EQ(num, BigInt(-456));
    ss.clear(); ss.str(""); num = BigInt(0);

    ss << "0"; ss >> num; EXPECT_EQ(num, zero);
    ss.clear(); ss.str(""); num = BigInt(0);

    ss << "-0"; ss >> num; EXPECT_EQ(num, zero);
    ss.clear(); ss.str(""); num = BigInt(0);

    ss << large1; ss >> num; EXPECT_EQ(num, large1);
    ss.clear(); ss.str(""); num = BigInt(0);

    ss << "-9876543210987654321"; ss >> num; EXPECT_EQ(num, -large2);
    ss.clear(); ss.str(""); num = BigInt(0);
}

class ModExpTest : public ::testing::Test {
protected:
    BigInt zero = BigInt(0);
    BigInt one = BigInt(1);
    BigInt two = BigInt(2);
    BigInt three = BigInt(3);
    BigInt five = BigInt(5);
    BigInt mod5 = BigInt(5);
    BigInt mod1 = BigInt(1);
};

TEST_F(ModExpTest, ZeroExponent) {
    BigInt base(123);
    BigInt exp(0);
    BigInt mod(456);
    BigInt result = mod_exp(base, exp, mod);
    EXPECT_EQ(result, BigInt(1));
}

TEST_F(ModExpTest, ModOne) {
    BigInt base(123);
    BigInt exp(456);
    BigInt result = mod_exp(base, exp, mod1);
    EXPECT_EQ(result, zero);
}

TEST_F(ModExpTest, ZeroBase) {
    BigInt base(0);
    BigInt exp(5);
    BigInt mod(10);
    BigInt result = mod_exp(base, exp, mod);
    EXPECT_EQ(result, zero);
}

TEST_F(ModExpTest, StandardTest) {
    BigInt base(12345);
    BigInt exp(12);
    BigInt mod(123);
    BigInt result = mod_exp(base, exp, mod);
    BigInt expected(57);
    EXPECT_EQ(result, expected);
}

TEST_F(ModExpTest, StandardTest2) {
    BigInt base(98765);
    BigInt exp(10);
    BigInt mod(13345);
    BigInt result = mod_exp(base, exp, mod);
    BigInt expected(4565);
    EXPECT_EQ(result, expected);
}

TEST_F(BigIntTest, ModuloOperation) {
    EXPECT_EQ(BigInt(7) % BigInt(3), BigInt(1));
    EXPECT_EQ(BigInt(-7) % BigInt(3), BigInt(2));
    EXPECT_EQ(BigInt(7) % BigInt(-3), BigInt(1));
    EXPECT_EQ(BigInt(-7) % BigInt(-3), BigInt(2));
    EXPECT_EQ(BigInt(5) % BigInt(5), zero);
    EXPECT_EQ(BigInt(-5) % BigInt(5), zero);
    EXPECT_EQ(BigInt(4) % BigInt(5), BigInt(4));
    EXPECT_EQ(BigInt(-4) % BigInt(5), BigInt(1));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}