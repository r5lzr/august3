#include <stdio.h>
#include <stdlib.h>
#include "bitboard.h"

UInt64 rook_occupancy_mask(int square);
UInt64 key_mask(int index, int relevant_bits, UInt64 occ_mask);
UInt64 magic_number_candidate();
UInt64 random_UInt64();

int main()
{
  leaper_attacks_table();
//
//  UInt64 block = 0ULL;
//  set_bit(block, e4);
//  set_bit(block, a4);
//  set_bit(block, b4);
//  set_bit(block, g3);

//  printf("bitcount: %d\n", count_bits(block));

//  for (int square = 0; square < 64; square++)
//  {
//    show_bitboard(rook_attack_mask(square, block));
//  }

//  UInt64 occupancy_mask = rook_occupancy_mask(a1);

//  for(int index = 0; index < 4096; index++)
//  {
//    show_bitboard(key_mask(index, count_bits(occupancy_mask), occupancy_mask));
//
//  }

//  show_bitboard(key_mask(4095, count_bits(occupancy_mask), occupancy_mask));
//    show_bitboard(random_UInt64() & random_UInt64() & random_UInt64());
    show_bitboard(magic_number_candidate());


  return 0;
}
