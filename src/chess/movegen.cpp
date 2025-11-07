#include "chess/movegen.hpp"
#include "chess/board.hpp"
#include "chess/move.hpp"

namespace chess::movegen {

void generateAllMoves(const Board& board, MoveList& list) noexcept {
    list.clear();
}

void generateAllCaptures(const Board& board, MoveList& list) noexcept {
    list.clear();
}

bool moveExists(Board& board, Move move) noexcept {
    return false;
}

void initMvvLva() noexcept {
}

} // namespace chess::movegen

