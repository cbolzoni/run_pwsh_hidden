cmake_minimum_required(VERSION 3.20)
project(run_pwsh_hidden C)

set(CMAKE_C_STANDARD 11)

# Executable + resource file
add_executable(run_pwsh_hidden WIN32
    src/run_pwsh_hidden.c
    res/run_pwsh_hidden.rc
)

# Always build as Unicode
target_compile_definitions(run_pwsh_hidden PRIVATE UNICODE _UNICODE)

# Warnings
target_compile_options(run_pwsh_hidden PRIVATE -Wall -Wextra)

# Linker options:
# - On MinGW: use wide-char CRT startup stub and GUI subsystem
# - On MSVC: request GUI subsystem; wWinMain is handled automatically
if(MSVC)
    target_link_options(run_pwsh_hidden PRIVATE /SUBSYSTEM:WINDOWS)
else()
    target_link_options(run_pwsh_hidden PRIVATE -municode -mwindows)
endif()

# Strip only in Release (non-MSVC)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(NOT MSVC)
        target_link_options(run_pwsh_hidden PRIVATE -s)
    endif()
endif()

target_include_directories(run_pwsh_hidden PRIVATE src)
