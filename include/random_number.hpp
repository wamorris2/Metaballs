#ifndef _RANDOM_NUMBER_H_
#define _RANDOM_NUMBER_H_

#include <random>

static std::random_device rd;
static std::mt19937 rnd;



static float random_float_uni()
{
	std::uniform_real_distribution<float> dist(0.0, 1.0);
	return dist(rnd);
}

#endif // !_RANDOM_NUMBER_H_
