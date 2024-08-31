#ifndef MAGIC_BITBOARD_H
#define MAGIC_BITBOARD_H


const enum {rook, bishop};

// shows relevant occupancy bits relative to board position, reduces bit count calls
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

UInt64 key_mask_table[4096];

UInt64 sliding_attack_table[4096];

UInt64 used_attacks[4096];

UInt64 rook_magic_numbers[64] = {
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

UInt64 bishop_magic_numbers[64] = {
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

UInt64 bishop_occupancy_table[64];

UInt64 rook_occupancy_table[64];

UInt64 bishop_attacks_table[64][512];

UInt64 rook_attacks_table[64][4096];


#endif
