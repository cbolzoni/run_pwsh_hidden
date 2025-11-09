cmake_minimum_required(VERSION 3.20)
project(run_pwsh_hidden C)

set(CMAKE_C_STANDARD 11)

add_executable(run_pwsh_hidden WIN32
    src/run_pwsh_hidden.c
)

target_compile_options(run_pwsh_hidden PRIVATE -municode -mwindows -Wall -Wextra)
target_link_options(run_pwsh_hidden PRIVATE -s)

if (WIN32)
    target_sources(run_pwsh_hidden PRIVATE res/run_pwsh_hidden.rc)
endif()

target_include_directories(run_pwsh_hidden PRIVATE src)
