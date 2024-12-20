#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "bitboard.h"
#include "zobrist.h"
#include "uci.h"
#include "attacktable.h"
#include "magicbitboard.h"
#include "movegen.h"
#include "perft.h"
#include "util.h"
#include "evaluation.h"
#include "search.h"
#include "ttable.h"

ui64 piece_bitboards[12];
ui64 side_bitboards[3];

char ascii_pieces[12] = "PNBRQKpnbrqk";
int char_pieces[] = {
    ['P'] = P,
    ['N'] = N,
    ['B'] = B,
    ['R'] = R,
    ['Q'] = Q,
    ['K'] = K,
    ['p'] = p,
    ['n'] = n,
    ['b'] = b,
    ['r'] = r,
    ['q'] = q,
    ['k'] = k
};
const char *square_to_coordinates[] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};

ui64 zobrist_key;

void show_bitboard(ui64 bitboard)
{
  printf("\n");

  for (int rank = 0; rank < 8; rank++)
  {
    printf("  %d ", 8 - rank);

    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      printf(" %d", get_bit(bitboard, square) ? 1 : 0);
    }
    printf("\n");

  }
  printf("\n     a b c d e f g h\n\n");

  printf("     Bitboard: %" PRIu64 "\n\n", bitboard);
}

void show_board()
{
  printf("\n");

  for (int rank = 0; rank < 8; rank++)
  {
    printf("  %d ", 8 - rank);

    for (int file = 0; file < 8; file++)
    {
      int square = rank * 8 + file;

      int piece = -1;

      for (int char_piece = P; char_piece <= k; char_piece++)
      {
        if (get_bit(piece_bitboards[char_piece], square))
          piece = char_piece;
      }
      printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);

    }
    printf("\n");

  }

  printf("\n     a b c d e f g h\n\n");

  printf("  side: %s\n", !board.side ? "white" : "black");

  printf("  enpass: %s\n", (board.enpassant != no_sq) ? square_to_coordinates[board.enpassant] : "none");

  printf("  castling: %c%c%c%c\n\n",
   (board.castle & wk) ? 'K' : '-',
   (board.castle & wq) ? 'Q' : '-',
   (board.castle & bk) ? 'k' : '-',
   (board.castle & bq) ? 'q' : '-');

  printf("  zobrist key: %" PRIx64 "\n\n", zobrist_key);
}

void parse_fen(char *fen, FenBoard *board)
{
  memset(piece_bitboards, 0ULL, sizeof(piece_bitboards));

  memset(side_bitboards, 0ULL, sizeof(side_bitboards));

  board->side = 0;
  board->enpassant = no_sq;
  board->castle = 0;

  repetition_index = 0;

  memset(repetition_table, 0ULL, sizeof(repetition_table));

  for (int square = 0; square < 64 && *fen && *fen != ' ';)
  {
    if ((*fen >= 'b' && *fen <= 'r') || (*fen >= 'B' && *fen <= 'R'))
    {
      int piece = char_pieces[(unsigned char)*fen];

      set_bit(piece_bitboards[piece], square);

      square++;
      fen++;
    }

    else if (*fen >= '1' && *fen <= '8')
    {
      int offset = *fen - '0';

      square += offset;
      fen++;
    }

    else if (*fen == '/')
    {
      fen++;
    }

    else
    {
      printf("Invalid character in fen: %c\n", *fen);
      fen++;
    }
  }

  fen++;

  (*fen == 'w') ? (board->side = white) : (board->side = black);

  fen += 2;

  while (*fen != ' ')
  {
    switch (*fen)
    {
      case 'K': board->castle |= wk; break;
      case 'Q': board->castle |= wq; break;
      case 'k': board->castle |= bk; break;
      case 'q': board->castle |= bq; break;
      case '-': break;
    }

    fen++;
  }

  fen++;

  if (*fen != '-')
  {
    int file = fen[0] - 'a';
    int rank = 8 - (fen[1] - '0');

    board->enpassant = rank * 8 + file;
  }

  else
  {
    board->enpassant = no_sq;
  }

  for (int piece = P; piece <= K; piece++)
  {
    side_bitboards[white] |= piece_bitboards[piece];
  }

  for (int piece = p; piece <= k; piece++)
  {
    side_bitboards[black] |= piece_bitboards[piece];
  }

  side_bitboards[both] |= side_bitboards[white] | side_bitboards[black];

  zobrist_key = generate_zobrist_key();
}

