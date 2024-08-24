#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

// based on relevant occupancy bits
UInt64 bishop_key_mask(int square)
{
  UInt64 key_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

	for(r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++) key_bitmask |= (1ULL << (r * 8 + f));
	for(r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--) key_bitmask |= (1ULL << (r * 8 + f));
	for(r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++) key_bitmask |= (1ULL << (r * 8 + f));
	for(r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--) key_bitmask |= (1ULL << (r * 8 + f));

  return key_bitmask;
}

UInt64 rook_key_mask(int square)
{
  UInt64 key_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

  for(r = rk + 1; r <= 6; r++) key_bitmask |= (1ULL << (r * 8 + fl));
  for(r = rk - 1; r >= 1; r--) key_bitmask |= (1ULL << (r * 8 + fl));
  for(f = fl + 1; f <= 6; f++) key_bitmask |= (1ULL << (rk * 8 + f));
  for(f = fl - 1; f >= 1; f--) key_bitmask |= (1ULL << (rk * 8 + f));

  return key_bitmask;
}

// attacks indexed from sliding piece lookup table
UInt64 bishop_attack_mask(int square, UInt64 block)
{
	UInt64 attack_bitmask = 0ULL;

	int rk = square / 8, fl = square % 8, r, f;

  for(r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++)
  {
    attack_bitmask |= (1ULL << (r * 8 + f));
    if(block & (1ULL << (r * 8 + f))) break;
  };

  for(r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--)
  {
    attack_bitmask |= (1ULL << (r * 8 + f));
    if(block & (1ULL << (r * 8 + f))) break;
	};

  for(r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++)
  {
    attack_bitmask |= (1ULL << (r * 8 + f));
    if(block & (1ULL << (r * 8 + f))) break;
  };

  for(r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--)
  {
    attack_bitmask |= (1ULL << (r * 8 + f));
    if(block & (1ULL << (r * 8 + f))) break;
  };

	return attack_bitmask;
}

UInt64 rook_attack_mask(int square, UInt64 block)
{
  UInt64 attack_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

  for(r = rk + 1; r <= 7; r++)
  {
    attack_bitmask |= (1ULL << (r * 8 + fl));
    if(block & (1ULL << (r * 8 + fl))) break;
  };

  for(r = rk - 1; r >= 0; r--)
  {
    attack_bitmask |= (1ULL << (r * 8 + fl));
    if(block & (1ULL << (r * 8 + fl))) break;
  };

  for(f = fl + 1; f <= 7; f++)
  {
    attack_bitmask |= (1ULL << (rk * 8 + f));
    if(block & (1ULL << (rk * 8 + f))) break;
  };

  for(f = fl - 1; f >= 0; f--)
  {
    attack_bitmask |= (1ULL << (rk * 8 + f));
    if(block & (1ULL << (rk * 8 + f))) break;
  };

  return attack_bitmask;
}

int count_bits(UInt64 block) {
  int count;
  for(count = 0; block; count++, block &= block - 1);
  return count;
}















