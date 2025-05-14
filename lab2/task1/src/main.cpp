#include "../include/bigint.h"

int main() {
    BigInt a("45235237000000856");
    BigInt b(1);
//
    std::cout << a << '\n' << b << '\n'
    << a + b << '\n' << a - b << '\n' << b - a
//    << '\n' << a * b << '\n' << a / b
    << std::endl;

    return 0;
}
