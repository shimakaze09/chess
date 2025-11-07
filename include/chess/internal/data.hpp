#pragma once

#include "chess/types.hpp"
#include <array>
#include <cstdint>

namespace chess::internal {

inline constexpr std::array<char, 13> kPieceChar = {'.', 'P', 'N', 'B', 'R', 'Q', 'K', 'p', 'n', 'b', 'r', 'q', 'k'};
inline constexpr std::array<char, 3> kSideChar = {'w', 'b', '-'};
inline constexpr std::array<char, 8> kRankChar = {'1', '2', '3', '4', '5', '6', '7', '8'};
inline constexpr std::array<char, 8> kFileChar = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

inline constexpr std::array<int, 13> kPieceBig = {0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1};
inline constexpr std::array<int, 13> kPieceMaj = {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1};
inline constexpr std::array<int, 13> kPieceMin = {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0};
inline constexpr std::array<int, 13> kPieceVal = {0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000};
inline constexpr std::array<int, 13> kPieceCol = {2, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
inline constexpr std::array<int, 13> kPiecePawn = {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
inline constexpr std::array<int, 13> kPieceKnight = {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
inline constexpr std::array<int, 13> kPieceKing = {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
inline constexpr std::array<int, 13> kPieceRookQueen = {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0};
inline constexpr std::array<int, 13> kPieceBishopQueen = {0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0};
inline constexpr std::array<int, 13> kPieceSlides = {0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0};

inline constexpr std::array<int, 64> kMirror64 = {
    56, 57, 58, 59, 60, 61, 62, 63, 48, 49, 50, 51, 52, 53, 54, 55,
    40, 41, 42, 43, 44, 45, 46, 47, 32, 33, 34, 35, 36, 37, 38, 39,
    24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23,
    8,  9,  10, 11, 12, 13, 14, 15, 0,  1,  2,  3,  4,  5,  6,  7
};

void initSq120To64() noexcept;
void initBitMasks() noexcept;
void initFilesRanksBrd() noexcept;
void initEvalMasks() noexcept;

extern std::array<int, kBoardSquareCount> g_sq120ToSq64;
extern std::array<int, 64> g_sq64ToSq120;
extern std::array<Bitboard, 64> g_setMask;
extern std::array<Bitboard, 64> g_clearMask;
extern std::array<std::array<Bitboard, 120>, 13> g_pieceKeys;
extern Bitboard g_sideKey;
extern std::array<Bitboard, 16> g_castleKeys;
extern std::array<int, kBoardSquareCount> g_filesBrd;
extern std::array<int, kBoardSquareCount> g_ranksBrd;
extern std::array<Bitboard, 8> g_fileBBMask;
extern std::array<Bitboard, 8> g_rankBBMask;
extern std::array<Bitboard, 64> g_blackPassedMask;
extern std::array<Bitboard, 64> g_whitePassedMask;
extern std::array<Bitboard, 64> g_isolatedMask;

[[nodiscard]] inline constexpr bool isBishopQueen(Piece p) noexcept {
    return kPieceBishopQueen[static_cast<int>(p)] != 0;
}

[[nodiscard]] inline constexpr bool isRookQueen(Piece p) noexcept {
    return kPieceRookQueen[static_cast<int>(p)] != 0;
}

[[nodiscard]] inline constexpr bool isKnight(Piece p) noexcept {
    return kPieceKnight[static_cast<int>(p)] != 0;
}

[[nodiscard]] inline constexpr bool isKing(Piece p) noexcept {
    return kPieceKing[static_cast<int>(p)] != 0;
}

[[nodiscard]] inline int squareTo64(Square sq120) noexcept {
    return g_sq120ToSq64[static_cast<int>(sq120)];
}

[[nodiscard]] inline Square squareTo120(int sq64) noexcept {
    return static_cast<Square>(g_sq64ToSq120[sq64]);
}

} // namespace chess::internal
