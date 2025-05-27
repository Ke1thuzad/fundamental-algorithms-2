#ifndef FUNDAMENTAL_ALGORITHMS_2_BIGINT_H
#define FUNDAMENTAL_ALGORITHMS_2_BIGINT_H

#include <iostream>
#include <ranges>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <complex>

#define DEFAULT_BASE 1000000

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

    [[nodiscard]] BigInt subtract_unsigned(const BigInt& a, const BigInt& b) const {
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

    void long_division(const BigInt& other, BigInt &quotient, BigInt &remainder) const {
        if (other == BigInt(0))
            throw std::invalid_argument("Division by zero");

        bool result_isNegative = isNegative != other.isNegative;
        BigInt a = *this;
        a.isNegative = false;
        BigInt b = other;
        b.isNegative = false;

        quotient._base = _base;
        BigInt base = BigInt(_base);

        for (unsigned long long & digit : std::ranges::reverse_view(a.digits)) {
            remainder = remainder * base + BigInt(digit);
            BigInt q_i(0);
            BigInt multiple(0);
            while (multiple + b <= remainder) {
                multiple += b;
                q_i += BigInt(1);
            }
            remainder -= multiple;
            quotient.digits.push_back(q_i.digits.empty() ? 0 : q_i.digits[0]);
        }
        std::reverse(quotient.digits.begin(), quotient.digits.end());
        quotient.remove_leading_zeros();
        quotient.isNegative = !quotient.digits.empty() && result_isNegative;
    }

    void fft(std::vector<std::complex<long double>>& a) const {
        size_t n = a.size();
        for (size_t i = 1, j = 0; i < n; ++i) {
            size_t bit = n >> 1;
            while (j >= bit) {
                j -= bit;
                bit >>= 1;
            }
            j += bit;
            if (i < j)
                std::swap(a[i], a[j]);
        }

        for (size_t len = 2; len <= n; len <<= 1) {
            long double ang = 2 * M_PI / len;
            std::complex<long double> wlen(std::cos(ang), std::sin(ang));

            for (size_t i = 0; i < n; i += len) {
                std::complex<long double> w(1);
                for (size_t j = 0; j < len / 2; ++j) {
                    std::complex<long double> u = a[i + j];
                    std::complex<long double> v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }
    }

//    BigInt karatsuba_multiply(const BigInt& rhs) const {
//        BigInt result;
//
//        BigInt temp_lhs = *this;
//        BigInt temp_rhs = rhs;
//
//        if (temp_lhs.digits.empty() || temp_rhs.digits.empty())
//            return BigInt(0);
//
//        if (temp_lhs.digits.size() == 1 && temp_rhs.digits.size() == 1) {
//            result = temp_lhs.digits[0] * temp_rhs.digits[0];
//        } else {
//            unsigned long max_len = std::max(temp_lhs.digits.size(), temp_rhs.digits.size());
//            if (max_len % 2 != 0)
//                max_len++;
//
//            temp_lhs.digits.resize(max_len, 0);
//            temp_rhs.digits.resize(max_len, 0);
//
//            unsigned long mid = max_len / 2;
//
//            BigInt lhs_right_copy(*this, (long) mid, (long) max_len); // A
//            BigInt lhs_left_copy(*this, 0, (long) mid); // B
//
//            BigInt rhs_right_copy(temp_rhs, (long) mid, (long) max_len); // C
//            BigInt rhs_left_copy(temp_rhs, 0, (long) mid); // D
//
//            BigInt ac = lhs_right_copy.karatsuba_multiply(rhs_right_copy);  // AC
//            BigInt bd = lhs_left_copy.karatsuba_multiply(rhs_left_copy);  // BD
//
//            BigInt cd = rhs_left_copy + rhs_right_copy;
//            BigInt ab = (lhs_right_copy + lhs_left_copy);
//            BigInt ab_cd = ab.karatsuba_multiply(cd);
//
//            BigInt ad_bc = ab_cd - ac - bd;
//
//            result += ac;
//
//            for (unsigned long i = 0; i < 2 * mid; ++i) {
//                result.digits.insert(result.digits.begin(), 0);
//            }
//
//            BigInt shifted_ad_bc = ad_bc;
//            for (unsigned long i = 0; i < mid; ++i) {
//                shifted_ad_bc.digits.insert(shifted_ad_bc.digits.begin(), 0);
//            }
//            result += shifted_ad_bc;
//
//            result += bd;
//        }
//
//        result.remove_leading_zeros();
//        result.isNegative = temp_lhs.isNegative != temp_rhs.isNegative;
//
//        return result;
//    }

public:
    BigInt fft_multiply(const BigInt& other) const {
        if (digits.empty() || other.digits.empty())
            return BigInt(0);

        size_t n = digits.size();
        size_t m = other.digits.size();
        size_t total_size = n + m;

        size_t fft_size = 1;
        while (fft_size < total_size)
            fft_size <<= 1;

        std::vector<std::complex<long double>> fa(fft_size, std::complex<long double>(0));
        std::vector<std::complex<long double>> fb(fft_size, std::complex<long double>(0));

        for (size_t i = 0; i < n; ++i)
            fa[i] = static_cast<long double>(digits[i]);
        for (size_t i = 0; i < m; ++i)
            fb[i] = static_cast<long double>(other.digits[i]);

        fft(fa);
        fft(fb);

        for (size_t i = 0; i < fft_size; ++i)
            fa[i] *= fb[i];

        for (auto& x : fa)
            x = std::conj(x);
        fft(fa);
        for (auto& x : fa)
            x = std::conj(x);
        for (auto& x : fa)
            x /= static_cast<long double>(fft_size);

        std::vector<long long> product_coefficients(fft_size, 0);
        for (size_t i = 0; i < fft_size; ++i)
            product_coefficients[i] = std::llround(fa[i].real());

        BigInt result;
        result._base = _base;
        long long carry = 0;
        for (size_t i = 0; i < fft_size || carry > 0; ++i) {
            long long value = carry;
            if (i < fft_size)
                value += product_coefficients[i];
            result.digits.push_back(value % static_cast<long long>(result._base));
            carry = value / static_cast<long long>(result._base);
        }

        result.remove_leading_zeros();
        result.isNegative = (isNegative != other.isNegative);
        return result;
    }

    BigInt karatsuba_multiply(const BigInt& rhs) const {
        BigInt result;

        BigInt temp_lhs = *this;
        BigInt temp_rhs = rhs;

        if (temp_lhs.digits.empty() || temp_rhs.digits.empty())
            return BigInt(0);

        if (temp_lhs.digits.size() == 1 && temp_rhs.digits.size() == 1) {
            result = temp_lhs.digits[0] * temp_rhs.digits[0];
        } else {
            unsigned long max_len = std::max(temp_lhs.digits.size(), temp_rhs.digits.size());
            if (max_len % 2 != 0)
                max_len++;

            temp_lhs.digits.resize(max_len, 0);
            temp_rhs.digits.resize(max_len, 0);

            unsigned long mid = max_len / 2;

            BigInt lhs_right_copy(temp_lhs, (long) mid, (long) max_len); // A
            BigInt lhs_left_copy(temp_lhs, 0, (long) mid); // B

            BigInt rhs_right_copy(temp_rhs, (long) mid, (long) max_len); // C
            BigInt rhs_left_copy(temp_rhs, 0, (long) mid); // D

            BigInt ac = lhs_right_copy.karatsuba_multiply(rhs_right_copy);  // AC
            BigInt bd = lhs_left_copy.karatsuba_multiply(rhs_left_copy);  // BD

            BigInt cd = rhs_left_copy + rhs_right_copy;
            BigInt ab = (lhs_right_copy + lhs_left_copy);
            BigInt ab_cd = ab.karatsuba_multiply(cd);

            BigInt ad_bc = ab_cd - ac - bd;

            result += ac;

            for (unsigned long i = 0; i < 2 * mid; ++i) {
                result.digits.insert(result.digits.begin(), 0);
            }

            BigInt shifted_ad_bc = ad_bc;
            for (unsigned long i = 0; i < mid; ++i) {
                shifted_ad_bc.digits.insert(shifted_ad_bc.digits.begin(), 0);
            }
            result += shifted_ad_bc;

            result += bd;
        }

        result.remove_leading_zeros();
        result.isNegative = temp_lhs.isNegative != temp_rhs.isNegative;

        return result;
    }

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

    BigInt(const BigInt& other, long start, long stop) {
        digits.assign(other.digits.begin() + start, other.digits.begin() + stop);
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

    BigInt& operator=(long long other) noexcept {
        *this = BigInt(other);
        return *this;
    }

    BigInt& operator=(unsigned long long other) noexcept {
        *this = BigInt(other);
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
//        return this->fft_multiply(other);
//    }
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
        BigInt quotient, remainder;

        long_division(other, quotient, remainder);

        return quotient;
    }

    BigInt operator%(const BigInt& other) const {
        BigInt mod = other;
        mod.isNegative = false;

        BigInt quotient, remainder;

        long_division(mod, quotient, remainder);

        if (isNegative && remainder != BigInt(0)) {
            remainder = mod - remainder;
        }

        return remainder;
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

BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod) {
    BigInt result(1);
    BigInt a = base;
    BigInt b = exp;

    a = a % mod;

    BigInt zero(0);

    while (b > zero) {
        if (b % BigInt(2) == BigInt(1)) {
            result = (result * a) % mod;
        }
        a = (a * a) % mod;

        b /= BigInt(2);
    }

    return result;
}

BigInt fft_multiply(const BigInt& a);

BigInt newton_divide(const BigInt& a);

#endif