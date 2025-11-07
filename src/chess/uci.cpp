#include "chess/uci.hpp"
#include "chess/board.hpp"
#include "chess/search_info.hpp"
#include "chess/search.hpp"
#include "chess/io.hpp"
#include "chess/types.hpp"
#include <cstdio>
#include <cstring>

namespace chess::uci {

inline constexpr int kInputBuffer = 400 * 6;

void loop(Board& board, SearchInfo& info) noexcept {
    info.setGameMode(GameMode::Uci);
    setbuf(stdin, nullptr);
    setbuf(stdout, nullptr);

    char line[kInputBuffer];
    printf("id name %s\n", kName);
    printf("id author Bluefever\n");
    printf("option name Hash type spin default 64 min 4 max %d\n", kMaxHash);
    printf("option name Book type check default true\n");
    printf("uciok\n");

    while (true) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if (!fgets(line, kInputBuffer, stdin))
            continue;

        if (line[0] == '\n')
            continue;

        if (strncmp(line, "isready", 7) == 0) {
            printf("readyok\n");
            continue;
        } else if (strncmp(line, "position", 8) == 0) {
        } else if (strncmp(line, "ucinewgame", 10) == 0) {
        } else if (strncmp(line, "go", 2) == 0) {
            printf("Seen Go..\n");
            search::searchPosition(board, info);
        } else if (strncmp(line, "quit", 4) == 0) {
            info.setQuit(true);
            break;
        } else if (strncmp(line, "uci", 3) == 0) {
            printf("id name %s\n", kName);
            printf("id author Bluefever\n");
            printf("uciok\n");
        }
    }
}

} // namespace chess::uci

