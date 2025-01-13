#ifndef BITBOARD_H
#define BITBOARD_H

#include <inttypes.h>

typedef unsigned long long ui64;

#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define initial_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define kiwipete_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"

enum {
  a8, b8, c8, d8, e8, f8, g8, h8,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a1, b1, c1, d1, e1, f1, g1, h1,
  no_sq
};
enum {white, black, both};
enum {wk = 8, wq = 4, bk = 2, bq = 1};
enum {P, N, B, R, Q, K, p, n, b, r, q, k};

extern ui64 piece_bitboards[12];
extern ui64 side_bitboards[3];

typedef struct
{
  int side;
  int enpassant;
  int castle;
} FenBoard;
extern FenBoard board;

extern char ascii_pieces[12];
extern int char_pieces[];
extern const char *square_to_coordinates[];

void show_bitboard(ui64 bitboard);
void show_board();
void parse_fen(char *fen, FenBoard *board);

#endif
