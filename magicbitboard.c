#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

UInt64 bishop_attack_mask(int square)
{
  UInt64 attack_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

	for (r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++) attack_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--) attack_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++) attack_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--) attack_bitmask |= (1ULL << (r * 8 + f));

  return attack_bitmask;
}
