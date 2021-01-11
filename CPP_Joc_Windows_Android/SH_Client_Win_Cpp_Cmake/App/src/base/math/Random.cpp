#include "Random.h"
#include <base/algorithms/randomNumbersGenerator/RNG_xoroshiro128Plus.h>
#include <base/MM.h>

Random::Random(unsigned int seed) {
	generator = new RNG_xoroshiro128Plus::rng128(seed);
}

float Random::nextFloat() {
	// Using nextInt is not perfect but works. The max range of the rng algo is unknown.
	float v = static_cast<float>(nextInt(RAND_MAX)) / static_cast<float>(RAND_MAX);

	return v;
}

float Random::nextFloat(float min, float max) {
	// Using nextInt is not perfect but works. The max range of the rng algo is unknown.
	float v = static_cast<float>(nextInt(RAND_MAX)) / static_cast<float>(RAND_MAX);

	v = min + (max - min) * v;

	return v;
}

int Random::nextInt() {
	int v = generator->next();

	return v;
}

int Random::nextInt(int range) {
	int v = generator->next() % (range + 1);

	return v;
}

int Random::nextInt(int start, int end) {
	int v = start + nextInt(end - start);

	return v;
}

Random::~Random() {
	if (generator != nullptr) {
		delete generator;
		generator = nullptr;
	}
}
