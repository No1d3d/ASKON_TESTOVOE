#include "auth.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Authorization FAILED\n";
        return 1;
    }

    if (verify_public_key(argv[1]))
    {
        std::cout << "Authorization SUCCESS\n";
        return 0;
    }

    std::cout << "Authorization FAILED\n";
    return 1;
}