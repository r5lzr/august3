#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"

U64 pawn_attacks_table(int side, int square)
{

  // initialised as bitboard, will return attack bit mask
  U64 attack_bitmask = 0ULL;

  U64 bitboard = 0ULL;

  set_bit(bitboard, square);

  if (!side)
	{
		// generate white pawn attacks
		attack_bitmask |= (bitboard >> 7) & overflow_A;
		attack_bitmask |= (bitboard >> 9) & overflow_H;
	}

  else
	{
		// generate black pawn attacks
		attack_bitmask |= (bitboard << 7) & overflow_H;
		attack_bitmask |= (bitboard << 9) & overflow_A;
	}

  return attack_bitmask;
}
