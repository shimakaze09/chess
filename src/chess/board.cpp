#include "chess/board.hpp"

#include <cassert>
#include <cctype>
#include <format>
#include <iostream>
#include <optional>

#include "chess/bitboard.hpp"
#include "chess/hash.hpp"
#include "chess/internal/data.hpp"
#include "chess/internal/init.hpp"
#include "chess/types.hpp"

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

    constexpr auto kBoardSize = 8;
    constexpr char kRankSeparator = '/';
    constexpr char kFieldSeparator = ' ';

    // Lambda to convert character to piece
    const auto charToPiece = [](char ch) constexpr -> std::optional<Piece> {
        switch (ch) {
            case 'p':
                return Piece::BlackPawn;
            case 'r':
                return Piece::BlackRook;
            case 'n':
                return Piece::BlackKnight;
            case 'b':
                return Piece::BlackBishop;
            case 'k':
                return Piece::BlackKing;
            case 'q':
                return Piece::BlackQueen;
            case 'P':
                return Piece::WhitePawn;
            case 'R':
                return Piece::WhiteRook;
            case 'N':
                return Piece::WhiteKnight;
            case 'B':
                return Piece::WhiteBishop;
            case 'K':
                return Piece::WhiteKing;
            case 'Q':
                return Piece::WhiteQueen;
            default:
                return std::nullopt;
        }
    };

    auto current_rank = static_cast<int>(Rank::R8);
    auto current_file = static_cast<int>(File::A);
    std::size_t pos = 0;

    // Parse board position
    while (current_rank >= static_cast<int>(Rank::R1) && pos < fen.length()) {
        const char current_char = fen[pos];

        if (current_char == kRankSeparator || current_char == kFieldSeparator) {
            current_rank--;
            current_file = static_cast<int>(File::A);
            pos++;
            if (current_char == kFieldSeparator) break;
            continue;
        }

        if (std::isdigit(current_char)) {
            const int empty_squares = current_char - '0';
            current_file += empty_squares;
        } else if (const auto piece_opt = charToPiece(current_char)) {
            const auto square_64 = (current_rank * kBoardSize) + current_file;
            const auto square_120 = internal::squareTo120(square_64);
            pieces_[static_cast<int>(square_120)] = *piece_opt;
            current_file++;
        } else {
            return false; // Invalid character
        }

        pos++;
    }

    // Parse side to move
    if (pos >= fen.length() || (fen[pos] != 'w' && fen[pos] != 'b')) {
        return false;
    }

    side_ = (fen[pos] == 'w') ? Color::White : Color::Black;
    pos += 2; // Skip side and space

    // Parse castling rights
    constexpr int kMaxCastleChars = 4;
    for (int castle_idx = 0; castle_idx < kMaxCastleChars && pos < fen.length(); ++castle_idx) {
        if (fen[pos] == kFieldSeparator) {
            break;
        }

        switch (fen[pos]) {
            case 'K':
                castlePerm_ |= static_cast<int>(CastleRights::WhiteKingside);
                break;
            case 'Q':
                castlePerm_ |= static_cast<int>(CastleRights::WhiteQueenside);
                break;
            case 'k':
                castlePerm_ |= static_cast<int>(CastleRights::BlackKingside);
                break;
            case 'q':
                castlePerm_ |= static_cast<int>(CastleRights::BlackQueenside);
                break;
            case '-':
                break; // No castling rights
            default:
                break;
        }
        pos++;
    }

    if (pos < fen.length() && fen[pos] == kFieldSeparator) {
        pos++;
    }

    // Parse en passant square
    if (pos < fen.length() && fen[pos] != '-') {
        if (pos + 1 < fen.length()) {
            const auto file_char = fen[pos] - 'a';
            const auto rank_char = fen[pos + 1] - '1';
            constexpr int kMinFileRank = 0;
            constexpr int kMaxFileRank = 7;

            if (file_char >= kMinFileRank && file_char <= kMaxFileRank &&
                rank_char >= kMinFileRank && rank_char <= kMaxFileRank) {
                enPas_ =
                    squareFromFileRank(static_cast<File>(file_char), static_cast<Rank>(rank_char));
            }
            pos += 2;
        }
    } else if (pos < fen.length() && fen[pos] == '-') {
        pos++;
    }

    posKey_ = hash::generatePositionKey(*this);
    updateListsMaterial();

    return true;
}

void Board::print() const noexcept {
    std::cout << "\nGame Board:\n\n";

    for (auto rank_idx = static_cast<int>(Rank::R8); rank_idx >= static_cast<int>(Rank::R1);
         --rank_idx) {
        std::cout << std::format("{}  ", rank_idx + 1);

        for (auto file_idx = static_cast<int>(File::A); file_idx <= static_cast<int>(File::H);
             ++file_idx) {
            const auto square =
                squareFromFileRank(static_cast<File>(file_idx), static_cast<Rank>(rank_idx));
            const auto piece = pieces_[static_cast<int>(square)];
            std::cout << std::format("{:>3}", internal::kPieceChar[static_cast<int>(piece)]);
        }
        std::cout << '\n';
    }

    std::cout << "\n   ";
    for (auto file_idx = static_cast<int>(File::A); file_idx <= static_cast<int>(File::H);
         ++file_idx) {
        std::cout << std::format("{:>3}", static_cast<char>('a' + file_idx));
    }
    std::cout << '\n';
    std::cout << std::format("side:{}\n", internal::kSideChar[static_cast<int>(side_)]);
    std::cout << std::format("enPas:{}\n", static_cast<int>(enPas_));

    const auto castle_rights = std::format(
        "castle:{}{}{}{}\n",
        (castlePerm_ & static_cast<int>(CastleRights::WhiteKingside)) != 0 ? 'K' : '-',
        (castlePerm_ & static_cast<int>(CastleRights::WhiteQueenside)) != 0 ? 'Q' : '-',
        (castlePerm_ & static_cast<int>(CastleRights::BlackKingside)) != 0 ? 'k' : '-',
        (castlePerm_ & static_cast<int>(CastleRights::BlackQueenside)) != 0 ? 'q' : '-');
    std::cout << castle_rights;

    std::cout << std::format("PosKey:{:X}\n", posKey_);
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

            pList_[static_cast<int>(piece)][pceNum_[static_cast<int>(piece)]] =
                static_cast<int>(sq);
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

void Board::mirror() noexcept {}

bool Board::isSquareAttacked(Square sq, Color side) const noexcept {
    static_cast<void>(sq);
    static_cast<void>(side);
    return false;
}

bool Board::makeMove(Move move) noexcept {
    static_cast<void>(move);
    return false;
}

void Board::takeMove() noexcept {}

void Board::makeNullMove() noexcept {}

void Board::takeNullMove() noexcept {}

} // namespace chess
