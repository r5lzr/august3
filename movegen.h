#ifndef MOVE_GEN_H
#define MOVE_GEN_H


#define encode_move(source, target, piece, promoted, capture, double, enpassant, castling) \
(source) | (target << 6) | (piece << 12) | (promoted << 16) | (capture << 20) | (double << 21) | (double << 22) | (castling << 23) \






void show_attacked_squares(int side);
int is_square_attacked(int square, int side);
void generate_moves();


#endif
