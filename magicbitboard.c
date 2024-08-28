#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitboard.h"
#include "magicbitboard.h"

// based on relevant occupancy bits
UInt64 bishop_occupancy_mask(int square)
{
  UInt64 occ_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

	for (r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++) occ_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--) occ_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++) occ_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--) occ_bitmask |= (1ULL << (r * 8 + f));

  return occ_bitmask;
}

UInt64 rook_occupancy_mask(int square)
{
  UInt64 occ_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

  for (r = rk + 1; r <= 6; r++) occ_bitmask |= (1ULL << (r * 8 + fl));
  for (r = rk - 1; r >= 1; r--) occ_bitmask |= (1ULL << (r * 8 + fl));
  for (f = fl + 1; f <= 6; f++) occ_bitmask |= (1ULL << (rk * 8 + f));
  for (f = fl - 1; f >= 1; f--) occ_bitmask |= (1ULL << (rk * 8 + f));

  return occ_bitmask;
}

// attacks indexed from sliding piece lookup table
UInt64 bishop_attack_mask(int square, UInt64 block)
{
	UInt64 attack_bitmask = 0ULL;

	int rk = square / 8, fl = square % 8, r, f;

  for (r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++)
  {
    attack_bitmask |= (1ULL << (r * 8 + f));
    if (block & (1ULL << (r * 8 + f))) break;
  };

  for (r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--)
  {
    attack_bitmask |= (1ULL << (r * 8 + f));
    if (block & (1ULL << (r * 8 + f))) break;
	};

  for (r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++)
  {
    attack_bitmask |= (1ULL << (r * 8 + f));
    if (block & (1ULL << (r * 8 + f))) break;
  };

  for (r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--)
  {
    attack_bitmask |= (1ULL << (r * 8 + f));
    if (block & (1ULL << (r * 8 + f))) break;
  };

	return attack_bitmask;
}

UInt64 rook_attack_mask(int square, UInt64 block)
{
  UInt64 attack_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

  for (r = rk + 1; r <= 7; r++)
  {
    attack_bitmask |= (1ULL << (r * 8 + fl));
    if (block & (1ULL << (r * 8 + fl))) break;
  };

  for (r = rk - 1; r >= 0; r--)
  {
    attack_bitmask |= (1ULL << (r * 8 + fl));
    if (block & (1ULL << (r * 8 + fl))) break;
  };

  for (f = fl + 1; f <= 7; f++)
  {
    attack_bitmask |= (1ULL << (rk * 8 + f));
    if (block & (1ULL << (rk * 8 + f))) break;
  };

  for (f = fl - 1; f >= 0; f--)
  {
    attack_bitmask |= (1ULL << (rk * 8 + f));
    if (block & (1ULL << (rk * 8 + f))) break;
  };

  return attack_bitmask;
}

// based on number of relevant bits
int count_bits(UInt64 block)
{
  int count;
  for (count = 0; block; count++, block &= block - 1);

  return count;
}

// based on bit mask variations of relevant occupancy bits
UInt64 key_mask(int index, int relevant_bits, UInt64 occ_mask)
{
	UInt64 key_bitmask = 0ULL;

	for (int count = 0; count < relevant_bits; count++)
	{
    int square = __builtin_ctzll(occ_mask);

    pop_bit(occ_mask, square);

    if (index & (1 << count))
      key_bitmask |= (1ULL << square);
	}

	return key_bitmask;
}

// generate 64-bit pseudo random numbers
UInt64 random_UInt64()
{
  UInt64 n1, n2, n3, n4;

  n1 = (UInt64)(rand()) & 0xFFFF;
  n2 = (UInt64)(rand()) & 0xFFFF;
  n3 = (UInt64)(rand()) & 0xFFFF;
  n4 = (UInt64)(rand()) & 0xFFFF;

  return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

UInt64 magic_number_candidate()
{
  return random_UInt64() & random_UInt64() & random_UInt64();
}

UInt64 find_magic_number(int square, int relevant_bits, int sliding_piece)
{
  UInt64 occ_mask = sliding_piece ? bishop_occupancy_mask(square) : rook_occupancy_mask(square);

  int max_occ_index = 1 << relevant_bits;

  for (int index = 0; index < max_occ_index; index++)
  {
    key_mask_table[index] = key_mask(index, relevant_bits, occ_mask);

    sliding_attack_table[index] = sliding_piece ? bishop_attack_mask(square, key_mask_table[index]) : rook_attack_mask(square, key_mask_table[index]);
  }

  for (int count = 0; count < 100000000; count++)
  {
    UInt64 magic_number = magic_number_candidate();

    if (count_bits((occ_mask * magic_number) & 0xFF00000000000000ULL) < 6) continue;

    memset(used_attacks, 0ULL, sizeof(used_attacks));

    int index, fail;

    for (index = 0, fail = 0; !fail && index < max_occ_index; index++)
    {
      int magic_index = (int)((key_mask_table[index] * magic_number) >> (64 - relevant_bits));

      if (used_attacks[magic_index] == 0ULL)
        used_attacks[magic_index] = sliding_attack_table[index];

      else if (used_attacks[magic_index] != sliding_attack_table[index])
        fail = 1;
    }

    if (!fail)
      return magic_number;
  }

  printf("failed");
  return 0ULL;
}

void init_magic_numbers()
{
  for (int square = 0; square < 64; square++)
    printf(" 0x%llxULL,\n", find_magic_number(square, rook_relevant_bits[square], rook));

  printf("\n\n");

  for (int square = 0; square < 64; square++)
    printf(" 0x%llxULL,\n", find_magic_number(square, bishop_relevant_bits[square], bishop));
}

void init_slider_attacks(int sliding_piece)
{
  for (int square = 0; square < 64; square++)
  {
    bishop_occupancy_table[square] = bishop_occupancy_mask(square);
    rook_occupancy_table[square] = rook_occupancy_mask(square);

    UInt64 occ_mask = sliding_piece ? bishop_occupancy_table[square] : rook_occupancy_table[square];

    int relevant_bits = count_bits(occ_mask);

    int max_occ_index = (1 << relevant_bits);

    for (int index = 0; index < max_occ_index; index++)
    {
      if (sliding_piece == bishop)
      {
        UInt64 occupancy = key_mask(index, relevant_bits, occ_mask);

        int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);

        bishop_attacks_table[square][magic_index] = bishop_attack_mask(square, occupancy);
      }

      else
      {
        UInt64 occupancy = key_mask(index, relevant_bits, occ_mask);

        int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);

        rook_attacks_table[square][magic_index] = rook_attack_mask(square, occupancy);
      }
    }
  }
}

UInt64 get_bishop_attacks(int square, UInt64 occupancy)
{
  occupancy &= bishop_occupancy_table[square];
  occupancy *= bishop_magic_numbers[square];
  occupancy >>= 64 - bishop_relevant_bits[square];

  return bishop_attacks_table[square][occupancy];
}

UInt64 get_rook_attacks(int square, UInt64 occupancy)
{
  occupancy &= rook_occupancy_table[square];
  occupancy *= rook_magic_numbers[square];
  occupancy >>= 64 - rook_relevant_bits[square];

  return rook_attacks_table[square][occupancy];
}

void slider_attacks_table()
{
  init_slider_attacks(bishop);
  init_slider_attacks(rook);
}
