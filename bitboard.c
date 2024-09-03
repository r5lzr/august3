#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

void show_bitboard(UInt64 bitboard)
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

  printf("     Bitboard: %llud\n\n", bitboard);
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

      for (int piece_bitboard = P; piece_bitboard <= k; piece_bitboard++)
      {
        if (get_bit(bitboards[piece_bitboard], square))
          piece = piece_bitboard;
      }

      printf(" %c", (piece == -1) ? '.' : ascii_pieces[piece]);
    }

    printf("\n");
  }

  printf("\n     a b c d e f g h\n\n");

  printf("  side: %s\n", !side ? "white" : "black");

  printf("  enpass: %s\n", (enpassant != no_sq) ? square_to_coordinates[enpassant] : "none");

  printf("  castling: %c%c%c%c\n\n", (castle & wk) ? 'K' : '-', (castle & wq) ? 'Q' : '-', (castle & bk) ? 'k' : '-', (castle & bq) ? 'q' : '-' );
}

void parse_fen(char *fen)
{
  memset(bitboards, 0ULL, sizeof(bitboards));

  memset(occupancies, 0ULL, sizeof(occupancies));

  side = 0;
  enpassant = no_sq;
  castle = 0;

  for (int square = 0; square < 64 && *fen && *fen != ' ';)
  {
    if ((*fen >= 'b' && *fen <= 'r') || (*fen >= 'B' && *fen <= 'R'))
    {
      int piece = char_pieces[*fen];

      set_bit(bitboards[piece], square);

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

  (*fen == 'w') ? (side = white) : (side = black);

  fen += 2;

  while (*fen != ' ')
  {
    switch (*fen)
    {
      case 'K': castle |= wk; break;
      case 'Q': castle |= wq; break;
      case 'k': castle |= bk; break;
      case 'q': castle |= bq; break;
      case '-': break;
    }

    fen++;
  }

  fen++;

  if (*fen != '-')
  {
    int file = fen[0] - 'a';
    int rank = 8 - (fen[1] - '0');

    enpassant = rank * 8 + file;
  }

  else
  {
    enpassant = no_sq;
  }

  for (int piece = P; piece <= K; piece++)
  {
    occupancies[white] |= bitboards[piece];
  }

  for (int piece = p; piece <= k; piece++)
  {
    occupancies[black] |= bitboards[piece];
  }

  occupancies[both] |= occupancies[white] | occupancies[black];

  printf("fen: '%s'\n", fen);
}

UInt64 bitboards[12];
UInt64 occupancies[3];

int side;
int enpassant = no_sq;
int castle;
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
