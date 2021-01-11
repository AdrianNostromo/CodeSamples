#pragma once

#include <base/gh.h>

namespace RNG_xoroshiro128Plus {
    struct rng128;
}

class Random {pub dCtor(Random);
    priv RNG_xoroshiro128Plus::rng128* generator;

    pub explicit Random(unsigned int seed);

    // [0.0f, ?]// The upper bounds is probably 1.0f but not tested.
    pub float nextFloat();
    //[min, max];
    pub float nextFloat(float min, float max);
    pub int nextInt();
    /** Returns a random number between 0 (inclusive) and the specified value (inclusive). */
    pub int nextInt(int range);
    /** Returns a random number between start (inclusive) and end (inclusive). */
    pub int nextInt(int start, int end);

    pub virtual ~Random();
};
