#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitboard.h"
#include "magicbitboard.h"

// based on relevant occupancy bits
ui64 bishop_occupancy_mask(int square)
{
  ui64 occ_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

	for (r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++) occ_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--) occ_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++) occ_bitmask |= (1ULL << (r * 8 + f));
	for (r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--) occ_bitmask |= (1ULL << (r * 8 + f));

  return occ_bitmask;
}

ui64 rook_occupancy_mask(int square)
{
  ui64 occ_bitmask = 0ULL;

  int rk = square / 8, fl = square % 8, r, f;

  for (r = rk + 1; r <= 6; r++) occ_bitmask |= (1ULL << (r * 8 + fl));
  for (r = rk - 1; r >= 1; r--) occ_bitmask |= (1ULL << (r * 8 + fl));
  for (f = fl + 1; f <= 6; f++) occ_bitmask |= (1ULL << (rk * 8 + f));
  for (f = fl - 1; f >= 1; f--) occ_bitmask |= (1ULL << (rk * 8 + f));

  return occ_bitmask;
}

// attacks indexed from sliding piece lookup table
ui64 bishop_attack_mask(int square, ui64 block)
{
	ui64 attack_bitmask = 0ULL;

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

ui64 rook_attack_mask(int square, ui64 block)
{
  ui64 attack_bitmask = 0ULL;

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
int count_bits(ui64 block)
{
  int count;
  for (count = 0; block; count++, block &= block - 1);

  return count;
}

// based on bit mask variations of relevant occupancy bits
ui64 key_mask(int index, int relevant_bits, ui64 occ_mask)
{
	ui64 key_bitmask = 0ULL;

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
ui64 random_ui64()
{
  ui64 n1, n2, n3, n4;

  n1 = (ui64)(rand()) & 0xFFFF;
  n2 = (ui64)(rand()) & 0xFFFF;
  n3 = (ui64)(rand()) & 0xFFFF;
  n4 = (ui64)(rand()) & 0xFFFF;

  return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

ui64 magic_number_candidate()
{
  return random_ui64() & random_ui64() & random_ui64();
}

ui64 find_magic_number(int square, int relevant_bits, int sliding_piece)
{
  ui64 occ_mask = sliding_piece ? bishop_occupancy_mask(square) : rook_occupancy_mask(square);

  int max_occ_index = 1 << relevant_bits;

  for (int index = 0; index < max_occ_index; index++)
  {
    key_mask_table[index] = key_mask(index, relevant_bits, occ_mask);

    sliding_attack_table[index] = sliding_piece ? bishop_attack_mask(square, key_mask_table[index])
    : rook_attack_mask(square, key_mask_table[index]);
  }

  for (int count = 0; count < 100000000; count++)
  {
    ui64 magic_number = magic_number_candidate();

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

    ui64 occ_mask = sliding_piece ? bishop_occupancy_table[square] : rook_occupancy_table[square];

    int relevant_bits = count_bits(occ_mask);

    int max_occ_index = (1 << relevant_bits);

    for (int index = 0; index < max_occ_index; index++)
    {
      if (sliding_piece == bishop)
      {
        ui64 occupancy = key_mask(index, relevant_bits, occ_mask);

        int magic_index = (occupancy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);

        bishop_attacks_table[square][magic_index] = bishop_attack_mask(square, occupancy);
      }

      else
      {
        ui64 occupancy = key_mask(index, relevant_bits, occ_mask);

        int magic_index = (occupancy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);

        rook_attacks_table[square][magic_index] = rook_attack_mask(square, occupancy);
      }
    }
  }
}

ui64 get_bishop_attacks(int square, ui64 occupancy)
{
  occupancy &= bishop_occupancy_table[square];
  occupancy *= bishop_magic_numbers[square];
  occupancy >>= 64 - bishop_relevant_bits[square];

  return bishop_attacks_table[square][occupancy];
}

ui64 get_rook_attacks(int square, ui64 occupancy)
{
  occupancy &= rook_occupancy_table[square];
  occupancy *= rook_magic_numbers[square];
  occupancy >>= 64 - rook_relevant_bits[square];

  return rook_attacks_table[square][occupancy];
}

ui64 get_queen_attacks(int square, ui64 occupancy)
{
  return (get_bishop_attacks(square, occupancy)) | (get_rook_attacks(square, occupancy));
}

// queen already init by bishop and rook together
void slider_attacks_table()
{
  init_slider_attacks(bishop);
  init_slider_attacks(rook);
}


const int bishop_relevant_bits[64] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6,
};
const int rook_relevant_bits[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12,
};

ui64 key_mask_table[4096];
ui64 sliding_attack_table[4096];
ui64 used_attacks[4096];

ui64 rook_magic_numbers[64] = {
 0x80004000976080ULL,
 0x1040400010002000ULL,
 0x4880200210000980ULL,
 0x5280080010000482ULL,
 0x200040200081020ULL,
 0x2100080100020400ULL,
 0x4280008001000200ULL,
 0x1000a4425820300ULL,
 0x29002100800040ULL,
 0x4503400040201004ULL,
 0x209002001004018ULL,
 0x1131000a10002100ULL,
 0x9000800120500ULL,
 0x10e001804820010ULL,
 0x29000402000100ULL,
 0x2002000d01c40292ULL,
 0x80084000200c40ULL,
 0x10004040002002ULL,
 0x201030020004014ULL,
 0x80012000a420020ULL,
 0x129010008001204ULL,
 0x6109010008040002ULL,
 0x950010100020004ULL,
 0x803a0000c50284ULL,
 0x80004100210080ULL,
 0x200240100140ULL,
 0x20004040100800ULL,
 0x4018090300201000ULL,
 0x4802010a00102004ULL,
 0x2001000900040002ULL,
 0x4a02104001002a8ULL,
 0x2188108200204401ULL,
 0x40400020800080ULL,
 0x880402000401004ULL,
 0x10040800202000ULL,
 0x604410a02001020ULL,
 0x200200206a001410ULL,
 0x86000400810080ULL,
 0x428200040600080bULL,
 0x2001000041000082ULL,
 0x80002000484000ULL,
 0x210002002c24000ULL,
 0x401a200100410014ULL,
 0x5021000a30009ULL,
 0x218000509010010ULL,
 0x4000400410080120ULL,
 0x20801040010ULL,
 0x29040040820011ULL,
 0x4080400024800280ULL,
 0x500200040100440ULL,
 0x2880142001004100ULL,
 0x412020400a001200ULL,
 0x18c028004080080ULL,
 0x884001020080401ULL,
 0x210810420400ULL,
 0x801048745040200ULL,
 0x4401002040120082ULL,
 0x408200210012ULL,
 0x110008200441ULL,
 0x2010002004100901ULL,
 0x801000800040211ULL,
 0x480d000400820801ULL,
 0x820104201280084ULL,
 0x1001040311802142ULL,
};

ui64 bishop_magic_numbers[64] = {
 0x1024b002420160ULL,
 0x1008080140420021ULL,
 0x2012080041080024ULL,
 0xc282601408c0802ULL,
 0x2004042000000002ULL,
 0x12021004022080ULL,
 0x880414820100000ULL,
 0x4501002211044000ULL,
 0x20402222121600ULL,
 0x1081088a28022020ULL,
 0x1004c2810851064ULL,
 0x2040080841004918ULL,
 0x1448020210201017ULL,
 0x4808110108400025ULL,
 0x10504404054004ULL,
 0x800010422092400ULL,
 0x40000870450250ULL,
 0x402040408080518ULL,
 0x1000980a404108ULL,
 0x1020804110080ULL,
 0x8200c02082005ULL,
 0x40802009a0800ULL,
 0x1000201012100ULL,
 0x111080200820180ULL,
 0x904122104101024ULL,
 0x4008200405244084ULL,
 0x44040002182400ULL,
 0x4804080004021002ULL,
 0x6401004024004040ULL,
 0x404010001300a20ULL,
 0x428020200a20100ULL,
 0x300460100420200ULL,
 0x404200c062000ULL,
 0x22101400510141ULL,
 0x104044400180031ULL,
 0x2040040400280211ULL,
 0x8020400401010ULL,
 0x20100110401a0040ULL,
 0x100101005a2080ULL,
 0x1a008300042411ULL,
 0x120a025004504000ULL,
 0x4001084242101000ULL,
 0xa020202010a4200ULL,
 0x4000002018000100ULL,
 0x80104000044ULL,
 0x1004009806004043ULL,
 0x100401080a000112ULL,
 0x1041012101000608ULL,
 0x40400c250100140ULL,
 0x80a10460a100002ULL,
 0x2210030401240002ULL,
 0x6040aa108481b20ULL,
 0x4009004050410002ULL,
 0x8106003420200e0ULL,
 0x1410500a08206000ULL,
 0x92548802004000ULL,
 0x1040041241028ULL,
 0x120042025011ULL,
 0x8060104054400ULL,
 0x20004404020a0a01ULL,
 0x40008010020214ULL,
 0x4000050209802c1ULL,
 0x208244210400ULL,
 0x10140848044010ULL,
};

ui64 bishop_occupancy_table[64];
ui64 rook_occupancy_table[64];
ui64 bishop_attacks_table[64][512];
ui64 rook_attacks_table[64][4096];




