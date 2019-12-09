#include "primitives.h"

// Apply a S-box layer to a Clyde-128 state.
void sbox_layer(uint32_t* state) {
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
void lbox(uint32_t* x, uint32_t* y) {
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

// Apply a D-box layer to a Shadow state.
void dbox_mls_layer(shadow_state state) {
  for (unsigned int row = 0; row < LS_ROWS; row++) {
#if SMALL_PERM
    uint32_t x = state[0][row];
    uint32_t y = state[1][row];
    uint32_t z = state[2][row];
    state[0][row] = x ^ y ^ z;
    state[1][row] = x ^ z;
    state[2][row] = x ^ y;
#else
    uint32_t w = state[0][row];
    uint32_t x = state[1][row];
    uint32_t y = state[2][row];
    uint32_t z = state[3][row];
    uint32_t u = w ^ x;
    uint32_t v = y ^ z;
    state[0][row] = x ^ v;
    state[1][row] = w ^ v;
    state[2][row] = u ^ z;
    state[3][row] = u ^ y;
#endif // SMALL_PERM
  }
}
