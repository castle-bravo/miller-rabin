/*
 * file:    miller-rabin.h
 * title:   Miller-Rabin primality test
 * author:  Alexander P. Gosselin
 * e-mail:  alexandergosselin@gmail.com
 * date:    October 7, 2014
 * 
 * Reference
 * ========= 
 * http://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
 * 
 * Variable Names
 * ==============
 *    I use the same variable names as in the wikipedia article linked 
 *    above. These are:
 *      n   number for which primality is being decided
 *      a   witness used to check primality of n
 *      A   set of witnesses
 *      k   number of witnesses to generate in non-deterministic test
 *      d   largest odd factor in n - 1 = d*2^s
 *      s   exponent on 2 in n - 1 = d*2^s
 * 
 * note:
 *    This implementation is only good for checking primality of
 *    numbers up to 2^32 bits. The reason for this is that the repeated
 *    squaring of x can generate numbers larger than 2^64 if n is
 *    larger than 2^32. This can be fixed in a future version that uses
 *    a 128-bit integer to represent x if n is greater than 2^32. 
 */

#ifndef _MILLER_RABIN_H_
#define _MILLER_RABIN_H

#include <random>
#include <stdint.h>
#include <vector>

#include "modular_pow.h"

bool is_prime(uint32_t n, bool deterministic = true, uint32_t k = 4) {
  if (n == 0 || n == 1) return false;
  if (n == 2 || n == 3) return true;
  if (n % 2 == 0) return false;
  std::vector<uint32_t> A; // set of witnesses
  if (k > n - 2) deterministic = true;
  if (deterministic) {
  if (n < 2047) {
      A = std::vector<uint32_t>({ 2 });
    } else if (n < 1373653) {
      A = std::vector<uint32_t>({ 2, 3 });
    } else if (n < 9090191) {
      A = std::vector<uint32_t>({ 31, 73 });
    } else if (n < 25326001) {
      A = std::vector<uint32_t>({ 2, 3, 5 });
    } else {
      deterministic = false;
    }
    /*if (n < 4759123141) { // larger than 32-bit integer
      A = std::vector<uint32_t>({ 2, 7, 61 });
    } else if (n < 1122004669633) {
      A = std::vector<uint32_t>({ 2, 13, 23, 1662803 });
    } else if (n < 2152302898747) {
      A = std::vector<uint32_t>({ 2, 3, 5, 7, 11 });
    } else if (n < 3474749660383) {
      A = std::vector<uint32_t>({ 2, 3, 5, 7, 11, 13 });
    } else if (n < 341550071728321) {
      A = std::vector<uint32_t>({ 2, 3, 5, 7, 11, 13, 17 });
    } else if (n < 3825123056546413051) {
      A = std::vector<uint32_t>({ 2, 3, 5, 7, 11, 13, 17, 19, 23 });
    } */ // else use randomly generated A
  }
  if (not deterministic) {
    std::default_random_engine generator;
    std::uniform_int_distribution<uint32_t> uniform(2, n - 1);
    for (uint32_t i = 0; i < k; i++) {
      A.push_back(uniform(generator));
    }
  }
  uint32_t d = n - 1;
  uint32_t s = 0;
  while (d % 2 == 0) {
    d /= 2;
    s++;
  }
  for (uint32_t a : A) {
    uint64_t x = modular_pow(a, d, n);
    if (x == 1 || x == n - 1) continue;
    for (uint32_t j = 1; j < s; j++) { 
      x *= x;
      if(x > n) x %= n; // will result in overflow if n > 2^32
      if (x == 1) return false;
      if (x == n - 1) goto next_witness;
    }
    return false;
    next_witness:;
  }
  return true;
}

#endif
