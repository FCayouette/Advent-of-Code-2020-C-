#include <iostream>
#include <fstream>
#include <string>

#include "TemplatedUtilities.h"

using u32 = unsigned int;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: Day25.exe Datafilename" << std::endl;
        return -1;
    }
    std::ifstream in(argv[1], std::ios::in);
    if (!in) return -1;
    u32 cardPublic, doorPublic;
    in >> cardPublic >> doorPublic;

    const u32 transform = 20201227u, subjectNumber = 7;
    size_t doorLoopSize = 0, cardLoopSize = 0, loops = 0;
    u32 value = 1;
    while (!doorLoopSize && !cardLoopSize)
    {
        ++loops;
        value = ModuloMul(value, subjectNumber, transform);
        if (doorPublic == value)
            doorLoopSize = loops;
        if (cardPublic == value)
            cardLoopSize = loops;
    }
   
    u32 publicKey = doorLoopSize ? cardPublic : doorPublic;
    value = 1;
    for (int i = 0; i < loops; ++i)
        value = ModuloMul(value, publicKey, transform);
    
    std::cout << "Encryption Key: " << value << std::endl;

    return 0;
}