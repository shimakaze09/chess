# chess

chess engine

## Build Instructions

### Prerequisites

- CMake 3.10 or higher
- C++20 compatible compiler (GCC, Clang, MSVC)

### Building

```bash
# Create build directory
mkdir build
cd build

# Configure (out-of-source recommended)
cmake -S .. -B .

# Build
cmake --build .

# Run
./src/chess
```

### Project Structure

```
chess/
├── CMakeLists.txt      # Main CMake configuration
├── src/                # Source files
│   ├── CMakeLists.txt  # Source CMake configuration
│   └── main.cpp        # Main entry point
├── include/            # Header files
└── tests/              # Test files
```
