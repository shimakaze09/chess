#include "chess/hash.hpp"

#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>
#include <span>

#include "chess/board.hpp"
#include "chess/internal/data.hpp"
#include "chess/types.hpp"

namespace chess::hash {

std::uint64_t generatePositionKey(const Board& board) noexcept {
    std::uint64_t final_key = 0;

    // Use ranges to iterate over board squares
    for (const auto square_idx : std::views::iota(0, kBoardSquareCount)) {
        const auto current_piece = board.pieceAt(static_cast<Square>(square_idx));
        if (current_piece != Piece::Empty &&
            static_cast<int>(current_piece) != static_cast<int>(Square::OffBoard)) {
            final_key ^= internal::g_pieceKeys[static_cast<int>(current_piece)][square_idx];
        }
    }

    if (board.side() == Color::White) {
        final_key ^= internal::g_sideKey;
    }

    if (board.enPas() != Square::NoSquare) {
        final_key ^=
            internal::g_pieceKeys[static_cast<int>(Piece::Empty)][static_cast<int>(board.enPas())];
    }

    final_key ^= internal::g_castleKeys[board.castlePerm()];

    return final_key;
}

} // namespace chess::hash

namespace chess {

void HashTable::init(int mb) {
    constexpr int kMegabyte = 0x100000;
    constexpr int kHashTablePadding = 2;

    const int hash_size = kMegabyte * mb;
    numEntries_ = hash_size / sizeof(HashEntry);
    numEntries_ -= kHashTablePadding;

    pTable_ = std::make_unique<HashEntry[]>(numEntries_);
    if (pTable_) {
        clear();
        std::cout << std::format("HashTable init complete with {} entries\n", numEntries_);
    } else {
        std::cout << std::format("Hash Allocation Failed, trying {}MB...\n", mb / 2);
        init(mb / 2);
    }
}

void HashTable::clear() noexcept {
    if (!pTable_) {
        return;
    }

    // Use ranges to clear hash table entries
    std::ranges::for_each(std::span{pTable_.get(), static_cast<std::size_t>(numEntries_)},
                          [](HashEntry& entry) {
                              entry.setPosKey(0);
                              entry.setMove(kNoMove);
                              entry.setDepth(0);
                              entry.setScore(0);
                              entry.setFlags(HashFlag::None);
                          });

    newWrite_ = 0;
}

} // namespace chess
