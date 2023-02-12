
# include <iostream>
# include <unordered_map>
# include <unordered_set>
# include <vector>
#include <cstdlib>
# include <ctime>
# include <cstdlib>

# include "bitmask.h"

# include "generateData.h"
# include "timer.h"

#include <random>

const int theta = 32 * 4;

template <typename A>
void printBitMask(const ripples::Bitmask<A> b) 
{
    for (int i = 0; i < b.bytes() / sizeof(A) ; i++) 
    {    
        std::cout << i << ") " << b.data()[i] << std::endl;
    }
}

template <typename A>
void randomizeBitMask(ripples::Bitmask<A>& b) 
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,1);
    std::cout << dist6(rng) << std::endl;

    for (int i = 0; i < b.size(); i++) {
        if (dist6(rng) == 1) {
            b.set(i);
        }
    }
}

int main () {
    std::cout << "test" << std::endl;
    ripples::Bitmask<int> b1(theta);
    ripples::Bitmask<int> b2(theta);

    std::cout << b1.size() << " " << theta  << std::endl;
    std::cout << b1.bytes() << " " <<  sizeof(int) << std::endl;

    randomizeBitMask(b1);
    randomizeBitMask(b2);

    printBitMask<int>(b1);
    printBitMask<int>(b2);

    b1.notOperation();

    printBitMask<int>(b1);

    b1.andOperation(b2);

    printBitMask<int>(b1);

    b1.orOperation(b2);

    printBitMask<int>(b1);

}