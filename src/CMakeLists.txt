# Source files
set(SOURCES
    main.cpp
)

# Create executable
add_executable(chess ${SOURCES})

# Set compile options for the target
target_compile_options(chess PRIVATE
    -Wall
    -Wextra
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O3>
)

# Set include directories for the target
target_include_directories(chess PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link libraries if needed
# target_link_libraries(chess PRIVATE ...)
