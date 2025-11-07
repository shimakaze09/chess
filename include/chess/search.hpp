#pragma once

#include "chess/types.hpp"

namespace chess {

class Board;
class SearchInfo;

namespace search {

void searchPosition(Board& board, SearchInfo& info) noexcept;

} // namespace search

} // namespace chess

