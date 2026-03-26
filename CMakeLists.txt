cmake_minimum_required(VERSION 4.1)
project(LetsLearnSDL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Suppress linker warnings from third-party shared libraries (macOS ld deprecations).
# CMAKE_SHARED_LINKER_FLAGS only affects SHARED targets, not our executables.
if (APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-w")
endif()

# --- SDL3 ---
set(SDL_TESTS    OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
    GIT_TAG        "release-3.4.2"
)
FetchContent_MakeAvailable(SDL3)

# --- SDL3_ttf ---
set(SDLTTF_FREETYPE  OFF CACHE BOOL "" FORCE)
set(SDLTTF_VENDORED  ON  CACHE BOOL "" FORCE)
set(SDLTTF_HARFBUZZ  OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_ttf.git"
    GIT_TAG        "release-3.2.2"
)
FetchContent_MakeAvailable(SDL3_ttf)

# freetype (vendored by SDL3_ttf) redefines OS_CODE in its gzip source.
if (TARGET freetype)
    target_compile_options(freetype PRIVATE -Wno-macro-redefined)
endif()

# --- SDL3_mixer ---
set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY "https://github.com/libsdl-org/SDL_mixer.git"
    GIT_TAG        "release-3.2.0"
)
FetchContent_MakeAvailable(SDL3_mixer)

# opus emits a #pragma message when built without optimization (Debug builds).
if (TARGET opus)
    target_compile_options(opus PRIVATE "-Wno-#pragma-messages")
endif()

# Fix for missing headers when compiling static version on Windows
if (WIN32)
    if (TARGET libmpg123)
        target_include_directories(SDL3_mixer-static PRIVATE
            "${SDL3_mixer_SOURCE_DIR}/external/mpg123/src/include"
            "${SDL3_mixer_BINARY_DIR}/external/libmpg123-build/ports/cmake/src/libmpg123"
        )
    endif()
endif()

add_subdirectory(engine)
add_subdirectory(demo1)
add_subdirectory(demo2_pong)
add_subdirectory(demo3_space)