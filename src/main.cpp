#include "defines.hpp"

int main() {
  U64 playBitboard = 0ULL;

  playBitboard |= (1ULL << SQ64(static_cast<int>(Square::D2)));
  playBitboard |= (1ULL << SQ64(static_cast<int>(Square::D3)));
  playBitboard |= (1ULL << SQ64(static_cast<int>(Square::D4)));

  int sq64 = 0;
  while (playBitboard) {
    sq64 = POP(&playBitboard);

    printf("Popped square: %d\n", sq64);
    PrintBitboard(playBitboard);
  }

  return 0;
}
