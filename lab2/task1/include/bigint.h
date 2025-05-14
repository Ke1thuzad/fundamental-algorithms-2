#ifndef FUNDAMENTAL_ALGORITHMS_2_BIGINT_H
#define FUNDAMENTAL_ALGORITHMS_2_BIGINT_H

#include <iostream>
#include <ranges>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

#define DEFAULT_BASE 100000

class BigInt {
private:
    std::vector<unsigned long long> digits {};
    bool isNegative = false;
    unsigned long long _base = DEFAULT_BASE;

    void parse_unsigned_value(unsigned long long value) {
        while (value > 0) {
            digits.push_back(value % _base);
            value /= _base;
        }
    }

    void remove_leading_zeros() {
        while (!digits.empty() && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.empty())
            isNegative = false;
    }

    static std::strong_ordering compare_absolutes(const BigInt& a, const BigInt& b) {
        if (a.digits.size() != b.digits.size()) {
            return a.digits.size() <=> b.digits.size();
        }
        for (auto it_a = a.digits.rbegin(), it_b = b.digits.rbegin();
             it_a != a.digits.rend();
             ++it_a, ++it_b) {
            if (*it_a != *it_b) {
                return *it_a <=> *it_b;
            }
        }
        return std::strong_ordering::equal;
    }

    BigInt subtract_unsigned(const BigInt& a, const BigInt& b) const {
        BigInt result;
        result._base = a._base;
        unsigned long long borrow = 0;
        for (size_t i = 0; i < a.digits.size(); ++i) {
            long long current = a.digits[i] - borrow;
            borrow = 0;
            if (i < b.digits.size()) {
                current -= b.digits[i];
            }
            if (current < 0) {
                current += _base;
                borrow = 1;
            }
            result.digits.push_back(static_cast<unsigned long long>(current));
        }
        result.remove_leading_zeros();
        return result;
    }

public:
    BigInt() = default;
    explicit BigInt(long long value) {
        if (value < 0) {
            value *= -1;
            isNegative = true;
        } else if (value == 0) {
            return;
        }
        parse_unsigned_value(value);
    }

    explicit BigInt(const std::string& str) {
        size_t index = 0;
        if (str[index] == '-') {
            index++;
            isNegative = true;
        }
        size_t temp = 0;
        size_t multiplier = 1;
        int temp_size = 0;
        int block_size = (int) log10((double)_base);
        for (size_t end = str.length() - 1; end > index; end--) {
            if (!isdigit(str[end]))
                throw std::invalid_argument("String contains non-digit characters");
            temp += (str[end] - '0') * multiplier;
            multiplier *= 10;
            temp_size++;
            if (temp_size == block_size) {
                digits.push_back(temp);
                temp = 0;
                temp_size = 0;
                multiplier = 1;
            }
        }
        temp += (str[index] - '0') * multiplier;
        if (temp > 0)
            digits.push_back(temp);
        if (temp == 0 && temp_size == 0)
            isNegative = false;
    }

    BigInt(const BigInt& other) {
        digits = other.digits;
        _base = other._base;
        isNegative = other.isNegative;
    }

    BigInt(BigInt&& other) noexcept {
        digits = std::move(other.digits);
        _base = other._base;
        isNegative = other.isNegative;
    }

    ~BigInt() = default;

    BigInt& operator=(const BigInt& other) {
        if (*this == other)
            return *this;
        digits = other.digits;
        _base = other._base;
        isNegative = other.isNegative;
        return *this;
    }

    BigInt& operator=(BigInt&& other) noexcept {
        if (*this == other)
            return *this;
        digits = std::move(other.digits);
        _base = other._base;
        isNegative = other.isNegative;
        return *this;
    }

