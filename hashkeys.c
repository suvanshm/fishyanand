#include "defs.h"

/*
we generate a bunch of random 64bit ints for each piece on each square in init.c, and castling possibilities, and side to move
then we xor them to get a unique key for each position
*/
U64 GeneratePosKey(const S_BOARD *pos) {

    U64 key = 0; 
    int piece = EMPTY; 

    for(int sq =0; sq < BRD_SQ_NUM; sq++) { 
        piece = pos->pieces[sq]; 
        if (sq != NO_SQ && piece != EMPTY) { 
            ASSERT(piece >= wP && piece <= bK)
            key ^= PieceKeys[piece][sq];
        } 
    }

    if(pos->side == WHITE) {
        key ^= SideKey;
    }

    if(pos->enPas != NO_SQ) { 
        ASSERT(pos->enPas >= 0 && pos->enPas < BRD_SQ_NUM)
        key ^= PieceKeys[EMPTY][pos->enPas]; // since a bunch for empty are created across all sqs anyway, we just use them here
    }

    key ^= CastleKeys[pos->castlePerm];

    return key; 

}