#include "chess/search.hpp"

#include <cstdio>

#include "chess/board.hpp"
#include "chess/hash.hpp"
#include "chess/misc.hpp"
#include "chess/move.hpp"
#include "chess/movegen.hpp"
#include "chess/search_info.hpp"
#include "chess/types.hpp"

namespace chess::search {

namespace {
[[maybe_unused]] void checkUp(SearchInfo& info) noexcept {
    if (info.timeSet() && misc::getTimeMs() > info.stopTime()) {
        info.setStopped(true);
    }
    misc::readInput(info);
}

[[maybe_unused]] void pickNextMove(int moveNum, MoveList& list) noexcept {
    if (moveNum >= list.size()) return;

    int bestScore = list[moveNum].score();
    int bestNum = moveNum;

    for (int index = moveNum + 1; index < list.size(); ++index) {
        if (list[index].score() > bestScore) {
            bestScore = list[index].score();
            bestNum = index;
        }
    }

    if (bestNum != moveNum) {
        Move temp = list[moveNum];
        list[moveNum] = list[bestNum];
        list[bestNum] = temp;
    }
}

[[maybe_unused]] bool isRepetition(const Board& board) noexcept {
    for (int index = board.hisPly() - board.fiftyMove(); index < board.hisPly() - 1; ++index) {
        if (index >= 0 && index < kMaxGameMoves) {
            if (board.posKey() == board.history(index).posKey()) {
                return true;
            }
        }
    }
    return false;
}

void clearForSearch(Board& board, SearchInfo& info) noexcept {
    for (int index = 0; index < 13; ++index) {
        for (int index2 = 0; index2 < kBoardSquareCount; ++index2) {
            board.searchHistory(static_cast<Piece>(index), static_cast<Square>(index2)) = 0;
        }
    }

    for (int index = 0; index < 2; ++index) {
        for (int index2 = 0; index2 < kMaxDepth; ++index2) {
            board.searchKiller(static_cast<Color>(index), index2) = 0;
        }
    }

    board.hashTable().clear();
    board.setPly(0);
    info.setStopped(false);
    info.setNodes(0);
    info.setFh(0.0f);
    info.setFhf(0.0f);
}
} // namespace

void searchPosition(Board& board, SearchInfo& info) noexcept {
    clearForSearch(board, info);
}

} // namespace chess::search
