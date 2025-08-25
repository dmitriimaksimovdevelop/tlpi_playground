cmake_minimum_required(VERSION 3.16)
project(tlpi_playground C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
add_compile_options(-Wall -Wextra -Wpedantic -O2 -g)
add_compile_definitions(_DEFAULT_SOURCE _GNU_SOURCE _XOPEN_SOURCE=700)

# Библиотека с утилитами TLPI
add_library(tlpi_common STATIC
        /root/tlpi-dist/lib/error_functions.c
        /root/tlpi-dist/lib/get_num.c
)
target_include_directories(tlpi_common PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/root/tlpi-dist/lib
)

# Твой бинарник
add_executable(tee tee/main.c)
target_link_libraries(tee PRIVATE tlpi_common)
