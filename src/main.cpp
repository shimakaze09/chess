#include "chess/board.hpp"
#include "chess/search_info.hpp"
#include "chess/internal/init.hpp"
#include "chess/uci.hpp"
#include "chess/xboard.hpp"
#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]) {
    chess::internal::initializeAll();

    chess::Board board;
    chess::SearchInfo info;
    info.setQuit(false);
    board.hashTable().init(64);
    setbuf(stdin, nullptr);
    setbuf(stdout, nullptr);

    for (int argNum = 0; argNum < argc; ++argNum) {
        if (strncmp(argv[argNum], "NoBook", 6) == 0) {
            chess::g_engineOptions.setUseBook(false);
            printf("Book Off\n");
        }
    }

    printf("Welcome!\n");

    char line[256];
    while (true) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if (!fgets(line, 256, stdin)) continue;
        if (line[0] == '\n') continue;
        
        if (strncmp(line, "uci", 3) == 0) {
            chess::uci::loop(board, info);
            if (info.quit()) break;
            continue;
        } else if (strncmp(line, "xboard", 6) == 0) {
            chess::xboard::loop(board, info);
            if (info.quit()) break;
            continue;
        } else if (strncmp(line, "vice", 4) == 0) {
            chess::console::loop(board, info);
            if (info.quit()) break;
            continue;
        } else if (strncmp(line, "quit", 4) == 0) {
            break;
        }
    }

    return 0;
}
