#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"

U64 pawn_attack_mask(int side, int square)
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

U64 knight_attack_mask(int square)
{
  U64 attack_bitmask = 0ULL;

  U64 bitboard = 0ULL;

  set_bit(bitboard, square);

  attack_bitmask |= (bitboard >> 17) & overflow_H;
  attack_bitmask |= (bitboard >> 15) & overflow_A;
  attack_bitmask |= (bitboard >> 10) & overflow_HG;
  attack_bitmask |= (bitboard >> 6) & overflow_AB;

  attack_bitmask |= (bitboard << 17) & overflow_A;
  attack_bitmask |= (bitboard << 15) & overflow_H;
  attack_bitmask |= (bitboard << 10) & overflow_AB;
  attack_bitmask |= (bitboard << 6) & overflow_HG;

  return attack_bitmask;
}

U64 king_attack_mask(int square)
{
  U64 attack_bitmask = 0ULL;

  U64 bitboard = 0ULL;

  set_bit(bitboard, square);

  attack_bitmask |= (bitboard >> 8);
  attack_bitmask |= (bitboard >> 9) & overflow_H;
  attack_bitmask |= (bitboard >> 7) & overflow_A;
  attack_bitmask |= (bitboard >> 1) & overflow_H;

  attack_bitmask |= (bitboard << 8);
  attack_bitmask |= (bitboard << 9) & overflow_A;
  attack_bitmask |= (bitboard << 7) & overflow_H;
  attack_bitmask |= (bitboard << 1) & overflow_A;

  return attack_bitmask;
}

U64 bishop_attack_mask(int square)
{
  U64 attack_bitmask = 0ULL;

}

void leaper_attacks_table()
{
  for (int square = 0; square < 64; square++)
  {
    pawn_attacks_table[white][square] = pawn_attack_mask(white, square);

    pawn_attacks_table[black][square] = pawn_attack_mask(black, square);

    knight_attacks_table[square] = knight_attack_mask(square);

    king_attacks_table[square] = king_attack_mask(square);
  }

  // accessing every bit mask attack
//  for (int square = 0; square < 64; square++)
//  {
//
//    show_bitboard(king_attacks_table[square]);
//
//  }
}





