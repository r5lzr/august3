#ifndef MOVE_GEN_H
#define MOVE_GEN_H


#define encode_move(source, target, piece, promoted, capture, double, enpassant, castling) \
(source) | (target << 6) | (piece << 12) | (promoted << 16) | (capture << 20) | (double << 21) | (enpassant << 22) | (castling << 23) \

#define get_move_source(move) (move & 0x3f)
#define get_move_target(move) ((move & 0xfc0) >> 6)
#define get_move_piece(move) ((move & 0xf000) >> 12)
#define get_move_promoted(move) ((move & 0xf0000) >> 16)
#define get_move_capture(move) ((move & 0x100000) >> 20)
#define get_move_double(move) ((move & 0x200000) >> 21)
#define get_move_enpassant(move) ((move & 0x400000) >> 22)
#define get_move_castling(move) ((move & 0x800000) >> 23)

typedef struct {
  int moves[256];
  int count;
} moves;
moves move_list;

char promoted_pieces[];

void show_attacked_squares(int side);
int is_square_attacked(int square, int side);
void generate_moves();
void show_move(int move);


#endif
