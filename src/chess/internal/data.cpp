#include "chess/internal/data.hpp"
#include <array>

namespace chess::internal {

std::array<int, kBoardSquareCount> g_sq120ToSq64{};
std::array<int, 64> g_sq64ToSq120{};
std::array<Bitboard, 64> g_setMask{};
std::array<Bitboard, 64> g_clearMask{};
std::array<std::array<Bitboard, 120>, 13> g_pieceKeys{};
Bitboard g_sideKey = 0;
std::array<Bitboard, 16> g_castleKeys{};
std::array<int, kBoardSquareCount> g_filesBrd{};
std::array<int, kBoardSquareCount> g_ranksBrd{};
std::array<Bitboard, 8> g_fileBBMask{};
std::array<Bitboard, 8> g_rankBBMask{};
std::array<Bitboard, 64> g_blackPassedMask{};
std::array<Bitboard, 64> g_whitePassedMask{};
std::array<Bitboard, 64> g_isolatedMask{};

} // namespace chess::internal
