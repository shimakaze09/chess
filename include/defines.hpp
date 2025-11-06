#pragma once

#include "pch.hpp"

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

constexpr auto build_set_mask() {
  std::array<U64, 64> masks{};
  for (std::size_t index = 0; index < masks.size(); ++index) {
    masks[index] = U64{1} << index;
  }
  return masks;
}

constexpr auto build_clear_mask() {
  std::array<U64, 64> masks{};
  for (std::size_t index = 0; index < masks.size(); ++index) {
    masks[index] = ~(U64{1} << index);
  }
  return masks;
}

}  // namespace detail

inline constexpr std::array<int, BRD_SQ_NUM> Sq120ToSq64 =
    detail::build_sq120_to_sq64();
inline constexpr std::array<int, 64> Sq64ToSq120 =
    detail::build_sq64_to_sq120();
inline constexpr std::array<U64, 64> SetMask = detail::build_set_mask();
inline constexpr std::array<U64, 64> ClearMask = detail::build_clear_mask();

[[nodiscard]] constexpr int Sq120ToSq64Index(int square120) noexcept {
  return Sq120ToSq64[static_cast<std::size_t>(square120)];
}

[[nodiscard]] constexpr int Sq64ToSq120Index(int square64) noexcept {
  return Sq64ToSq120[static_cast<std::size_t>(square64)];
}

inline constexpr void SetBit(U64& bitboard, std::size_t square) noexcept {
  bitboard |= SetMask[square];
}

inline constexpr void ClearBit(U64& bitboard, std::size_t square) noexcept {
  bitboard &= ClearMask[square];
}

[[nodiscard]] inline bool IsBitSet(U64 bitboard, std::size_t square) noexcept {
  return (bitboard & SetMask[square]) != 0ULL;
}

[[nodiscard]] int PopBit(U64& bitboard);
[[nodiscard]] int CountBits(U64 bitboard);
void PrintBitboard(U64 bitboard);
