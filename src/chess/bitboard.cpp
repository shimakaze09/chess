#include "chess/bitboard.hpp"

#include <bit>
#include <format>
#include <iostream>

#include "chess/internal/data.hpp"
#include "chess/internal/init.hpp"
#include "chess/types.hpp"

namespace chess::bitboard {

int popBit(Bitboard& bitboard) noexcept {
    // Use C++20 bit manipulation functions for better performance
    if (bitboard == 0) {
        return -1;
    }

    const int index = std::countr_zero(bitboard);
    bitboard &= (bitboard - 1); // Clear the lowest set bit
    return index;
}

int countBits(Bitboard bitboard) noexcept {
    // Use C++20 popcount for hardware-accelerated bit counting
    return std::popcount(bitboard);
}

void print(Bitboard bitboard) noexcept {
    std::cout << '\n';

    for (auto rank_idx = static_cast<int>(Rank::R8); rank_idx >= static_cast<int>(Rank::R1);
         --rank_idx) {
        for (auto file_idx = static_cast<int>(File::A); file_idx <= static_cast<int>(File::H);
             ++file_idx) {
            const auto square =
                squareFromFileRank(static_cast<File>(file_idx), static_cast<Rank>(rank_idx));
            const auto square_64 = internal::g_sq120ToSq64[static_cast<int>(square)];

            const char symbol = ((1ULL << square_64) & bitboard) ? 'X' : '-';
            std::cout << symbol;
        }
        std::cout << '\n';
    }
    std::cout << "\n\n" << std::flush;
}

} // namespace chess::bitboard
