#include "randgen.h"
#include <cmath>
#include <QMessageBox>

RandomGenerator::RandomGenerator(uint8_t type, int32_t seed) : _mt(N), nd(0) /*initializes the generator from a given seed*/
{
	_type = type;
	seed = seed < 0 ? -seed : seed;
	seed = seed == 0 ? seed + 1 : seed;
	/* Ziff */
	ra[0] = static_cast<int32_t>(fmod(16807.0 * seed, 2147483647.0));
	for (int32_t i = 1; i <= M; i++)
		ra[i] = static_cast<int32_t>(fmod(16807.0 * ra[i - 1], 2147483647.0));
	/* Matsumoto Mersenne twister */
	_mt[0]= seed & 0xffffffffUL;
	for (mti=1; mti<N; mti++) {
		_mt[mti] =
		(1812433253UL * (_mt[mti-1] ^ (_mt[mti-1] >> 30)) + mti);
		// See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
		// In the previous versions, MSBs of the seed affect
		// only MSBs of the array mt[].
		// 2002/01/09 modified by Makoto Matsumoto
		_mt[mti] &= 0xffffffffUL;
		// for >32 bit machines
	}

}

int32_t RandomGenerator::next()
{
	switch(_type) {
	case 1:
		return ++nd, ra[nd & M] = ra[(nd-A) & M] ^ ra[(nd-B) & M] ^ ra[(nd-C) & M] ^ ra[(nd-D) & M];
		break;
	case 2:
		unsigned long y;
		   static unsigned long mag01[2]={0x0UL, MATRIX_A};
		   /* mag01[x] = x * MATRIX_A  for x=0,1 */

		   if (mti >= N) { /* generate N words at one time */
			   int kk;

			   for (kk=0;kk<N-M;kk++) {
				   y = (_mt[kk]&UPPER_MASK)|(_mt[kk+1]&LOWER_MASK);
				   _mt[kk] = _mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
			   }
			   for (;kk<N-1;kk++) {
				   y = (_mt[kk]&UPPER_MASK)|(_mt[kk+1]&LOWER_MASK);
				   _mt[kk] = _mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
			   }
			   y = (_mt[N-1]&UPPER_MASK)|(_mt[0]&LOWER_MASK);
			   _mt[N-1] = _mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

			   mti = 0;
		   }

		   y = _mt[mti++];

		   /* Tempering */
		   y ^= (y >> 11);
		   y ^= (y << 7) & 0x9d2c5680UL;
		   y ^= (y << 15) & 0xefc60000UL;
		   y ^= (y >> 18);

		   return y;

		break;


	}
	 /*retrieves a random
																								   number from the
																								   generator*/
}

double RandomGenerator::nextNormal()
{
	return static_cast<double>(next()) / RIMAX;
}
