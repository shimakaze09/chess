#pragma once

#include <concepts>
#include <type_traits>

#include "chess/types.hpp"


namespace chess::concepts {

// Concept for valid chess pieces
template <typename T>
concept ChessPiece = std::same_as<T, Piece> && requires(T piece) {
    static_cast<int>(piece) >= static_cast<int>(Piece::Empty) &&
        static_cast<int>(piece) <= static_cast<int>(Piece::BlackKing);
};

// Concept for valid board squares
template <typename T>
concept BoardSquare = std::same_as<T, Square> && requires(T square) {
    static_cast<int>(square) >= 0 && static_cast<int>(square) < kBoardSquareCount;
};

// Concept for valid chess colors
template <typename T>
concept ChessColor = std::same_as<T, Color> && requires(T color) {
    static_cast<int>(color) >= static_cast<int>(Color::White) &&
        static_cast<int>(color) <= static_cast<int>(Color::Both);
};

// Concept for valid files
template <typename T>
concept ChessFile = std::same_as<T, File> && requires(T file) {
    static_cast<int>(file) >= static_cast<int>(File::A) &&
        static_cast<int>(file) <= static_cast<int>(File::None);
};

// Concept for valid ranks
template <typename T>
concept ChessRank = std::same_as<T, Rank> && requires(T rank) {
    static_cast<int>(rank) >= static_cast<int>(Rank::R1) &&
        static_cast<int>(rank) <= static_cast<int>(Rank::None);
};

// Concept for bitboards
template <typename T>
concept BitboardType = std::same_as<T, Bitboard> && std::unsigned_integral<T> && sizeof(T) == 8;

// Concept for move representation
template <typename T>
concept MoveType = requires(T move) {
    { move.value() } -> std::convertible_to<int>;
    { move.from() } -> BoardSquare;
    { move.to() } -> BoardSquare;
    { move.captured() } -> ChessPiece;
    { move.promoted() } -> ChessPiece;
    { move.isCapture() } -> std::convertible_to<bool>;
    { move.isPromotion() } -> std::convertible_to<bool>;
    { move.isCastle() } -> std::convertible_to<bool>;
    { move.isEnPassant() } -> std::convertible_to<bool>;
};

// Concept for board-like objects
template <typename T>
concept BoardLike = requires(T board, Square sq, Color color, Piece piece) {
    { board.pieceAt(sq) } -> ChessPiece;
    { board.side() } -> ChessColor;
    { board.enPas() } -> BoardSquare;
    { board.castlePerm() } -> std::convertible_to<int>;
    { board.posKey() } -> std::convertible_to<std::uint64_t>;
    { board.isSquareAttacked(sq, color) } -> std::convertible_to<bool>;
};

} // namespace chess::concepts