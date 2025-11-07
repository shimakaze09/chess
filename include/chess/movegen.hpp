#pragma once

#include "chess/types.hpp"

namespace chess {

class Board;
class MoveList;
class Move;

namespace movegen {

void generateAllMoves(const Board& board, MoveList& list) noexcept;
void generateAllCaptures(const Board& board, MoveList& list) noexcept;
bool MoveExists(Board& board, const Move& move) noexcept;
void initMvvLva() noexcept;

} // namespace movegen

} // namespace chess
