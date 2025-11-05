#pragma once

#include "pch.hpp"

using U64 = std::uint64_t;

inline constexpr std::string_view NAME = "Chess Forever";
inline constexpr std::string_view VERSION = "0.1.0";
inline constexpr int BRD_SQ_NUM = 120;
inline constexpr int MAXGAMEMOVES = 2048;

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
  int side{};
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
};