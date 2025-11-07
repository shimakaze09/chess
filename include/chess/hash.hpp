#pragma once

#include "chess/types.hpp"
#include <cstdint>
#include <memory>

namespace chess {

class Board;

namespace hash {

std::uint64_t generatePositionKey(const Board& board) noexcept;

} // namespace hash

class HashEntry {
public:
    HashEntry() noexcept : posKey_(0), move_(kNoMove), score_(0), depth_(0), flags_(static_cast<int>(HashFlag::None)) {}

    [[nodiscard]] std::uint64_t posKey() const noexcept { return posKey_; }
    [[nodiscard]] int move() const noexcept { return move_; }
    [[nodiscard]] int score() const noexcept { return score_; }
    [[nodiscard]] int depth() const noexcept { return depth_; }
    [[nodiscard]] HashFlag flags() const noexcept { return static_cast<HashFlag>(flags_); }

    void setPosKey(std::uint64_t key) noexcept { posKey_ = key; }
    void setMove(int move) noexcept { move_ = move; }
    void setScore(int score) noexcept { score_ = score; }
    void setDepth(int depth) noexcept { depth_ = depth; }
    void setFlags(HashFlag flags) noexcept { flags_ = static_cast<int>(flags); }

private:
    std::uint64_t posKey_;
    int move_;
    int score_;
    int depth_;
    int flags_;
};

class HashTable {
public:
    HashTable() noexcept : numEntries_(0), newWrite_(0), overWrite_(0), hit_(0), cut_(0) {}
    ~HashTable() = default;

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable(HashTable&&) noexcept = default;
    HashTable& operator=(HashTable&&) noexcept = default;

    void init(int mb);
    void clear() noexcept;

    [[nodiscard]] int numEntries() const noexcept { return numEntries_; }
    [[nodiscard]] int newWrite() const noexcept { return newWrite_; }
    [[nodiscard]] int overWrite() const noexcept { return overWrite_; }
    [[nodiscard]] int hit() const noexcept { return hit_; }
    [[nodiscard]] int cut() const noexcept { return cut_; }

    [[nodiscard]] HashEntry* entries() noexcept { return pTable_.get(); }
    [[nodiscard]] const HashEntry* entries() const noexcept { return pTable_.get(); }

    void incrementNewWrite() noexcept { ++newWrite_; }
    void incrementOverWrite() noexcept { ++overWrite_; }
    void incrementHit() noexcept { ++hit_; }
    void incrementCut() noexcept { ++cut_; }

private:
    std::unique_ptr<HashEntry[]> pTable_;
    int numEntries_;
    int newWrite_;
    int overWrite_;
    int hit_;
    int cut_;
};

} // namespace chess

