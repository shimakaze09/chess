#include "chess/misc.hpp"
#include "chess/search_info.hpp"
#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#endif

namespace chess::misc {

int getTimeMs() noexcept {
#ifdef _WIN32
    return static_cast<int>(GetTickCount());
#else
    struct timeval t;
    gettimeofday(&t, nullptr);
    return static_cast<int>(t.tv_sec * 1000 + t.tv_usec / 1000);
#endif
}

int inputWaiting() noexcept {
#ifndef _WIN32
    fd_set readfds;
    struct timeval tv;
    FD_ZERO(&readfds);
    FD_SET(fileno(stdin), &readfds);
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    select(16, &readfds, nullptr, nullptr, &tv);
    return (FD_ISSET(fileno(stdin), &readfds));
#else
    static int init = 0;
    static HANDLE inh;
    DWORD dw;

    if (!init) {
        init = 1;
        inh = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        if (!GetConsoleMode(inh, &mode)) {
            DWORD available;
            if (PeekNamedPipe(inh, nullptr, 0, nullptr, &available, nullptr)) {
                return available;
            }
            return 1;
        }
        SetConsoleMode(inh, mode & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
        FlushConsoleInputBuffer(inh);
    }
    DWORD available;
    if (PeekNamedPipe(inh, nullptr, 0, nullptr, &available, nullptr)) {
        return available;
    }
    GetNumberOfConsoleInputEvents(inh, &dw);
    return dw <= 1 ? 0 : static_cast<int>(dw);
#endif
}

void readInput(SearchInfo& info) noexcept {
    if (inputWaiting()) {
        info.setStopped(true);
        char input[256] = "";
#ifdef _WIN32
        DWORD bytesRead;
        ReadFile(GetStdHandle(STD_INPUT_HANDLE), input, 256, &bytesRead, nullptr);
#else
        ssize_t bytes = read(fileno(stdin), input, 256);
        (void)bytes;
#endif
        char* endc = strchr(input, '\n');
        if (endc) *endc = 0;

        if (strlen(input) > 0) {
            if (strncmp(input, "quit", 4) == 0) {
                info.setQuit(true);
            }
        }
    }
}

} // namespace chess::misc

