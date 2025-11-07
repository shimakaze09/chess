#include "chess/uci.hpp"

#include <format>
#include <iostream>
#include <string>
#include <string_view>

#include "chess/board.hpp"
#include "chess/io.hpp"
#include "chess/search.hpp"
#include "chess/search_info.hpp"
#include "chess/types.hpp"

namespace chess::uci {

namespace {
constexpr std::string_view kAuthor = "Bluefever";
constexpr int kDefaultHashSize = 64;
constexpr int kMinHashSize = 4;

void PrintUciInfo() {
    std::cout << std::format("id name {}\n", kName);
    std::cout << std::format("id author {}\n", kAuthor);
    std::cout << std::format("option name Hash type spin default {} min {} max {}\n",
                             kDefaultHashSize, kMinHashSize, kMaxHash);
    std::cout << "option name Book type check default true\n";
    std::cout << "uciok\n" << std::flush;
}

enum class UciCommand : std::uint8_t {
    kIsReady,
    kPosition,
    kUciNewGame,
    kGo,
    kQuit,
    kUci,
    kUnknown
};

constexpr UciCommand ParseUciCommand(std::string_view line) {
    if (line.starts_with("isready")) {
        return UciCommand::kIsReady;
    }
    if (line.starts_with("position")) {
        return UciCommand::kPosition;
    }
    if (line.starts_with("ucinewgame")) {
        return UciCommand::kUciNewGame;
    }
    if (line.starts_with("go")) {
        return UciCommand::kGo;
    }
    if (line.starts_with("quit")) {
        return UciCommand::kQuit;
    }
    if (line.starts_with("uci")) {
        return UciCommand::kUci;
    }
    return UciCommand::kUnknown;
}
} // namespace

void loop(Board& board, SearchInfo& info) noexcept {
    info.setGameMode(GameMode::Uci);

    // Synchronize streams for better performance in UCI mode
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    PrintUciInfo();

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        const auto kCommand = ParseUciCommand(line);

        switch (kCommand) {
            case UciCommand::kIsReady:
                std::cout << "readyok\n" << std::flush;
                break;

            case UciCommand::kPosition:
                // TODO: Handle position command
                break;

            case UciCommand::kUciNewGame:
                // TODO: Handle new game
                break;

            case UciCommand::kGo:
                std::cout << "Seen Go..\n" << std::flush;
                search::searchPosition(board, info);
                break;

            case UciCommand::kQuit:
                info.setQuit(true);
                return;

            case UciCommand::kUci:
                PrintUciInfo();
                break;

            case UciCommand::kUnknown:
                // Ignore unknown commands in UCI mode
                break;
        }
    }
}

} // namespace chess::uci
