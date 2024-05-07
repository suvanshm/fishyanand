#include <stdio.h>
#include "defs.h" 


/* 
parses fen string to board_s
 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" 
*/
int ParseFEN(char *fen, S_BOARD *pos) {

    ResetBoard(pos);

    int empty_count = 0;
    int piece = 0; 
    int file = FILE_A; 
    int rank = RANK_8; // traversing top to bottom, A to H 
    int sq64 = 0; 
    int sq120 = 0;

    while((rank >= RANK_1) && (*fen)) {
        
        empty_count = 1; // so atleast for loop executes once

        switch (*fen) {

            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1': 
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8': piece = EMPTY; empty_count = *fen - '0'; break; // if number, that many empty squares

            case '/': 
            case ' ': rank--; file = FILE_A; fen++; continue;

            default: 
                printf("FEN invalid \n");
                return -1;
        }

        for(int i = 0; i < empty_count; i++) {
            sq120 = FR2SQ(file, rank); 
            if (piece != EMPTY) {
                pos->pieces[sq120] = piece; 
            }
            file++; 
        }
        fen++; // moves string along next char
    } 

    
    ASSERT(*fen == 'w' || *fen == 'b'); 
    pos->side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2; 

    for(int i = 0; i < 4; i++) {
        if(*fen == ' ') {
            break; 
        }
        switch(*fen) {
            case 'K': pos->castlePerm |= WKCA; break; 
            case 'Q': pos->castlePerm |= WQCA; break; 
            case 'k': pos->castlePerm |= BKCA; break; 
            case 'q': pos->castlePerm |= BQCA; break; 
            default: break; 
        }
        fen++; 
    }

    fen++; 

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    if(*fen != '-') {
        file = fen[0] - 'a'; 
        rank = fen[1] - '1'; 

        ASSERT(file >= FILE_A && file <= FILE_H); 
        ASSERT(rank >= RANK_1 && rank <= RANK_8); 

        pos->enPas = FR2SQ(file, rank); 
    }

    pos->posKey = GeneratePosKey(pos);

    return 0; 
}


void ResetBoard(S_BOARD *pos){

    for(int i = 0; i < BRD_SQ_NUM; i++) {
        pos->pieces[i] = OFFBOARD; 
    }

    for(int i = 0; i < 64; i++) {
        pos->pieces[SQ120(i)] = EMPTY; 
    }

    // b, w and both so 3 in total 
    for(int i = 0; i < 3; i++) {
        pos->bigPce[i] = 0; 
        pos->majPce[i] = 0; 
        pos->minPce[i] = 0; 
        pos->pawns[i] = 0ULL; 
    }

    for(int i = 0; i < 13; i++) {
        pos->pceNum[i] = 0; 
    }

    pos->KingSq[WHITE] = pos->KingSq[BLACK] = NO_SQ;
    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMove = 0;
    pos->ply = 0;
    pos->hisPly = 0;
    pos->castlePerm = 0;
    pos->posKey = 0ULL;

}

void PrintBoard(const S_BOARD *pos) { 
    int sq, file, rank, piece;

    printf("\nGame Board:\n\n"); 

    for(rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d  ", rank + 1); 
        for(file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file, rank); 
            piece = pos->pieces[sq]; 
            printf("%3c", PceChar[piece]); 
        }
        printf("\n"); 
    }

    printf("\n   ");
    for(file = FILE_A; file <= FILE_H; file++) {
        printf("%3c", 'a' + file); 
    }
    printf("\n");
    printf("side:%c\n", SideChar[pos->side]);
    printf("enPas:%d\n", pos->enPas);
    printf("castle:%c%c%c%c\n", 
        pos->castlePerm & WKCA ? 'K' : '-',
        pos->castlePerm & WQCA ? 'Q' : '-',
        pos->castlePerm & BKCA ? 'k' : '-',
        pos->castlePerm & BQCA ? 'q' : '-'
    );
    printf("PosKey:%llX\n", pos->posKey);
}