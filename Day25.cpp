#include <iostream>
#include <fstream>
#include <string>
#include "TemplatedUtilities.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day25.exe Datafilename" << std::endl;
        return -1;
    }
    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;
    size_t cardPublicKey, doorPublicKey;
    in >> cardPublicKey >> doorPublicKey;

    const size_t transform = 20201227;
    std::cout << "Encription Key: " << ModuloExp(doorPublicKey, ModuloLog(7u, cardPublicKey, transform), transform) << std::endl;

    return 0;
}