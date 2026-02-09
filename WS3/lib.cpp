#include <iostream>

extern "C" __declspec(dllexport)
void hello_from_lib() {
    std::cout << "Hello from DLL" << std::endl;
}
