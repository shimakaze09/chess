#include "chess/board.hpp"
#include "chess/hash.hpp"
#include "chess/internal/data.hpp"
#include "chess/internal/init.hpp"
#include "chess/types.hpp"
#include "chess/bitboard.hpp"
#include <cstdio>
#include <cstring>
#include <cassert>

namespace chess {

Board::Board() noexcept {
    reset();
}

void Board::reset() noexcept {
    for (int index = 0; index < kBoardSquareCount; ++index) {
        pieces_[index] = Piece::Empty;
    }

    for (int index = 0; index < 64; ++index) {
        pieces_[internal::g_sq64ToSq120[index]] = Piece::Empty;
    }

    for (int index = 0; index < 2; ++index) {
        bigPce_[index] = 0;
        majPce_[index] = 0;
        minPce_[index] = 0;
        material_[index] = 0;
    }

    for (int index = 0; index < 3; ++index) {
        pawns_[index] = 0ULL;
    }

    for (int index = 0; index < 13; ++index) {
        pceNum_[index] = 0;
    }

    kingSq_[0] = kingSq_[1] = Square::NoSquare;
    side_ = Color::Both;
    enPas_ = Square::NoSquare;
    fiftyMove_ = 0;
    ply_ = 0;
    hisPly_ = 0;
    castlePerm_ = 0;
    posKey_ = 0ULL;
}

bool Board::parseFen(std::string_view fen) noexcept {
    reset();

    int rank = static_cast<int>(Rank::R8);
    int file = static_cast<int>(File::A);
    Piece piece = Piece::Empty;
    int count = 0;
    size_t i = 0;
    int sq64 = 0;
    Square sq120 = Square::NoSquare;

    while (rank >= static_cast<int>(Rank::R1) && i < fen.length()) {
        count = 1;
        switch (fen[i]) {
            case 'p': piece = Piece::BlackPawn; break;
            case 'r': piece = Piece::BlackRook; break;
            case 'n': piece = Piece::BlackKnight; break;
            case 'b': piece = Piece::BlackBishop; break;
            case 'k': piece = Piece::BlackKing; break;
            case 'q': piece = Piece::BlackQueen; break;
            case 'P': piece = Piece::WhitePawn; break;
            case 'R': piece = Piece::WhiteRook; break;
            case 'N': piece = Piece::WhiteKnight; break;
            case 'B': piece = Piece::WhiteBishop; break;
            case 'K': piece = Piece::WhiteKing; break;
            case 'Q': piece = Piece::WhiteQueen; break;
            case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8':
                piece = Piece::Empty;
                count = fen[i] - '0';
                break;
            case '/': case ' ':
                rank--;
                file = static_cast<int>(File::A);
                i++;
                continue;
            default:
                return false;
        }

        for (int j = 0; j < count; j++) {
            sq64 = rank * 8 + file;
            sq120 = squareTo120(sq64);
            if (piece != Piece::Empty) {
                pieces_[static_cast<int>(sq120)] = piece;
            }
            file++;
        }
        i++;
    }

    if (i >= fen.length() || (fen[i] != 'w' && fen[i] != 'b')) {
        return false;
    }

    side_ = (fen[i] == 'w') ? Color::White : Color::Black;
    i += 2;

    for (int j = 0; j < 4 && i < fen.length(); j++) {
        if (fen[i] == ' ') {
            break;
        }
        switch (fen[i]) {
            case 'K': castlePerm_ |= static_cast<int>(CastleRights::WhiteKingside); break;
            case 'Q': castlePerm_ |= static_cast<int>(CastleRights::WhiteQueenside); break;
            case 'k': castlePerm_ |= static_cast<int>(CastleRights::BlackKingside); break;
            case 'q': castlePerm_ |= static_cast<int>(CastleRights::BlackQueenside); break;
            default: break;
        }
        i++;
    }
    i++;

    if (i < fen.length() && fen[i] != '-') {
        if (i + 1 < fen.length()) {
            file = fen[i] - 'a';
            rank = fen[i + 1] - '1';
            if (file >= 0 && file <= 7 && rank >= 0 && rank <= 7) {
                enPas_ = squareFromFileRank(static_cast<File>(file), static_cast<Rank>(rank));
            }
            i += 2;
        }
    } else if (i < fen.length() && fen[i] == '-') {
        i++;
    }

    posKey_ = hash::generatePositionKey(*this);
    updateListsMaterial();

    return true;
}

void Board::print() const noexcept {
    printf("\nGame Board:\n\n");

    for (int rank = static_cast<int>(Rank::R8); rank >= static_cast<int>(Rank::R1); rank--) {
        printf("%d  ", rank + 1);
        for (int file = static_cast<int>(File::A); file <= static_cast<int>(File::H); file++) {
            Square sq = squareFromFileRank(static_cast<File>(file), static_cast<Rank>(rank));
            Piece piece = pieces_[static_cast<int>(sq)];
            printf("%3c", internal::kPieceChar[static_cast<int>(piece)]);
        }
        printf("\n");
    }

    printf("\n   ");
    for (int file = static_cast<int>(File::A); file <= static_cast<int>(File::H); file++) {
        printf("%3c", 'a' + file);
    }
    printf("\n");
    printf("side:%c\n", internal::kSideChar[static_cast<int>(side_)]);
    printf("enPas:%d\n", static_cast<int>(enPas_));
    printf("castle:%c%c%c%c\n",
           (castlePerm_ & static_cast<int>(CastleRights::WhiteKingside)) ? 'K' : '-',
           (castlePerm_ & static_cast<int>(CastleRights::WhiteQueenside)) ? 'Q' : '-',
           (castlePerm_ & static_cast<int>(CastleRights::BlackKingside)) ? 'k' : '-',
           (castlePerm_ & static_cast<int>(CastleRights::BlackQueenside)) ? 'q' : '-');
    printf("PosKey:%llX\n", posKey_);
}

void Board::updateListsMaterial() noexcept {
    for (int index = 0; index < kBoardSquareCount; ++index) {
        Square sq = static_cast<Square>(index);
        Piece piece = pieces_[index];
        if (piece != Piece::Empty && static_cast<int>(sq) != static_cast<int>(Square::OffBoard)) {
            Color col = static_cast<Color>(internal::kPieceCol[static_cast<int>(piece)]);

            if (internal::kPieceBig[static_cast<int>(piece)] != 0) {
                bigPce_[static_cast<int>(col)]++;
                if (internal::kPieceMaj[static_cast<int>(piece)] != 0) {
                    majPce_[static_cast<int>(col)]++;
                } else {
                    minPce_[static_cast<int>(col)]++;
                }
            }

            material_[static_cast<int>(col)] += internal::kPieceVal[static_cast<int>(piece)];

            pList_[static_cast<int>(piece)][pceNum_[static_cast<int>(piece)]] = static_cast<int>(sq);
            pceNum_[static_cast<int>(piece)]++;

            if (piece == Piece::WhiteKing) {
                kingSq_[static_cast<int>(Color::White)] = sq;
            }
            if (piece == Piece::BlackKing) {
                kingSq_[static_cast<int>(Color::Black)] = sq;
            }

            if (piece == Piece::WhitePawn) {
                int sq64 = internal::g_sq120ToSq64[static_cast<int>(sq)];
                bitboard::setBit(pawns_[static_cast<int>(Color::White)], sq64);
                bitboard::setBit(pawns_[static_cast<int>(Color::Both)], sq64);
            } else if (piece == Piece::BlackPawn) {
                int sq64 = internal::g_sq120ToSq64[static_cast<int>(sq)];
                bitboard::setBit(pawns_[static_cast<int>(Color::Black)], sq64);
                bitboard::setBit(pawns_[static_cast<int>(Color::Both)], sq64);
            }
        }
    }
}

bool Board::checkBoard() const noexcept {
    return true;
}

void Board::mirror() noexcept {
}

bool Board::isSquareAttacked(Square sq, Color side) const noexcept {
    return false;
}

bool Board::makeMove(Move move) noexcept {
    return false;
}

void Board::takeMove() noexcept {
}

void Board::makeNullMove() noexcept {
}

void Board::takeNullMove() noexcept {
}

} // namespace chess

