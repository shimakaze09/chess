#include "chess/movegen.hpp"

#include "chess/board.hpp"
#include "chess/move.hpp"

namespace chess::movegen {

void generateAllMoves(const Board& board, MoveList& list) noexcept {
    static_cast<void>(board);
    list.clear();
}

void generateAllCaptures(const Board& board, MoveList& list) noexcept {
    static_cast<void>(board);
    list.clear();
}

bool MoveExists(Board& board, const Move& move) noexcept {
    static_cast<void>(board);
    static_cast<void>(move);
    return false;
}

void initMvvLva() noexcept {}

} // namespace chess::movegen
