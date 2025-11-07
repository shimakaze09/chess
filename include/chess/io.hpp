#pragma once

#include "chess/types.hpp"
#include "chess/move.hpp"
#include <string>
#include <string_view>
#include <optional>

namespace chess {

class Board;
class MoveList;

namespace io {

std::string printSquare(Square sq) noexcept;
std::string printMove(Move move) noexcept;
void printMoveList(const MoveList& list) noexcept;
std::optional<Move> parseMove(std::string_view str, const Board& board) noexcept;

} // namespace io

} // namespace chess