    BigInt operator+(const BigInt& other) const {
        if (isNegative == other.isNegative) {
            BigInt result;
            result.isNegative = isNegative;
            result._base = _base;
            size_t carry = 0;
            size_t max_length = std::max(digits.size(), other.digits.size());
            for (size_t i = 0; i < max_length || carry; ++i) {
                size_t current_sum = carry;
                if (i < digits.size())
                    current_sum += digits[i];
                if (i < other.digits.size())
                    current_sum += other.digits[i];
                result.digits.push_back(current_sum % _base);
                carry = current_sum / _base;
            }
            result.remove_leading_zeros();
            return result;
        } else {
            BigInt abs_this = *this;
            BigInt abs_other = other;
            abs_this.isNegative = false;
            abs_other.isNegative = false;
            auto compare = compare_absolutes(abs_this, abs_other);
            if (compare == std::strong_ordering::equal)
                return BigInt(0);
            BigInt diff;
            if (compare == std::strong_ordering::greater) {
                diff = subtract_unsigned(abs_this, abs_other);
                diff.isNegative = isNegative;
            } else if (compare == std::strong_ordering::less) {
                diff = subtract_unsigned(abs_other, abs_this);
                diff.isNegative = other.isNegative;
            }
            return diff;
        }
    }

    BigInt operator-() const {
        BigInt a = *this;
        a.isNegative = !a.isNegative;
        return a;
    }

    BigInt operator-(const BigInt& other) const {
        return *this + (-other);
    }

    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result._base = _base;
        result.digits.resize(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); ++i) {
            unsigned long long carry = 0;
            for (size_t j = 0; j < other.digits.size() || carry; ++j) {
                unsigned long long prod = 0;
                if (j < other.digits.size()) {
                    prod = digits[i] * other.digits[j];
                }
                unsigned long long sum = result.digits[i + j] + prod + carry;
                result.digits[i + j] = sum % _base;
                carry = sum / _base;
            }
        }
        result.isNegative = isNegative != other.isNegative;
        result.remove_leading_zeros();
        return result;
    }

    BigInt operator/(const BigInt& other) const {
        if (other == BigInt(0)) {
            throw std::invalid_argument("Division by zero");
        }
        bool result_isNegative = isNegative != other.isNegative;
        BigInt a = *this;
        a.isNegative = false;
        BigInt b = other;
        b.isNegative = false;
        if (a < b) {
            return BigInt(0);
        }
        BigInt quotient;
        quotient._base = _base;
        BigInt current_remainder;
        BigInt base = BigInt(_base);

        for (unsigned long long & digit : std::ranges::reverse_view(a.digits)) {
            current_remainder = current_remainder * base + BigInt(digit);
            BigInt q_i(0);
            BigInt multiple(0);
            while (multiple + b <= current_remainder) {
                multiple += b;
                q_i += BigInt(1);
            }
            current_remainder -= multiple;
            quotient.digits.push_back(q_i.digits.empty() ? 0 : q_i.digits[0]);
        }
        std::reverse(quotient.digits.begin(), quotient.digits.end());
        quotient.remove_leading_zeros();
        quotient.isNegative = !quotient.digits.empty() && result_isNegative;
        return quotient;
    }

    BigInt& operator+=(const BigInt& other) { return *this = *this + other; }
    BigInt& operator-=(const BigInt& other) { return *this = *this - other; }
    BigInt& operator*=(const BigInt& other) { return *this = *this * other; }
    BigInt& operator/=(const BigInt& other) { return *this = *this / other; }

    BigInt operator++() { return *this += BigInt(1); }
    BigInt operator--() { return *this -= BigInt(1); }

    bool operator==(const BigInt& other) const {
        return (*this <=> other) == std::strong_ordering::equal;
    }

    std::strong_ordering operator<=>(const BigInt& other) const {
        if (isNegative != other.isNegative) {
            return isNegative ? std::strong_ordering::less
                              : std::strong_ordering::greater;
        }
        const auto abs_comparison = compare_absolutes(*this, other);
        if (isNegative)
            return 0 <=> abs_comparison;
        return abs_comparison;
    }

    friend std::istream& operator>>(std::istream& is, BigInt& num) {
        std::string input;
        is >> input;
        num = BigInt(input);
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInt& num) {
        if (num.isNegative && !(num.digits.size() == 1 && num.digits[0] == 0)) {
            os << '-';
        }
        if (num.digits.empty()) {
            os << '0';
        } else {
            os << num.digits.back();
            for (auto it = num.digits.rbegin() + 1; it != num.digits.rend(); ++it) {
                os << std::setw((int)log10(num._base)) << std::setfill('0') << *it;
            }
        }
        return os;
    }
};

BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod);
BigInt fft_multiply(const BigInt& a);
BigInt karatsuba_multiply(const BigInt& a);
BigInt newton_divide(const BigInt& a);

#endif