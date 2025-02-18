#include "random.h"

namespace ELSA
{

u64 xorshift64()
{
    static u64 seed = 35907035218217ull;

    seed ^= seed >> 12;
    seed ^= seed << 25;
    seed ^= seed >> 27;

    return seed * 2685821657736338717ull;
}

}; // namespace

