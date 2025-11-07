#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>


namespace chess {

using Bitboard = std::uint64_t;

inline constexpr int kMaxHash = 1024;
inline constexpr int kBoardSquareCount = 120;
inline constexpr int kMaxGameMoves = 2048;
inline constexpr int kMaxPositionMoves = 256;
inline constexpr int kMaxDepth = 64;
inline constexpr int kInfinite = 30000;
inline constexpr int kIsMate = kInfinite - kMaxDepth;
inline constexpr int kNoMove = 0;

inline constexpr const char* kName = "Chess Forever";
inline constexpr const char* kVersion = "0.1.0";
inline constexpr const char* kStartFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

enum class Piece : int {
    Empty = 0,
    WhitePawn = 1,
    WhiteKnight = 2,
    WhiteBishop = 3,
    WhiteRook = 4,
    WhiteQueen = 5,
    WhiteKing = 6,
    BlackPawn = 7,
    BlackKnight = 8,
    BlackBishop = 9,
    BlackRook = 10,
    BlackQueen = 11,
    BlackKing = 12
};

enum class File : int { A = 0, B = 1, C = 2, D = 3, E = 4, F = 5, G = 6, H = 7, None = 8 };

enum class Rank : int { R1 = 0, R2 = 1, R3 = 2, R4 = 3, R5 = 4, R6 = 5, R7 = 6, R8 = 7, None = 8 };

enum class Color : int { White = 0, Black = 1, Both = 2 };

enum class GameMode : int { Uci = 0, XBoard = 1, Console = 2 };

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
    NoSquare = 99,
    OffBoard = 100
};

enum class CastleRights : std::uint8_t {
    None = 0,
    WhiteKingside = 1,
    WhiteQueenside = 2,
    BlackKingside = 4,
    BlackQueenside = 8
};

inline constexpr CastleRights operator|(CastleRights a, CastleRights b) {
    return static_cast<CastleRights>(static_cast<std::uint8_t>(a) | static_cast<std::uint8_t>(b));
}

inline constexpr CastleRights operator&(CastleRights a, CastleRights b) {
    return static_cast<CastleRights>(static_cast<std::uint8_t>(a) & static_cast<std::uint8_t>(b));
}

inline constexpr CastleRights& operator|=(CastleRights& a, CastleRights b) {
    a = a | b;
    return a;
}

inline constexpr CastleRights& operator&=(CastleRights& a, CastleRights b) {
    a = a & b;
    return a;
}

enum class HashFlag : int { None = 0, Alpha = 1, Beta = 2, Exact = 3 };

inline constexpr int kMoveFlagEnPassant = 0x40000;
inline constexpr int kMoveFlagPawnStart = 0x80000;
inline constexpr int kMoveFlagCastle = 0x1000000;
inline constexpr int kMoveFlagCapture = 0x7C000;
inline constexpr int kMoveFlagPromotion = 0xF00000;

[[nodiscard]] constexpr int fromSquare(int move) noexcept {
    return move & 0x7F;
}

[[nodiscard]] constexpr int toSquare(int move) noexcept {
    return (move >> 7) & 0x7F;
}

[[nodiscard]] constexpr int capturedPiece(int move) noexcept {
    return (move >> 14) & 0xF;
}

[[nodiscard]] constexpr int promotedPiece(int move) noexcept {
    return (move >> 20) & 0xF;
}

[[nodiscard]] constexpr Square squareFromFileRank(File f, Rank r) noexcept {
    return static_cast<Square>(21 + static_cast<int>(f) + static_cast<int>(r) * 10);
}

// C++20 convenience type aliases
template <typename T>
concept ChessPieceType = std::same_as<T, Piece>;

template <typename T>
concept ChessSquareType = std::same_as<T, Square>;

template <typename T>
concept ChessColorType = std::same_as<T, Color>;

// Type-safe factory functions
[[nodiscard]] consteval auto makePiece(int value) noexcept -> Piece {
    return static_cast<Piece>(value);
}

[[nodiscard]] consteval auto makeSquare(int value) noexcept -> Square {
    return static_cast<Square>(value);
}

[[nodiscard]] consteval auto makeColor(int value) noexcept -> Color {
    return static_cast<Color>(value);
}

} // namespace chess
