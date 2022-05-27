#include "fmt/format.h"
#include <iostream>

int main() {
    std::cout << fmt::format("the answer is {}\n", 42);
}