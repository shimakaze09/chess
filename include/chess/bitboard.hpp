#pragma once

#include <array>
#include <cstdint>

#include "chess/types.hpp"


namespace chess::bitboard {

inline constexpr std::array<int, 64> kBitTable = {
    63, 30, 3,  32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,  51, 21, 43,
    45, 10, 18, 47, 1,  54, 9,  57, 0,  35, 62, 31, 40, 4,  49, 5,  52, 26, 60, 6,  23, 44,
    46, 27, 56, 16, 7,  39, 48, 24, 59, 14, 12, 55, 38, 28, 58, 20, 37, 17, 36, 8};

void print(Bitboard bitboard) noexcept;
int popBit(Bitboard& bitboard) noexcept;
int countBits(Bitboard bitboard) noexcept;

inline void clearBit(Bitboard& bb, int sq) noexcept {
    bb &= ~(1ULL << sq);
}

inline void setBit(Bitboard& bb, int sq) noexcept {
    bb |= (1ULL << sq);
}

} // namespace chess::bitboard
