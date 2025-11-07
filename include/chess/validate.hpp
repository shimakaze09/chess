#pragma once

#include "chess/types.hpp"

namespace chess {

class Board;
class MoveList;

namespace validate {

bool squareOnBoard(Square sq) noexcept;
bool sideValid(Color side) noexcept;
bool fileRankValid(int fr) noexcept;
bool pieceValidEmpty(Piece pce) noexcept;
bool pieceValid(Piece pce) noexcept;
bool pieceValidEmptyOffbrd(Piece pce) noexcept;
bool squareIs120(int sq) noexcept;
bool moveListOk(const MoveList& list, const Board& board) noexcept;

} // namespace validate

} // namespace chess

