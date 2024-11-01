#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"
#include "zobrist.h"
#include "search.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "util.h"
#include "evaluation.h"

ui64 piece_keys[12][64];
ui64 enpassant_keys[64];
ui64 castle_keys[16];
ui64 side_key;

void init_zobrist_hash()
{
  for (int piece = P; piece <= k; piece++)
  {
    for (int square = 0; square < 64; square++)
    {
      piece_keys[piece][square] = random_ui64();
    }
  }

  for (int square = 0; square < 64; square++)
  {
    enpassant_keys[square] = random_ui64();
  }

  for (int decimal = 0; decimal < 16; decimal++)
  {
    castle_keys[decimal] = random_ui64();
  }

  side_key = random_ui64();
}

ui64 generate_zobrist_key()
{
  ui64 hash_key = 0ULL;

  ui64 bitboard;

  for (int piece = P; piece <= k; piece++)
  {
    bitboard = piece_bitboards[piece];

    while (bitboard)
    {
      int square = __builtin_ctzll(bitboard);

      hash_key ^= piece_keys[piece][square];

      pop_bit(bitboard, square);
    }
  }

  if (board.enpassant != no_sq)
  {
    hash_key ^= enpassant_keys[board.enpassant];
  }

  hash_key ^= castle_keys[board.castle];

  if (board.side == black)
  {
    hash_key ^= side_key;
  }

  return hash_key;
}
