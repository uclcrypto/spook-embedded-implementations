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

static uint32_t xtime(uint32_t x) {
#if (DBOX8==1)
    uint32_t x_msk1 = x & 0x80808080;
    uint32_t x_msk2 = x & 0xefefefef;
    return ((x_msk2 << 1) | (x_msk1 >> 7)) ^ (x_msk1 >> (8-1));
#else
    return ((x << 1)  | (x >> 31)) ^ (x >> (31-8));
#endif
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
    uint32_t x1 = state[0][row];
    uint32_t x2 = state[1][row];
    uint32_t x3 = state[2][row];
    uint32_t x4 = state[3][row];

    uint32_t y1 = x2;
    uint32_t y2 = x3 ^ x4;
    uint32_t y3 = x4;
    uint32_t y4 = x1 ^ x2;

    uint32_t z1 = y2;
    uint32_t z2 = y3 ^ xtime(y4);
    uint32_t z3 = y4;
    uint32_t z4 = y1 ^ y2;

    uint32_t t = xtime(z4);
    uint32_t w1 = z2;
    uint32_t w2 = z3 ^ t;
    uint32_t w3 = t;
    uint32_t w4 = z1 ^ xtime(z2);

    state[0][row] = w1 ^ w2;
    state[1][row] = w2;
    state[2][row] = w3 ^ w4;
    state[3][row] = w4;
#endif // SMALL_PERM
  }
}
