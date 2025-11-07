#include "chess/polybook.hpp"

#include "chess/board.hpp"
#include "chess/types.hpp"

namespace chess::polybook {

void init() noexcept {}

void clean() noexcept {}

int getBookMove(Board& board) noexcept {
    static_cast<void>(board);
    return kNoMove;
}

} // namespace chess::polybook
