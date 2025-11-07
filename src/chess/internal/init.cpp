#include "chess/internal/init.hpp"
#include "chess/internal/data.hpp"
#include "chess/types.hpp"
#include <cstdio>
#include <array>
#include "chess/movegen.hpp"
#include "chess/polybook.hpp"

namespace chess::internal {

namespace {
    constexpr std::uint64_t generateRandom64(std::uint64_t seed) noexcept {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;
        return seed;
    }

    constexpr std::array<std::uint64_t, 13 * 120 + 1 + 16> generateHashKeys() noexcept {
        std::array<std::uint64_t, 13 * 120 + 1 + 16> keys{};
        std::uint64_t seed = 0x123456789ABCDEF0ULL;
        for (std::size_t i = 0; i < keys.size(); ++i) {
            seed = generateRandom64(seed);
            keys[i] = seed;
        }
        return keys;
    }

    inline constexpr auto kHashKeys = generateHashKeys();
}

void initHashKeys() noexcept {
    std::size_t idx = 0;
    for (int index = 0; index < 13; ++index) {
        for (int index2 = 0; index2 < 120; ++index2) {
            g_pieceKeys[index][index2] = kHashKeys[idx++];
        }
    }
    g_sideKey = kHashKeys[idx++];
    for (int index = 0; index < 16; ++index) {
        g_castleKeys[index] = kHashKeys[idx++];
    }
}

void initBitMasks() noexcept {
    for (int index = 0; index < 64; index++) {
        g_setMask[index] = (1ULL << index);
        g_clearMask[index] = ~g_setMask[index];
    }
}

void initSq120To64() noexcept {
    for (int index = 0; index < kBoardSquareCount; ++index) {
        g_sq120ToSq64[index] = 65;
    }

    for (int index = 0; index < 64; ++index) {
        g_sq64ToSq120[index] = static_cast<int>(Square::OffBoard);
    }

    int sq64 = 0;
    for (int rank = static_cast<int>(Rank::R1); rank <= static_cast<int>(Rank::R8); ++rank) {
        for (int file = static_cast<int>(File::A); file <= static_cast<int>(File::H); ++file) {
            Square sq = squareFromFileRank(static_cast<File>(file), static_cast<Rank>(rank));
            g_sq64ToSq120[sq64] = static_cast<int>(sq);
            g_sq120ToSq64[static_cast<int>(sq)] = sq64;
            sq64++;
        }
    }
}

void initFilesRanksBrd() noexcept {
    for (int index = 0; index < kBoardSquareCount; ++index) {
        g_filesBrd[index] = static_cast<int>(File::None);
        g_ranksBrd[index] = static_cast<int>(Rank::None);
    }

    for (int rank = static_cast<int>(Rank::R1); rank <= static_cast<int>(Rank::R8); ++rank) {
        for (int file = static_cast<int>(File::A); file <= static_cast<int>(File::H); ++file) {
            Square sq = squareFromFileRank(static_cast<File>(file), static_cast<Rank>(rank));
            g_filesBrd[static_cast<int>(sq)] = file;
            g_ranksBrd[static_cast<int>(sq)] = rank;
        }
    }
}

void initEvalMasks() noexcept {
    for (int sq = 0; sq < 8; ++sq) {
        g_fileBBMask[sq] = 0ULL;
        g_rankBBMask[sq] = 0ULL;
    }

    for (int r = static_cast<int>(Rank::R8); r >= static_cast<int>(Rank::R1); r--) {
        for (int f = static_cast<int>(File::A); f <= static_cast<int>(File::H); f++) {
            int sq = r * 8 + f;
            g_fileBBMask[f] |= (1ULL << sq);
            g_rankBBMask[r] |= (1ULL << sq);
        }
    }

    for (int sq = 0; sq < 64; ++sq) {
        g_isolatedMask[sq] = 0ULL;
        g_whitePassedMask[sq] = 0ULL;
        g_blackPassedMask[sq] = 0ULL;
    }

    for (int sq = 0; sq < 64; ++sq) {
        int tsq = sq + 8;
        while (tsq < 64) {
            g_whitePassedMask[sq] |= (1ULL << tsq);
            tsq += 8;
        }

        tsq = sq - 8;
        while (tsq >= 0) {
            g_blackPassedMask[sq] |= (1ULL << tsq);
            tsq -= 8;
        }

        Square sq120 = squareTo120(sq);
        if (g_filesBrd[static_cast<int>(sq120)] > static_cast<int>(File::A)) {
            g_isolatedMask[sq] |= g_fileBBMask[g_filesBrd[static_cast<int>(sq120)] - 1];
            tsq = sq + 7;
            while (tsq < 64) {
                g_whitePassedMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }
            tsq = sq - 9;
            while (tsq >= 0) {
                g_blackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }

        if (g_filesBrd[static_cast<int>(sq120)] < static_cast<int>(File::H)) {
            g_isolatedMask[sq] |= g_fileBBMask[g_filesBrd[static_cast<int>(sq120)] + 1];
            tsq = sq + 9;
            while (tsq < 64) {
                g_whitePassedMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }
            tsq = sq - 7;
            while (tsq >= 0) {
                g_blackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
    }
}

void initializeAll() noexcept {
    initSq120To64();
    initBitMasks();
    initHashKeys();
    initFilesRanksBrd();
    initEvalMasks();
    movegen::initMvvLva();
    polybook::init();
}

} // namespace chess::internal
