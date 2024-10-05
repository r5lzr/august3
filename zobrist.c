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
      printf("%llx\n", piece_keys[piece][square]);
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
