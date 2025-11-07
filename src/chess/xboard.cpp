#include "chess/xboard.hpp"

#include <iostream>

#include "chess/board.hpp"
#include "chess/search.hpp"
#include "chess/search_info.hpp"
#include "chess/types.hpp"

namespace chess::xboard {

void loop(Board& board, SearchInfo& info) noexcept {
    static_cast<void>(board); // Suppress unused parameter warning

    info.setGameMode(GameMode::XBoard);
    info.setPostThinking(true);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << "feature ping=1 setboard=1 colors=0 usermove=1 memory=1\n"
              << "feature done=1\n"
              << std::flush;
}

} // namespace chess::xboard

namespace chess::console {

void loop(Board& board, SearchInfo& info) noexcept {
    static_cast<void>(board); // Suppress unused parameter warning

    info.setGameMode(GameMode::Console);
    info.setPostThinking(true);

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cout << "Welcome to Vice In Console Mode!\n"
              << "Type help for commands\n\n"
              << std::flush;
}

} // namespace chess::console
