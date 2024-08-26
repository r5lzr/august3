#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "magicbitboard.h"

// based on relevant occupancy bits
UInt64 bishop_occupancy_mask(int square)
{
  UInt64 occ_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

	for(r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++) occ_bitmask |= (1ULL << (r * 8 + f));
	for(r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--) occ_bitmask |= (1ULL << (r * 8 + f));
	for(r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++) occ_bitmask |= (1ULL << (r * 8 + f));
	for(r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--) occ_bitmask |= (1ULL << (r * 8 + f));

  return occ_bitmask;
}

UInt64 rook_occupancy_mask(int square)
{
  UInt64 occ_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

  for(r = rk + 1; r <= 6; r++) occ_bitmask |= (1ULL << (r * 8 + fl));
  for(r = rk - 1; r >= 1; r--) occ_bitmask |= (1ULL << (r * 8 + fl));
  for(f = fl + 1; f <= 6; f++) occ_bitmask |= (1ULL << (rk * 8 + f));
  for(f = fl - 1; f >= 1; f--) occ_bitmask |= (1ULL << (rk * 8 + f));

  return occ_bitmask;
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

// based on number of relevant bits
int count_bits(UInt64 block)
{
  int count;
  for(count = 0; block; count++, block &= block - 1);
  return count;
}

// based on bit mask variations of relevant occupancy bits
UInt64 key_mask(int index, int relevant_bits, UInt64 occ_mask)
{
	UInt64 key_bitmask = 0ULL;

	for (int count = 0; count < relevant_bits; count++)
	{
	    int square = __builtin_ctzll(occ_mask);

	    pop_bit(occ_mask, square);

	    if (index & (1 << count))
		    key_bitmask |= (1ULL << square);
	}

	return key_bitmask;
}

// generate 64-bit pseudo random numbers
UInt64 random_UInt64()
{
  UInt64 n1, n2, n3, n4;

  n1 = (UInt64)(rand()) & 0xFFFF;
  n2 = (UInt64)(rand()) & 0xFFFF;
  n3 = (UInt64)(rand()) & 0xFFFF;
  n4 = (UInt64)(rand()) & 0xFFFF;

  return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}






