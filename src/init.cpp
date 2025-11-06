#include "defines.hpp"

U64 SetMask[64];
U64 ClearMask[64];

void InitBitMasks() {
  int index = 0;

  for (index = 0; index < 64; index++) {
    SetMask[index] = 0ULL;
    ClearMask[index] = 0ULL;
  }

  for (index = 0; index < 64; index++) {
    SetMask[index] |= (1ULL << index);
    ClearMask[index] = ~SetMask[index];
  }
}

void AllInit() { InitBitMasks(); }