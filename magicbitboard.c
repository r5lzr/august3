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

  for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++)
  {
    attack_bitmask |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  };

  for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--)
  {
    attack_bitmask |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
	};

  for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++)
  {
    attack_bitmask |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  };

  for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--)
  {
    attack_bitmask |= (1ULL << (f + r*8));
    if(block & (1ULL << (f + r * 8))) break;
  };

	return attack_bitmask;
}

UInt64 rook_attack_mask(int square, UInt64 block)
{
  UInt64 attack_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

  for(r = rk+1; r <= 7; r++)
  {
    attack_bitmask |= (1ULL << (fl + r*8));
    if(block & (1ULL << (fl + r*8))) break;
  };

  for(r = rk-1; r >= 0; r--)
  {
    attack_bitmask |= (1ULL << (fl + r*8));
    if(block & (1ULL << (fl + r*8))) break;
  };

  for(f = fl+1; f <= 7; f++)
  {
    attack_bitmask |= (1ULL << (f + rk*8));
    if(block & (1ULL << (f + rk*8))) break;
  };

  for(f = fl-1; f >= 0; f--)
  {
    attack_bitmask |= (1ULL << (f + rk*8));
    if(block & (1ULL << (f + rk*8))) break;
  };

  return attack_bitmask;
}


