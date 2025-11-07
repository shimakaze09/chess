#pragma once

#include "chess/types.hpp"
#include <cstdint>

namespace chess {

class SearchInfo {
public:
    SearchInfo() noexcept
        : startTime_(0),
          stopTime_(0),
          depth_(0),
          timeSet_(false),
          movesToGo_(0),
          nodes_(0),
          quit_(false),
          stopped_(false),
          fh_(0.0f),
          fhf_(0.0f),
          nullCut_(0),
          gameMode_(GameMode::Uci),
          postThinking_(false) {}

    [[nodiscard]] int startTime() const noexcept { return startTime_; }
    [[nodiscard]] int stopTime() const noexcept { return stopTime_; }
    [[nodiscard]] int depth() const noexcept { return depth_; }
    [[nodiscard]] bool timeSet() const noexcept { return timeSet_; }
    [[nodiscard]] int movesToGo() const noexcept { return movesToGo_; }
    [[nodiscard]] long nodes() const noexcept { return nodes_; }
    [[nodiscard]] bool quit() const noexcept { return quit_; }
    [[nodiscard]] bool stopped() const noexcept { return stopped_; }
    [[nodiscard]] float fh() const noexcept { return fh_; }
    [[nodiscard]] float fhf() const noexcept { return fhf_; }
    [[nodiscard]] int nullCut() const noexcept { return nullCut_; }
    [[nodiscard]] GameMode gameMode() const noexcept { return gameMode_; }
    [[nodiscard]] bool postThinking() const noexcept { return postThinking_; }

    void setStartTime(int time) noexcept { startTime_ = time; }
    void setStopTime(int time) noexcept { stopTime_ = time; }
    void setDepth(int depth) noexcept { depth_ = depth; }
    void setTimeSet(bool set) noexcept { timeSet_ = set; }
    void setMovesToGo(int moves) noexcept { movesToGo_ = moves; }
    void setNodes(long nodes) noexcept { nodes_ = nodes; }
    void setQuit(bool quit) noexcept { quit_ = quit; }
    void setStopped(bool stopped) noexcept { stopped_ = stopped; }
    void setFh(float fh) noexcept { fh_ = fh; }
    void setFhf(float fhf) noexcept { fhf_ = fhf; }
    void setNullCut(int nullCut) noexcept { nullCut_ = nullCut; }
    void setGameMode(GameMode mode) noexcept { gameMode_ = mode; }
    void setPostThinking(bool post) noexcept { postThinking_ = post; }

    void incrementNodes() noexcept { ++nodes_; }

private:
    int startTime_;
    int stopTime_;
    int depth_;
    bool timeSet_;
    int movesToGo_;
    long nodes_;
    bool quit_;
    bool stopped_;
    float fh_;
    float fhf_;
    int nullCut_;
    GameMode gameMode_;
    bool postThinking_;
};

class EngineOptions {
public:
    EngineOptions() noexcept : useBook_(true) {}

    [[nodiscard]] bool useBook() const noexcept { return useBook_; }
    void setUseBook(bool use) noexcept { useBook_ = use; }

private:
    bool useBook_;
};

inline EngineOptions g_engineOptions;

} // namespace chess

