#pragma once

#include <atomic>
#include <chrono>
#include <limits>
#include <vector>

/******* RANDOM NUMBER GENERATOR BY ZIFF **********/
/*implemented as a class*/

class RandomGenerator
{
public:
	RandomGenerator(uint8_t type = 1, int32_t seed = static_cast<int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()));

	int32_t next();
	double nextNormal();

	static constexpr int32_t RIMAX = std::numeric_limits<int32_t>::max();/*the maximum value the generator can return*/
private:
	static constexpr int32_t A = 471, B = 1586, C = 6988, D = 9689, M = 16383, N = 624, M2 = 397; /* additional constants and variables
																				 for the algorithm */
	uint32_t MATRIX_A = 0x9908b0dfUL;   /* constant vector a */
	int32_t UPPER_MASK = 0x80000000UL; /* most significant w-r bits */
	int32_t LOWER_MASK = 0x7fffffffUL; /* least significant r bits */
	std::vector<size_t> _mt; /* the array for the state vector  */
	int mti=N+1; /* mti==N+1 means mt[N] is not initialized */


	std::atomic<int32_t> nd;
	std::atomic<int32_t> ra[M+1];
	uint8_t _type;
};
