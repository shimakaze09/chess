#pragma once

#include "chess/types.hpp"

namespace chess {

class Board;
class SearchInfo;

namespace xboard {

void loop(Board& board, SearchInfo& info) noexcept;

} // namespace xboard

namespace console {

void loop(Board& board, SearchInfo& info) noexcept;

} // namespace console

} // namespace chess
