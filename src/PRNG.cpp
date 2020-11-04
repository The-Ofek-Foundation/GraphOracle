#include "PRNG.h"

#include <openssl/rand.h>

#include <cmath>
#include <limits>

#include <stdio.h>

using namespace ogila;

PRNG::PRNG() noexcept : buffer{}, index(sizeof(buffer))
{
}

unsigned PRNG::get_random_number(unsigned ceil) noexcept
{
	if (ceil == 0u)
	{
		return 0u;
	}

	unsigned true_ceil = ~0u - (~0u % ceil);

	unsigned random_number;

	do
	{
		random_number = get_random_type<unsigned>();
	}
	while (random_number > true_ceil);

	return random_number % ceil;
}

double PRNG::get_random_double(double ceil) noexcept
{
	double true_ceil = 1.0 - std::fmod(1.0, ceil);

	double random_number;

	do
	{
		random_number = get_random_type<double>();
		random_number /= std::pow(10.0, std::ceil(std::log10(random_number)));
	}
	while (random_number > true_ceil);

	return std::fmod(random_number, ceil);
}

unsigned char PRNG::get_random_byte() noexcept
{
	if (index == sizeof(buffer))
	{
		refresh_buffer();
	}

	return buffer[index++];
}

void PRNG::refresh_buffer() noexcept
{
	error = RAND_bytes(buffer, sizeof(buffer)) != 1;
	index = 0u;
}
