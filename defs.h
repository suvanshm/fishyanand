#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>
#include <stdio.h>

#define DEBUG // can comment this out to remove debug and make it faster

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if(!(n)) { \
    printf("%s - Failed", #n); \
    printf("On %s ", __DATE__); \
    printf("At %s ", __TIME__); \
    printf("In File %s ", __FILE__); \
    printf("At Line %d\n", __LINE__); \
    exit(1); \
}
#endif


typedef unsigned long long U64;
/*
idea being that we can use 64 bit integer to represent the board, 1 bit per sq 
and then use bitwise operations to manipulate the board
for eg. U64 pawns[3] will have 3 ints, each representing where the pawns are for white, black, and both respectively.

least sig digit A1, B1... till H1 going up rank wise from there, structure of int will be like this: 
H8 G8 ... A8 H7 G7 ... A7 ... H1 G1 ... C1 B1 A1
*/

#define NAME "fishyanand 1.0"

#define BRD_SQ_NUM 120 // see explanation below
#define MAXGAMEMOVES 1024 // arbitrarily chosen, should be enough 
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" // starting position


enum {EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK};
enum {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE};
enum {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE};
enum {WHITE, BLACK, BOTH};

/* 
imagine a rect of numbers 12x10
0  1  2  3  4  5  6  7  8  9
 10 11 12 13 14 15 16 17 18 19
 20 21 22 23 24 25 26 27 28 29
 30 31 32 33 34 35 36 37 38 39
 40 41 42 43 44 45 46 47 48 49
 50 51 52 53 54 55 56 57 58 59
 60 61 62 63 64 65 66 67 68 69
 70 71 72 73 74 75 76 77 78 79
 80 81 82 83 84 85 86 87 88 89
 90 91 92 93 94 95 96 97 98 99
 100 101 102 103 104 105 106 107 108 109 
 110 111 112 113 114 115 116 117 118 119

we carve out playing board from within this, leaving 2 rows on top and bottom and 1 file on each side as buffer 
to calculate against illegal moves.
we need 2 extra rows just for handling knight movement, 
but only 1 extra file is needed to protect against knight move since they wrap.
 
*/
enum {A1 = 21, B1, C1, D1, E1, F1, G1, H1,
      A2 = 31, B2, C2, D2, E2, F2, G2, H2,
      A3 = 41, B3, C3, D3, E3, F3, G3, H3,
      A4 = 51, B4, C4, D4, E4, F4, G4, H4,
      A5 = 61, B5, C5, D5, E5, F5, G5, H5,
      A6 = 71, B6, C6, D6, E6, F6, G6, H6,
      A7 = 81, B7, C7, D7, E7, F7, G7, H7,
      A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD};

enum {FALSE, TRUE}; // false 0, true 1

enum {WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8}; // represent castling permissions as 4 bit int with each bit as boolean flag 

typedef struct 
{
    int move;
    int castlePerm; 
    int enPas;
    int fiftyMove;
    U64 posKey;

} S_UNDO;

typedef struct 
{
    int pieces[BRD_SQ_NUM]; 
    U64 pawns[3]; 
    // we use 64 bit type to repr squares on board, 1 to represent presence of pawn (0 for white, 1 for black, 2 for both)   
    
    int KingSq[2]; // position of kings 
    int castlePerm; // castling permissions

    int side; // side to move
    int enPas; // en passant square
    int fiftyMove; // 50 move rule

    int ply; // ply
    int hisPly; // history of plies

    U64 posKey; // hash key for position

    int pceNum[13]; // number of pieces on board (indexed based on enum defined earlier, blackking=12 and so on) 
    int bigPce[3]; // number of big pieces (not pawns) [b, w and both so 3 in total]
    int majPce[3]; // number of major pieces (rooks and queens)
    int minPce[3]; // number of minor pieces (bishops and knights)

    S_UNDO history[MAXGAMEMOVES]; // history of moves

    // piece list
    int pList[13][10]; // 13 pieces(including empty), 10 max of each piece (eg 2 R + 8 promotions)


} S_BOARD;

// MACROS
#define FR2SQ(f, r) ((21 + (f)) + ((r) * 10)) // converts file and rank to 120 based square
#define SQ64(sq120) (Sq120ToSq64[(sq120)]) // converts 120 based square to 64 based square
#define SQ120(sq64) (Sq64ToSq120[(sq64)]) // converts 64 based square to 120 based square
#define POP(b) PopBits(b) // pops least sig bit and returns it
#define CNT(b) CountBits(b) // counts number of bits set in a 64 bit number

// GLOBALS
extern int Sq120ToSq64[BRD_SQ_NUM];
extern int Sq64ToSq120[64];
extern U64 SetMask[64]; 
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120]; // 13 pieces (6w + 6b + empty), 120 squares ?? WHY NOT USE 64 HERE
extern U64 SideKey;
extern U64 CastleKeys[16]; // castling represented as 4 bits, so 16 possibilities
extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];


// FUNCTIONS 

// init. c
extern void AllInit();
extern void ClearBit(U64 *bb, U64 sq);
extern void SetBit(U64 *bb, U64 sq);

// bitboards.c 
extern void PrintBitBoard(U64 bb);
extern int PopBits(U64 *bb);
extern int CountBits(U64 b);

// hashkeys.c 
extern U64 GeneratePosKey(const S_BOARD *pos);

// board.c 
extern void ResetBoard(S_BOARD *pos);
extern int ParseFEN(char *fen, S_BOARD *pos);
extern void PrintBoard(const S_BOARD *pos);

#endif