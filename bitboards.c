#include <stdio.h>
#include "defs.h"


/*
bit table and popping bits is black magic C wizardry from: 
https://www.chessprogramming.org/Looking_for_Magics
*/

const int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
  51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
  26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
  58, 20, 37, 17, 36, 8
};

/* returns index of least signifcant set bit and unsets it*/
int PopBits(U64 *bb) {
  U64 b = *bb ^ (*bb - 1); // bb XOR (bb-1) will get a number where least sig bit and all bits to the right are set, rest are unset 
  unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32)); // XORing the lower 32 bits with the upper 32 bits (???? why tf)
  *bb &= (*bb - 1); // unset the least sig bit (see explanation in CountBits)
  return BitTable[(fold * 0x783a9b23) >> 26]; // magic number, have no idea how or why this works
}


int CountBits(U64 b) {
    int r; 
    for(r=0; b; r++, b &= b-1);
    // b-1 flips all bits from last set bit (inclusive) till the end, now b&b-1 will leave the rest of the number as is, but the last set bit will be unset
    return r; 
} 

void PrintBitBoard(U64 bb) {
    U64 shiftMe = 1ULL; 
    int rank = 0; 
    int file = 0; 
    int sq = 0; 
    int sq64 = 0; 

    for(rank = RANK_8; rank >= RANK_1; rank--) {
        for(file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file, rank); // 120 base 
            sq64 = SQ64(sq); // 64 base 

            if ((shiftMe << sq64) & bb) {
                printf("x");
            }
            else {
                printf("-");
            } 
        }
        printf("\n");
    }
}