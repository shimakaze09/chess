#include "defines.hpp"

namespace {

constexpr char kOccupiedSymbol = 'X';
constexpr char kEmptySymbol = '-';

}  // namespace

int PopBit(U64& bitboard) {
  if (bitboard == 0ULL) {
    return -1;
  }

  const auto lsbIndex = std::countr_zero(bitboard);
  bitboard &= bitboard - 1;
  return static_cast<int>(lsbIndex);
}

int CountBits(U64 bitboard) {
  return static_cast<int>(std::popcount(bitboard));
}

void PrintBitboard(U64 bitboard) {
  std::cout << '\n';

  for (int rank = static_cast<int>(Rank::R8);
       rank >= static_cast<int>(Rank::R1); --rank) {
    for (int file = static_cast<int>(File::A);
         file <= static_cast<int>(File::H); ++file) {
      const auto square120 = FR2SQ(file, rank);
      const auto square64 = Sq120ToSq64Index(square120);

      assert(square64 >= 0 && square64 < 64);
      const auto index = static_cast<std::size_t>(square64);
      const bool occupied = IsBitSet(bitboard, index);
      std::cout << (occupied ? kOccupiedSymbol : kEmptySymbol);
    }
    std::cout << '\n';
  }

  std::cout << '\n';
}