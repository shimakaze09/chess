#include <algorithm>
#include <iostream>
#include <span>
#include <string>
#include <string_view>

#include "chess/board.hpp"
#include "chess/internal/init.hpp"
#include "chess/search_info.hpp"
#include "chess/uci.hpp"
#include "chess/xboard.hpp"

namespace {
constexpr int kDefaultHashSize = 64;

void ProcessCommandLineArgs(std::span<const char* const> args) {
    auto contains_no_book = [](std::string_view arg) { return arg == "NoBook"; };

    if (std::ranges::any_of(args, contains_no_book)) {
        chess::g_engineOptions.setUseBook(false);
        std::cout << "Book Off\n";
    }
}

enum class CommandType : std::uint8_t { kUci, kXBoard, kVice, kQuit, kUnknown };

constexpr CommandType ParseCommand(std::string_view line) {
    if (line.starts_with("uci")) {
        return CommandType::kUci;
    }
    if (line.starts_with("xboard")) {
        return CommandType::kXBoard;
    }
    if (line.starts_with("vice")) {
        return CommandType::kVice;
    }
    if (line.starts_with("quit")) {
        return CommandType::kQuit;
    }
    return CommandType::kUnknown;
}
} // namespace

int main(int argc, char* argv[]) {
    try {
        chess::internal::initializeAll();

        chess::Board board;
        chess::SearchInfo info;
        info.setQuit(false);
        board.hashTable().init(kDefaultHashSize);

        // Synchronize C++ streams with C stdio for better performance
        std::ios::sync_with_stdio(false);
        std::cin.tie(nullptr);

        // Process command line arguments
        ProcessCommandLineArgs(std::span{argv, static_cast<std::size_t>(argc)});

        std::cout << "Welcome!\n" << std::flush;

        std::string line;
        while (true) {
            if (!std::getline(std::cin, line)) {
                continue;
            }

            if (line.empty()) {
                continue;
            }

            const auto kCommand = ParseCommand(line);

            switch (kCommand) {
                case CommandType::kUci:
                    chess::uci::loop(board, info);
                    if (info.quit()) {
                        return 0;
                    }
                    break;

                case CommandType::kXBoard:
                    chess::xboard::loop(board, info);
                    if (info.quit()) {
                        return 0;
                    }
                    break;

                case CommandType::kVice:
                    chess::console::loop(board, info);
                    if (info.quit()) {
                        return 0;
                    }
                    break;

                case CommandType::kQuit:
                    return 0;

                case CommandType::kUnknown:
                    // Ignore unknown commands
                    break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown error occurred\n";
        return 1;
    }
}
