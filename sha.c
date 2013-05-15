// Barebones optimized bloocoin miner
// Copyright (C) 2013 T. Galvin

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "sha.h"

#define S64(b,x)	(((x) >> (b)) | ((x) << (64 - (b))))
#define R(b,x) 		((x) >> (b))
#define Ch(x,y,z)	(((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z)	(((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))


void sha512SingleBlock(char *input, char *output)
{
	uint64_t v[8] = {0x6a09e667f3bcc908UL, 0xbb67ae8584caa73bUL,
		0x3c6ef372fe94f82bUL, 0xa54ff53a5f1d36f1UL,
		0x510e527fade682d1UL, 0x9b05688c2b3e6c1fUL,
		0x1f83d9abfb41bd6bUL, 0x5be0cd19137e2179UL};
	
}
