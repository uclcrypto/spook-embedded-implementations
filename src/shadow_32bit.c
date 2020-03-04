/* MIT 
 *
 * Copyright (c) 2019 Gaëtan Cassiers
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
#include "primitives.c"

#define CLYDE_128_NS 6                // Number of steps
#define CLYDE_128_NR 2 * CLYDE_128_NS // Number of rounds
#define SHADOW_NS 6                   // Number of steps
#define SHADOW_NR 2 * SHADOW_NS       // Number of roundsv

#if (SHCST==1)
#define XORCST(DEST, LFSR, SHIFT) do { \
    (DEST)[0] ^= ((LFSR)>>3 & 0x1)<< (SHIFT); \
    (DEST)[1] ^= ((LFSR)>>2 & 0x1)<< (SHIFT); \
    (DEST)[2] ^= ((LFSR)>>1 & 0x1)<< (SHIFT); \
    (DEST)[3] ^= ((LFSR) & 0x1)<< (SHIFT); } while (0)
#elif (SHCST==8)
#define XORCST(DEST, LFSR, SHIFT) do { \
    (DEST)[0] ^= (LFSR) & (0xff << (8*(SHIFT))); } while(0)
#elif (SHCST==32)
#define XORCST(DEST, LFSR, SHIFT) do { \
    (DEST)[0] ^= (LFSR); \
    (LFSR) = ROTL((LFSR),8); } while (0)
#endif

static uint32_t update_lfsr(uint32_t lfsr) {
        uint32_t b = lfsr & 0x1;
        return (lfsr^(b<<3) | b<<4)>>1;
}

// Shadow permutation. Updates state.
void shadow(shadow_state state) {
    uint32_t lfsr = 0x8;
    for (unsigned int s = 0; s < SHADOW_NS; s++) {
        #pragma GCC unroll 0
        for (unsigned int b = 0; b < MLS_BUNDLES; b++) {
            sbox_layer(state[b]);
            lbox(&state[b][0], &state[b][1]);
            lbox(&state[b][2], &state[b][3]);
            XORCST(state[b], lfsr, b);
            sbox_layer(state[b]);
        }

        lfsr = update_lfsr(lfsr);

        dbox_mls_layer(state,&lfsr);

        lfsr = update_lfsr(lfsr);
    }
}
