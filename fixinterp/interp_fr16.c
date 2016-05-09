//! interpolate 16-bit sign fract with unsigned phase

#include <stdint.h>
#include <stdio.h>

#define PHASE_BITS 32
#define TAB_BITS 10
#define TAB_SIZE (1 << TAB_BITS)
#define TAB_MASK (TAB_SIZE - 1)

// multiply 16-bit as signed fract (1.15)
int16_t mul_fr16(int16_t a, int16_t b) {
  int32_t y = (int32_t)a * (int32_t)b;
  return (int16_t)(y >> 15);
}

int16_t mix(int16_t a, int16_t b, int16_t bal) {
  // return (int16_t)((int32_t)a + mul_fr16((int32_t)b - (int32_t)a, bal));
  //  return mul_fr16(b, bal) + mul_fr16(a, 0x7fff - bal);
  
  //  printf("mix_a: %04x ; mix_b: %04x ; bal: %04x \r\n", a, b, bal);
  return a + mul_fr16(b-a, bal);
}

// linear interpolation

// cosine segment interpolation
const int32_t half_cos_tab[TAB_SIZE + 1] = {
#include "half_cos_fr32_1024.inc"
};

int16_t get_cos_mul(uint32_t phase) {
  int idxA = phase >> (PHASE_BITS - TAB_BITS);
  int idxB = (idxA + 1);
  uint32_t bal = phase & ((1 << (PHASE_BITS-TAB_BITS)) - 1);
  int16_t cos_a = (int16_t)(half_cos_tab[idxA] >> 16);
  int16_t cos_b = (int16_t)(half_cos_tab[idxB] >> 16);
			     
  //  printf("\r\nidxA: %d ; idxB: %d ; rem: %08X \r\n", idxA, idxB, bal);
  //  printf("cos_a: %08x ; cos_b: %08x \r\n", cos_a, cos_b);
  
  bal >>= (PHASE_BITS - TAB_BITS) - 15;
  //  printf (" cos bal: %08X \r\n ", bal);
  return mix( cos_a, cos_b, (int16_t)bal );
}

int16_t interp_cos(int16_t a, int16_t b, uint32_t phase) {
  return mix(a, b, get_cos_mul(phase));
  //  return get_cos_mul(phase);
}

/// test
int main(void) {
  int16_t a = 16383;
  int16_t b = -16384;
  int i = 0;
  uint32_t phase = 0;
  uint32_t phase0;
  
  int val;
  int tmp;
  
  // a "typical" phase increment
  int phi = (int)((float)0xffffffff / 44100.f * 22.f);

  #if 0
  printf(" \r\n a: %08X ; b: %08X ; (b-a): %08X ; phi: %08X \r\n",
	 a, b, b-a, phi);
  
  printf("\r\n tabmask: %08x \r\n", TAB_MASK);
#endif
  
  
  while (i < (1 << 14)) {
      //while(0) {
    val = interp_cos(a, b, phase);
    printf("%d \t %d \r\n", i, val);
    phase0 = phase;
    phase += phi;
    if(phase < phase0) {
      tmp = a;
      a = b;
      b = tmp;
    }
    i++;
  }
  
}
