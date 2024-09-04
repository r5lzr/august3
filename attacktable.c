#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "attacktable.h"

ui64 pawn_attack_mask(int side, int square)
{

  // initialised as bitboard, will return attack bit mask
  ui64 attack_bitmask = 0ULL;

  ui64 bitboard = 0ULL;

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

ui64 knight_attack_mask(int square)
{
  ui64 attack_bitmask = 0ULL;

  ui64 bitboard = 0ULL;

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

ui64 king_attack_mask(int square)
{
  ui64 attack_bitmask = 0ULL;

  ui64 bitboard = 0ULL;

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

void leaper_attacks_table()
{
  for (int square = 0; square < 64; square++)
  {
    pawn_attacks_table[white][square] = pawn_attack_mask(white, square);

    pawn_attacks_table[black][square] = pawn_attack_mask(black, square);

    knight_attacks_table[square] = knight_attack_mask(square);

    king_attacks_table[square] = king_attack_mask(square);
  }
}


const ui64 overflow_A = 18374403900871474942ULL;
const ui64 overflow_H = 9187201950435737471ULL;
const ui64 overflow_AB = 18229723555195321596ULL;
const ui64 overflow_HG = 4557430888798830399ULL;

ui64 pawn_attacks_table[2][64];
ui64 knight_attacks_table[64];
ui64 king_attacks_table[64];

