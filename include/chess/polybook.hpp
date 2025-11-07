#pragma once

namespace chess {

class Board;

namespace polybook {

void init() noexcept;
void clean() noexcept;
int getBookMove(Board& board) noexcept;

} // namespace polybook

} // namespace chess

