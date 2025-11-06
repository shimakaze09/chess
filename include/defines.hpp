#pragma once

#include "pch.hpp"

#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)                     \
  if (!(n)) {                         \
    printf("%s - Failed", #n);        \
    printf("On %s ", __DATE__);       \
    printf("At %s ", __TIME__);       \
    printf("In File %s ", __FILE__);  \
    printf("At Line %d\n", __LINE__); \
    exit(1);                          \
  }
#endif

using U64 = std::uint64_t;

inline constexpr std::string_view NAME = "Chess Forever";
inline constexpr std::string_view VERSION = "0.1.0";
inline constexpr std::size_t BRD_SQ_NUM = 120;
inline constexpr std::size_t MAXGAMEMOVES = 2048;

enum class Piece : int {
  EMPTY,
  wP,
  wN,
  wB,
  wR,
  wQ,
  wK,
  bP,
  bN,
  bB,
  bR,
  bQ,
  bK
};

enum class File : int { A, B, C, D, E, F, G, H, NONE };

enum class Rank : int { R1, R2, R3, R4, R5, R6, R7, R8, NONE };

enum class Side : int { WHITE, BLACK, BOTH };

enum class Square : int {
  A1 = 21,
  B1,
  C1,
  D1,
  E1,
  F1,
  G1,
  H1,
  A2 = 31,
  B2,
  C2,
  D2,
  E2,
  F2,
  G2,
  H2,
  A3 = 41,
  B3,
  C3,
  D3,
  E3,
  F3,
  G3,
  H3,
  A4 = 51,
  B4,
  C4,
  D4,
  E4,
  F4,
  G4,
  H4,
  A5 = 61,
  B5,
  C5,
  D5,
  E5,
  F5,
  G5,
  H5,
  A6 = 71,
  B6,
  C6,
  D6,
  E6,
  F6,
  G6,
  H6,
  A7 = 81,
  B7,
  C7,
  D7,
  E7,
  F7,
  G7,
  H7,
  A8 = 91,
  B8,
  C8,
  D8,
  E8,
  F8,
  G8,
  H8,
  NO_SQ
};

enum class Bool : bool { FALSE = false, TRUE = true };

enum CastlePerm : int { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

struct S_UNDO {
  int move{};
  int castlePerm{};
  int enPas{};
  int fiftyMove{};
  U64 posKey{};
};

struct S_BOARD {
  std::array<int, BRD_SQ_NUM> pieces{};
  std::array<U64, 3> pawns{};
  std::array<int, 2> KingSq{};
  Side side{};
  int enPas{};
  int fiftyMove{};
  int ply{};
  int hisPly{};
  int castlePerm{};
  U64 posKey{};
  std::array<int, 13> pceNum{};
  std::array<int, 3> bigPce{};
  std::array<int, 3> majPce{};
  std::array<int, 3> minPce{};
  std::array<S_UNDO, MAXGAMEMOVES> history{};

  // Piece list
  int pList[13][10]{};
};

// Helper instead of macro
[[nodiscard]] constexpr int FR2SQ(int f, int r) noexcept {
  return 21 + f + (r * 10);
}

#define SQ64(sq120) Sq120ToSq64[sq120]
#define POP(b) PopBit(b)
#define CNT(b) CountBits(b)
#define CLRBIT(bb, sq) ((bb) &= ClearMask[(sq)])
#define SETBIT(bb, sq) ((bb) |= SetMask[(sq)])

namespace detail {

constexpr auto build_sq120_to_sq64() {
  std::array<int, BRD_SQ_NUM> mapping{};
  mapping.fill(65);

  std::size_t sq64 = 0;
  for (int rank = static_cast<int>(Rank::R1);
       rank <= static_cast<int>(Rank::R8); ++rank) {
    for (int file = static_cast<int>(File::A);
         file <= static_cast<int>(File::H); ++file) {
      const auto sq120 = FR2SQ(file, rank);
      mapping[static_cast<std::size_t>(sq120)] = static_cast<int>(sq64);
      ++sq64;
    }
  }

  return mapping;
}

constexpr auto build_sq64_to_sq120() {
  std::array<int, 64> mapping{};
  mapping.fill(120);

  std::size_t sq64 = 0;
  for (int rank = static_cast<int>(Rank::R1);
       rank <= static_cast<int>(Rank::R8); ++rank) {
    for (int file = static_cast<int>(File::A);
         file <= static_cast<int>(File::H); ++file) {
      mapping[sq64] = FR2SQ(file, rank);
      ++sq64;
    }
  }

  return mapping;
}

}  // namespace detail

inline constexpr std::array<int, BRD_SQ_NUM> Sq120ToSq64 =
    detail::build_sq120_to_sq64();
inline constexpr std::array<int, 64> Sq64ToSq120 =
    detail::build_sq64_to_sq120();

extern U64 SetMask[64];
extern U64 ClearMask[64];

extern void PrintBitboard(U64 bb);
extern int PopBit(U64* bb);
extern int CountBits(U64 b);

extern void AllInit();