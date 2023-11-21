#include "prime.h"
#include <math.h>

/*
 * Return Whether x is or not a prime
 *
 * Returns:
 *    1 - prime
 *    0 - not prime
 *    -1 - undefined (i.e. x < 2)
 */

int is_prime(const int x) {
  if (x < 2) {
    return -1;
  }
  if (x < 4) {
    return 1;
  }
  if ((x % 2) == 0) {
    return 0;
  }
  for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
    if ((x % i) == 0) {
      return 0;
    }
  }
  return 1;
}

/*
 * return next prime after x , or x if x is a prime
 */
int next_prime(int x) {
  while (is_prime(x) != 1) {
    x++;
  }
  return x;
}
