#include "defines.hpp"

int main() {
  U64 playBitboard=0ULL;

  printf("Initial Bitboard:\n");
  PrintBitboard(playBitboard);

  playBitboard |=(1ULL << SQ64(static_cast<int>(Square::D2)));
  printf("After adding D2:\n");
  PrintBitboard(playBitboard);

  playBitboard |=(1ULL << SQ64(static_cast<int>(Square::G2)));
  printf("After adding G2:\n");
  PrintBitboard(playBitboard);

  return 0;
}
