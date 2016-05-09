//! interpolate 16-bit sign fract with unsigned phase
#include <stdint.h>

#define PHASE_BITS 32
#define TAB_SIZE 1024

// multiply 16-bit as signed fract (1.15)
int16_t mul_fr16(int16_t a, int16_t b) {
  int32_t y = (int32_t)a * (int32_t)b;
  return (int16_t)y >> 15;
}


// linear interpolation

// interpolation from cosine segment table
const int16_t cos_tab[TAB_SIZE] = {
#include "cos_tab.inc"
};

