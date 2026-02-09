#include <iostream>

extern "C" void hello_from_lib();

int main() {
    hello_from_lib();
    return 0;
}
