/* MIT License
 *
 * Copyright (c) 2019 Gaëtan Cassiers Olivier Bronchain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <string.h>
#include <stdint.h>

#include "primitives.h"

#define CLYDE_128_NS 6                // Number of steps
#define CLYDE_128_NR 2 * CLYDE_128_NS // Number of rounds


// Apply a S-box layer to a Clyde-128 state.
static void sbox_layer(uint32_t* state) {
  uint32_t y1 = (state[0] & state[1]) ^ state[2];
  uint32_t y0 = (state[3] & state[0]) ^ state[1];
  uint32_t y3 = (y1 & state[3]) ^ state[0];
  uint32_t y2 = (y0 & y1) ^ state[3];
  state[0] = y0;
  state[1] = y1;
  state[2] = y2;
  state[3] = y3;
}

// Apply a L-box to a pair of Clyde-128 rows.
#define ROT32(x,n) ((uint32_t)(((x)>>(n))|((x)<<(32-(n)))))
static void lbox(uint32_t* x, uint32_t* y) {
  uint32_t a, b, c, d;
  a = *x ^ ROT32(*x, 12);
  b = *y ^ ROT32(*y, 12);
  a = a ^ ROT32(a, 3);
  b = b ^ ROT32(b, 3);
  a = a ^ ROT32(*x, 17);
  b = b ^ ROT32(*y, 17);
  c = a ^ ROT32(a, 31);
  d = b ^ ROT32(b, 31);
  a = a ^ ROT32(d, 26);
  b = b ^ ROT32(c, 25);
  a = a ^ ROT32(c, 15);
  b = b ^ ROT32(d, 15);
  *x = a;
  *y = b;
}

#define XORLS(DEST, OP) do { \
	(DEST)[0] ^= (OP)[0]; \
	(DEST)[1] ^= (OP)[1]; \
	(DEST)[2] ^= (OP)[2]; \
	(DEST)[3] ^= (OP)[3]; } while (0)

#define XORCST(DEST, LFSR) do { \
	(DEST)[0] ^= ((LFSR)>>3 & 0x1); \
	(DEST)[1] ^= ((LFSR)>>2 & 0x1); \
	(DEST)[2] ^= ((LFSR)>>1 & 0x1); \
	(DEST)[3] ^= ((LFSR) & 0x1); } while (0)

void clyde128_encrypt(clyde128_state state, const clyde128_state t, const unsigned char* k) {
	// Key schedule
	clyde128_state k_st;
	memcpy(k_st, k, CLYDE128_NBYTES);
	clyde128_state tk[3] = {
		{ t[0], t[1], t[2], t[3] },
		{ t[0] ^ t[2], t[1] ^ t[3], t[0], t[1] },
		{ t[2], t[3], t[0] ^ t[2], t[1] ^ t[3] }
	};
	XORLS(tk[0], k_st);
	XORLS(tk[1], k_st);
	XORLS(tk[2], k_st);

	// Datapath
	XORLS(state, tk[0]);
	uint32_t off = 0x924;		// 2-bits describing the round key
	uint32_t lfsr = 0x8;		// LFSR for round constant
	for (uint32_t s = 0; s < CLYDE_128_NS; s++) {
		sbox_layer(state);
		lbox(&state[0], &state[1]);
		lbox(&state[2], &state[3]);
		XORCST(state,lfsr);
		uint32_t b = lfsr & 0x1;
		lfsr = (lfsr^(b<<3) | b<<4)>>1;	// update LFSR

		sbox_layer(state);
		lbox(&state[0], &state[1]);
		lbox(&state[2], &state[3]);
		XORCST(state,lfsr);
		b = lfsr & 0x1;
		lfsr = (lfsr^(b<<3) | b<<4)>>1; // update LFSR
		off >>=2;
		XORLS(state, tk[off&0x03]);
	}
}
