#include "defines.hpp"

int main() {
  constexpr std::size_t sq120Columns = 10;
  constexpr std::size_t sq64Columns = 8;

  // Print mappings with column grouping for readability.
  for (auto index : std::views::iota(std::size_t{0}, Sq120ToSq64.size())) {
    if (index % sq120Columns == 0) {
      std::cout << '\n';
    }
    std::cout << "Sq120ToSq64[" << std::setw(3) << index
              << "] = " << std::setw(2) << Sq120ToSq64[index] << '\t';
  }

  std::cout << "\n\n";

  for (auto index : std::views::iota(std::size_t{0}, Sq64ToSq120.size())) {
    if (index % sq64Columns == 0) {
      std::cout << '\n';
    }
    std::cout << "Sq64ToSq120[" << std::setw(3) << index
              << "] = " << std::setw(3) << Sq64ToSq120[index] << '\t';
  }

  std::cout << '\n';
}
