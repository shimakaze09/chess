#include "chess/misc.hpp"

#include <cctype>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>

#include "chess/search_info.hpp"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>

#else
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#endif

namespace chess::misc {

int getTimeMs() noexcept {
    const auto now = std::chrono::steady_clock::now();
    const auto epoch = now.time_since_epoch();
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    return static_cast<int>(milliseconds.count());
}

namespace {
[[maybe_unused]] constexpr int kMaxSelectTimeout = 16;
constexpr std::string_view kQuitCommand = "quit";
} // namespace

int InputWaiting() noexcept {
#ifdef _WIN32
    return _kbhit();
#else
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(fileno(stdin), &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    const int result = select(kMaxSelectTimeout, &readfds, nullptr, nullptr, &tv);
    return result > 0 && FD_ISSET(fileno(stdin), &readfds) ? 1 : 0;
#endif
}

void readInput(SearchInfo& info) noexcept {
    if (!InputWaiting()) {
        return;
    }

    info.setStopped(true);

    try {
        std::string input;
        if (std::getline(std::cin, input)) {
            // Remove trailing whitespace
            while (!input.empty() && std::isspace(input.back())) {
                input.pop_back();
            }

            if (!input.empty() && input.starts_with(kQuitCommand)) {
                info.setQuit(true);
            }
        }
    } catch (...) {
        // Ignore input errors in search context
        info.setStopped(true);
    }
}

} // namespace chess::misc
