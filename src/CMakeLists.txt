set(SOURCES
    main.cpp
    chess/bitboard.cpp
    chess/board.cpp
    chess/hash.cpp
    chess/internal/data.cpp
    chess/internal/init.cpp
    chess/misc.cpp
    chess/movegen.cpp
    chess/polybook.cpp
    chess/search.cpp
    chess/uci.cpp
    chess/xboard.cpp
)

add_executable(chess ${SOURCES})

target_compile_features(chess PRIVATE cxx_std_20)

if(MSVC)
    target_compile_options(chess PRIVATE /W4)
    target_compile_definitions(chess PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(chess PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(chess PRIVATE
        $<$<CONFIG:Debug>:-g -O0>
        $<$<CONFIG:Release>:-O3>
    )
endif()

target_include_directories(chess PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_precompile_headers(chess PRIVATE
    "${CMAKE_SOURCE_DIR}/include/pch.hpp"
)
