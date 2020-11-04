#ifndef PRNG_H
#define PRNG_H

#include <cstdint>
#include <type_traits>

namespace ogila
{
	class PRNG
	{
	public:
		PRNG() noexcept;

		unsigned get_random_number(unsigned ceil) noexcept;

		/**
		 * Returns a random double from [0, ceil], where ceil is (0, 1].
		 * @param  ceil a double in (0, 1]
		 * @return      a random double from [0, ceil]
		 */
		double get_random_double(double ceil) noexcept;

		unsigned char get_random_byte() noexcept;

		bool has_error() const noexcept
		{
			return error;
		}

	private:
		void refresh_buffer() noexcept;

		template <typename T>
		T get_random_type() noexcept;

		unsigned char buffer[2048u];
		unsigned index;
		bool error;
	};
}

template <typename T>
T ogila::PRNG::get_random_type() noexcept
{
	using IntType = typename std::conditional<sizeof(T) == 4u, unsigned, uint64_t>::type;
	IntType random = 0;

	for (unsigned i = 0u; i < sizeof(T); ++i)
	{
		random = (random << 8) | get_random_byte();
	}

	return static_cast<T>(random);
}

#endif
