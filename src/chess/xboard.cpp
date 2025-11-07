#include "chess/xboard.hpp"
#include "chess/board.hpp"
#include "chess/search_info.hpp"
#include "chess/search.hpp"
#include "chess/types.hpp"
#include <cstdio>
#include <cstring>

namespace chess::xboard {

void loop(Board& board, SearchInfo& info) noexcept {
    info.setGameMode(GameMode::XBoard);
    info.setPostThinking(true);
    setbuf(stdin, nullptr);
    setbuf(stdout, nullptr);
    printf("feature ping=1 setboard=1 colors=0 usermove=1 memory=1\n");
    printf("feature done=1\n");
}

} // namespace chess::xboard

namespace chess::console {

void loop(Board& board, SearchInfo& info) noexcept {
    info.setGameMode(GameMode::Console);
    info.setPostThinking(true);
    setbuf(stdin, nullptr);
    setbuf(stdout, nullptr);
    printf("Welcome to Vice In Console Mode!\n");
    printf("Type help for commands\n\n");
}

} // namespace chess::console

