#include <cstdio>

#include "defines.hpp"

int main() {
    int PieceOne = rand();
    int PieceTwo = rand();
    int PieceThree = rand();
    int PieceFour = rand();

    printf("Pieces: %X\n", PieceOne);
    printf("Pieces: %X\n", PieceTwo);
    printf("Pieces: %X\n", PieceThree);
    printf("Pieces: %X\n", PieceFour);

    int Key = PieceOne ^ PieceTwo ^ PieceThree ^ PieceFour;
    int TempKey = PieceOne;
    TempKey ^= PieceThree;
    TempKey ^= PieceFour;
    TempKey ^= PieceTwo;

    printf("Key: %X\n", Key);
    printf("TempKey: %X\n", TempKey);

    return 0;
}
