#include "defines.hpp"

int main() {
  AllInit();

  int index = 0;
  U64 playBitboard = 0ULL;

  SETBIT(playBitboard, 61);
  PrintBitboard(playBitboard);

  CLRBIT(playBitboard, 61);
  PrintBitboard(playBitboard);

  return 0;
}
