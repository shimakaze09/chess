#include "defines.hpp"

int main() {
  U64 primaryBoard = 0ULL;
  SetBit(primaryBoard, 61);
  SetBit(primaryBoard, 12);

  std::cout << "Initial board state:";
  PrintBitboard(primaryBoard);

  std::cout << "Active squares: " << CountBits(primaryBoard) << "\n";

  auto workingBoard = primaryBoard;
  const auto poppedIndex = PopBit(workingBoard);
  std::cout << "After popping least-significant bit (index " << poppedIndex
            << "):";
  PrintBitboard(workingBoard);

  ClearBit(primaryBoard, 61);
  std::cout << "After clearing square 61:";
  PrintBitboard(primaryBoard);

  return 0;
}
