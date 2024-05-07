#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"


int main () {
    AllInit();
    
    S_BOARD pos[1];

    ParseFEN(START_FEN, pos);
    PrintBoard(pos);

    ParseFEN(FEN1, pos);
    PrintBoard(pos);

    ParseFEN(FEN2, pos);
    PrintBoard(pos);

    ParseFEN(FEN3, pos);
    PrintBoard(pos);

    return 0;
}