#include "chess/hash.hpp"
#include "chess/board.hpp"
#include "chess/internal/data.hpp"
#include "chess/types.hpp"
#include <cstdio>
#include <cstdlib>

namespace chess::hash {

std::uint64_t generatePositionKey(const Board& board) noexcept {
    std::uint64_t finalKey = 0;

    for (int sq = 0; sq < kBoardSquareCount; ++sq) {
        Piece piece = board.pieceAt(static_cast<Square>(sq));
        if (piece != Piece::Empty && piece != Piece::Empty && static_cast<int>(piece) != static_cast<int>(Square::OffBoard)) {
            finalKey ^= internal::g_pieceKeys[static_cast<int>(piece)][sq];
        }
    }

    if (board.side() == Color::White) {
        finalKey ^= internal::g_sideKey;
    }

    if (board.enPas() != Square::NoSquare) {
        finalKey ^= internal::g_pieceKeys[static_cast<int>(Piece::Empty)][static_cast<int>(board.enPas())];
    }

    finalKey ^= internal::g_castleKeys[board.castlePerm()];

    return finalKey;
}

} // namespace chess::hash

namespace chess {

void HashTable::init(int mb) {
    int hashSize = 0x100000 * mb;
    numEntries_ = hashSize / sizeof(HashEntry);
    numEntries_ -= 2;

    pTable_ = std::make_unique<HashEntry[]>(numEntries_);
    if (pTable_) {
        clear();
        printf("HashTable init complete with %d entries\n", numEntries_);
    } else {
        printf("Hash Allocation Failed, trying %dMB...\n", mb / 2);
        init(mb / 2);
    }
}

void HashTable::clear() noexcept {
    for (int i = 0; i < numEntries_; ++i) {
        pTable_[i].setPosKey(0);
        pTable_[i].setMove(kNoMove);
        pTable_[i].setDepth(0);
        pTable_[i].setScore(0);
        pTable_[i].setFlags(HashFlag::None);
    }
    newWrite_ = 0;
}

} // namespace chess

