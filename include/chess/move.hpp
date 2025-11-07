#pragma once

#include "chess/types.hpp"
#include <array>
#include <cstdint>

namespace chess {

class Move {
public:
    Move() noexcept : move_(kNoMove), score_(0) {}
    Move(int move, int score = 0) noexcept : move_(move), score_(score) {}

    [[nodiscard]] int value() const noexcept { return move_; }
    [[nodiscard]] int score() const noexcept { return score_; }
    [[nodiscard]] int& score() noexcept { return score_; }
    [[nodiscard]] Square from() const noexcept { return static_cast<Square>(fromSquare(move_)); }
    [[nodiscard]] Square to() const noexcept { return static_cast<Square>(toSquare(move_)); }
    [[nodiscard]] Piece captured() const noexcept { return static_cast<Piece>(capturedPiece(move_)); }
    [[nodiscard]] Piece promoted() const noexcept { return static_cast<Piece>(promotedPiece(move_)); }
    [[nodiscard]] bool isEnPassant() const noexcept { return (move_ & kMoveFlagEnPassant) != 0; }
    [[nodiscard]] bool isPawnStart() const noexcept { return (move_ & kMoveFlagPawnStart) != 0; }
    [[nodiscard]] bool isCastle() const noexcept { return (move_ & kMoveFlagCastle) != 0; }
    [[nodiscard]] bool isCapture() const noexcept { return (move_ & kMoveFlagCapture) != 0; }
    [[nodiscard]] bool isPromotion() const noexcept { return (move_ & kMoveFlagPromotion) != 0; }

    [[nodiscard]] bool operator==(const Move& other) const noexcept { return move_ == other.move_; }
    [[nodiscard]] bool operator!=(const Move& other) const noexcept { return move_ != other.move_; }

    static Move create(int from, int to, int captured, int promoted, int flags) noexcept {
        return Move(from | (to << 7) | (captured << 14) | (promoted << 20) | flags);
    }

private:
    int move_;
    int score_;
};

class MoveList {
public:
    MoveList() noexcept : count_(0) {}

    void clear() noexcept { count_ = 0; }
    [[nodiscard]] int size() const noexcept { return count_; }
    [[nodiscard]] bool empty() const noexcept { return count_ == 0; }

    void add(const Move& move) {
        if (count_ < kMaxPositionMoves) {
            moves_[count_] = move;
            ++count_;
        }
    }

    void add(int move, int score) {
        if (count_ < kMaxPositionMoves) {
            moves_[count_] = Move(move, score);
            ++count_;
        }
    }

    [[nodiscard]] const Move& operator[](int index) const noexcept { return moves_[index]; }
    [[nodiscard]] Move& operator[](int index) noexcept { return moves_[index]; }

    [[nodiscard]] const Move* begin() const noexcept { return moves_.data(); }
    [[nodiscard]] const Move* end() const noexcept { return moves_.data() + count_; }
    [[nodiscard]] Move* begin() noexcept { return moves_.data(); }
    [[nodiscard]] Move* end() noexcept { return moves_.data() + count_; }

private:
    std::array<Move, kMaxPositionMoves> moves_;
    int count_;
};

} // namespace chess

