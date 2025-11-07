#pragma once

#include "chess/types.hpp"
#include "chess/move.hpp"
#include "chess/hash.hpp"
#include <array>
#include <cstdint>
#include <string_view>

namespace chess {

class Undo {
public:
    Undo() noexcept : move_(kNoMove), castlePerm_(0), enPas_(Square::NoSquare), fiftyMove_(0), posKey_(0) {}

    [[nodiscard]] int move() const noexcept { return move_; }
    [[nodiscard]] int castlePerm() const noexcept { return castlePerm_; }
    [[nodiscard]] Square enPas() const noexcept { return enPas_; }
    [[nodiscard]] int fiftyMove() const noexcept { return fiftyMove_; }
    [[nodiscard]] std::uint64_t posKey() const noexcept { return posKey_; }

    void setMove(int move) noexcept { move_ = move; }
    void setCastlePerm(int perm) noexcept { castlePerm_ = perm; }
    void setEnPas(Square sq) noexcept { enPas_ = sq; }
    void setFiftyMove(int move) noexcept { fiftyMove_ = move; }
    void setPosKey(std::uint64_t key) noexcept { posKey_ = key; }

private:
    int move_;
    int castlePerm_;
    Square enPas_;
    int fiftyMove_;
    std::uint64_t posKey_;
};

class Board {
public:
    Board() noexcept;
    ~Board() = default;

    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;
    Board(Board&&) noexcept = default;
    Board& operator=(Board&&) noexcept = default;

    void reset() noexcept;
    bool parseFen(std::string_view fen) noexcept;
    void print() const noexcept;
    void updateListsMaterial() noexcept;
    bool checkBoard() const noexcept;
    void mirror() noexcept;

    [[nodiscard]] Piece pieceAt(Square sq) const noexcept { return pieces_[static_cast<int>(sq)]; }
    [[nodiscard]] Bitboard pawns(Color color) const noexcept { return pawns_[static_cast<int>(color)]; }
    [[nodiscard]] Square kingSquare(Color color) const noexcept { return kingSq_[static_cast<int>(color)]; }
    [[nodiscard]] Color side() const noexcept { return side_; }
    [[nodiscard]] Square enPas() const noexcept { return enPas_; }
    [[nodiscard]] int fiftyMove() const noexcept { return fiftyMove_; }
    [[nodiscard]] int ply() const noexcept { return ply_; }
    [[nodiscard]] int hisPly() const noexcept { return hisPly_; }
    [[nodiscard]] int castlePerm() const noexcept { return castlePerm_; }
    [[nodiscard]] std::uint64_t posKey() const noexcept { return posKey_; }
    [[nodiscard]] int pieceCount(Piece pce) const noexcept { return pceNum_[static_cast<int>(pce)]; }
    [[nodiscard]] int bigPiece(Color color) const noexcept { return bigPce_[static_cast<int>(color)]; }
    [[nodiscard]] int majPiece(Color color) const noexcept { return majPce_[static_cast<int>(color)]; }
    [[nodiscard]] int minPiece(Color color) const noexcept { return minPce_[static_cast<int>(color)]; }
    [[nodiscard]] int material(Color color) const noexcept { return material_[static_cast<int>(color)]; }
    [[nodiscard]] Square pieceList(Piece pce, int index) const noexcept { return static_cast<Square>(pList_[static_cast<int>(pce)][index]); }
    [[nodiscard]] HashTable& hashTable() noexcept { return hashTable_; }
    [[nodiscard]] const HashTable& hashTable() const noexcept { return hashTable_; }
    [[nodiscard]] int pvArray(int index) const noexcept { return pvArray_[index]; }
    [[nodiscard]] int& pvArray(int index) noexcept { return pvArray_[index]; }
    [[nodiscard]] int searchHistory(Piece pce, Square sq) const noexcept { return searchHistory_[static_cast<int>(pce)][static_cast<int>(sq)]; }
    [[nodiscard]] int& searchHistory(Piece pce, Square sq) noexcept { return searchHistory_[static_cast<int>(pce)][static_cast<int>(sq)]; }
    [[nodiscard]] int searchKiller(Color color, int depth) const noexcept { return searchKillers_[static_cast<int>(color)][depth]; }
    [[nodiscard]] int& searchKiller(Color color, int depth) noexcept { return searchKillers_[static_cast<int>(color)][depth]; }
    [[nodiscard]] const Undo& history(int index) const noexcept { return history_[index]; }
    [[nodiscard]] Undo& history(int index) noexcept { return history_[index]; }

    void setPieceAt(Square sq, Piece pce) noexcept { pieces_[static_cast<int>(sq)] = pce; }
    void setPawns(Color color, Bitboard bb) noexcept { pawns_[static_cast<int>(color)] = bb; }
    void setKingSquare(Color color, Square sq) noexcept { kingSq_[static_cast<int>(color)] = sq; }
    void setSide(Color side) noexcept { side_ = side; }
    void setEnPas(Square sq) noexcept { enPas_ = sq; }
    void setFiftyMove(int move) noexcept { fiftyMove_ = move; }
    void setPly(int ply) noexcept { ply_ = ply; }
    void setHisPly(int ply) noexcept { hisPly_ = ply; }
    void setCastlePerm(int perm) noexcept { castlePerm_ = perm; }
    void setPosKey(std::uint64_t key) noexcept { posKey_ = key; }
    void setPieceCount(Piece pce, int count) noexcept { pceNum_[static_cast<int>(pce)] = count; }
    void setBigPiece(Color color, int count) noexcept { bigPce_[static_cast<int>(color)] = count; }
    void setMajPiece(Color color, int count) noexcept { majPce_[static_cast<int>(color)] = count; }
    void setMinPiece(Color color, int count) noexcept { minPce_[static_cast<int>(color)] = count; }
    void setMaterial(Color color, int material) noexcept { material_[static_cast<int>(color)] = material; }
    void setPieceList(Piece pce, int index, Square sq) noexcept { pList_[static_cast<int>(pce)][index] = static_cast<int>(sq); }

    bool makeMove(Move move) noexcept;
    void takeMove() noexcept;
    void makeNullMove() noexcept;
    void takeNullMove() noexcept;
    bool isSquareAttacked(Square sq, Color side) const noexcept;

private:
    std::array<Piece, kBoardSquareCount> pieces_;
    std::array<Bitboard, 3> pawns_;
    std::array<Square, 2> kingSq_;
    Color side_;
    Square enPas_;
    int fiftyMove_;
    int ply_;
    int hisPly_;
    int castlePerm_;
    std::uint64_t posKey_;
    std::array<int, 13> pceNum_;
    std::array<int, 2> bigPce_;
    std::array<int, 2> majPce_;
    std::array<int, 2> minPce_;
    std::array<int, 2> material_;
    std::array<Undo, kMaxGameMoves> history_;
    std::array<std::array<int, 10>, 13> pList_;
    HashTable hashTable_;
    std::array<int, kMaxDepth> pvArray_;
    std::array<std::array<int, kBoardSquareCount>, 13> searchHistory_;
    std::array<std::array<int, kMaxDepth>, 2> searchKillers_;
};

} // namespace chess

