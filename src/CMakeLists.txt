# Source files
set(SOURCES
    main.cpp
    bitboard.cpp
)

# Create executable
add_executable(chess ${SOURCES})

# Require C++20 for this target (project already requests C++20, but set per-target)
target_compile_features(chess PRIVATE cxx_std_20)

# Compiler-specific and configuration-specific options
if(MSVC)
    # Use level 4 warnings on MSVC and disable some noisy warnings if desired
    target_compile_options(chess PRIVATE /W4)
    target_compile_definitions(chess PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    # GCC/Clang
    target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic)
    # Debug/Release specific flags
    target_compile_options(chess PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3>
    )
endif()

# Set include directories for the target (use target-based include)
target_include_directories(chess PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_precompile_headers(chess PRIVATE
    "${CMAKE_SOURCE_DIR}/include/pch.hpp"
)

# Link libraries if needed
# target_link_libraries(chess PRIVATE ...)
