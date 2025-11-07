#include "chess/bitboard.hpp"
#include "chess/internal/data.hpp"
#include "chess/internal/init.hpp"
#include "chess/types.hpp"
#include <cstdio>

namespace chess::bitboard {

int popBit(Bitboard& bb) noexcept {
    Bitboard b = bb ^ (bb - 1);
    unsigned int fold = static_cast<unsigned>((b & 0xffffffff) ^ (b >> 32));
    bb &= (bb - 1);
    return kBitTable[(fold * 0x783a9b23) >> 26];
}

int countBits(Bitboard b) noexcept {
    int r = 0;
    for (; b; r++, b &= b - 1);
    return r;
}

void print(Bitboard bb) noexcept {
    printf("\n");
    for (int rank = static_cast<int>(Rank::R8); rank >= static_cast<int>(Rank::R1); --rank) {
        for (int file = static_cast<int>(File::A); file <= static_cast<int>(File::H); ++file) {
            Square sq = squareFromFileRank(static_cast<File>(file), static_cast<Rank>(rank));
            int sq64 = internal::g_sq120ToSq64[static_cast<int>(sq)];
            if ((1ULL << sq64) & bb)
                printf("X");
            else
                printf("-");
        }
        printf("\n");
    }
    printf("\n\n");
}

} // namespace chess::bitboard
