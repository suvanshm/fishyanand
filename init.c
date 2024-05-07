#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];

U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120]; // 13 pieces (6w + 6b + empty), 120 squares ?? WHY NOT USE 64 HERE
U64 SideKey;
U64 CastleKeys[16]; // castling represented as 4 bits, so 16 possibilities

/*Generates random 64 bits, rand() only generates 15 bits so we keep shifting, 
and last 4 digits are anded with hex code 0xf = 1111*/
U64 Rand64() {
    U64 r = 0ULL;
    r = (U64)rand() | (U64)rand() << 15 | (U64)rand() << 30 | (U64)rand() << 45 | ((U64)rand() & 0xf)<< 60;
    return r;
}


void InitHashKeys() {
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 120; j++) {
            PieceKeys[i][j] = Rand64(); 
        }
    }

    SideKey = Rand64(); 

    for(int i = 0; i < 16; i++) {
        CastleKeys[i] = Rand64(); 
    }
}

void InitBitMasks() {

    // for(int i = 0; i < 64; i++) {
    //     SetMask[i] = 0ULL; 
    //     ClearMask[i] = 0ULL;
    // }

    for(int i = 0; i < 64; i++) {
        SetMask[i] = (1ULL << i); 
        ClearMask[i] = ~SetMask[i]; // 2's complement, 1s turn to 0 and vice versa 
    }

}

// clearbit mask has 0 at sq and 1 everywhere else, so & operation will clear that bit and leave everything else as is
void ClearBit(U64 *bb, U64 sq) {  *bb &= ClearMask[sq]; }
// setbit mask has 1 at sq operation and 0 everywhere else, so | operation will set that bit and leave everything else as is
void SetBit(U64 *bb, U64 sq) { *bb |= SetMask[sq]; }

void InitSq120To64() {

    int i = 0; 
    int file = FILE_A; 
    int rank = RANK_1; 
    int sq = 0; 
    int sq64 = 0; 

    for(i = 0; i < BRD_SQ_NUM; i++){
        Sq120ToSq64[i] = 65; 
    }

    // for(i = 0; i < 64; i++){
    //     Sq64ToSq120[i] = 65; 
    // }

    for(rank == RANK_1; rank <= RANK_8; ++rank){
        for(file = FILE_A; file <= FILE_H; file++) { 
            sq = FR2SQ(file, rank); 
            Sq64ToSq120[sq64] = sq; 
            Sq120ToSq64[sq] = sq64; 
            sq64++; 
        }
    }

}

void AllInit() {
    InitSq120To64(); 
    InitBitMasks();
    InitHashKeys();
}