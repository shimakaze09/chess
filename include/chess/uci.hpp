#pragma once

#include "chess/types.hpp"

namespace chess {

class Board;
class SearchInfo;

namespace uci {

void loop(Board& board, SearchInfo& info) noexcept;

} // namespace uci

} // namespace chess

