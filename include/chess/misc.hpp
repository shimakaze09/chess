#pragma once

#include "chess/types.hpp"

namespace chess {

class SearchInfo;

namespace misc {

int getTimeMs() noexcept;
void readInput(SearchInfo& info) noexcept;

} // namespace misc

} // namespace chess

