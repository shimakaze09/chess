#include "defines.hpp"

void PrintBitboard(U64 bb) {
  U64 shiftMe = 1ULL;

  int rank = 0;
  int file = 0;
  int sq = 0;
  int sq64 = 0;

  printf("\n");
  for (rank = static_cast<int>(Rank::R8); rank >= static_cast<int>(Rank::R1);
       rank--) {
    for (file = static_cast<int>(File::A); file <= static_cast<int>(File::H);
         file++) {
      sq = FR2SQ(file, rank);
      sq64 = SQ64(sq);
      if (bb & (shiftMe << sq64)) {
        printf("X");
      } else {
        printf("-");
      }
    }
    printf("\n");
  }
  printf("\n\n");
}