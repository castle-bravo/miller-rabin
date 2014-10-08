/*
 * file:    modular_pow.h
 * title:   Modular Power
 * author:  Alexander P. Gosselin
 * e-mail:  alexandergosselin@gmail.com
 * date:    October 7, 2014
 * 
 * desctription:
 *          Returns the base^exponent % modulus
 */
 
#ifndef _MODULAR_POW_H
#define _MODULAR_POW_H

#include <stdint.h>

uint32_t modular_pow(uint64_t base, uint32_t exponent, 
                     uint32_t modulus) {
  uint32_t result = 1;
  while (exponent > 0) {
    if (exponent % 2 == 1) {
      result = (result * base) % modulus;
    }
    exponent /= 2;
    if (exponent == 0) break;
    base = (base * base) % modulus;
  }
  return result;
}

#endif
