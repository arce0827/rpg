cmake_minimum_required(VERSION 3.10)
project(RPG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Manual SFML Configuration ---
# This is a more direct approach since find_package is failing.

# Define the path to the SFML root directory
set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML-3.0.0")

# Create your executable
add_executable(arces
    src/main.cpp
    src/core/GameEngine.cpp
    src/entities/Player.cpp
    src/entities/Projectile.cpp
    src/entities/Enemy.cpp
    src/managers/CollisionManager.cpp
)

# --- Target Configuration for 'arces' ---

# 1. Add project and library include directories
target_include_directories(arces PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"  # For your own headers (e.g., core/GameEngine.hpp)
    "${SFML_ROOT}/include"             # For SFML headers
)

# 2. Add the SFML_STATIC preprocessor definition for static linking
target_compile_definitions(arces PRIVATE SFML_STATIC)

# 3. Add the path to the SFML .lib files
target_link_directories(arces PRIVATE "${SFML_ROOT}/lib")

# 4. Link all the required libraries manually
# Use CMake's 'debug' and 'optimized' keywords to specify the correct library
# for each build configuration. This resolves Debug vs. Release linker errors.
target_link_libraries(arces PRIVATE
    # SFML Libraries
    debug sfml-graphics-s-d   optimized sfml-graphics-s
    debug sfml-window-s-d     optimized sfml-window-s
    debug sfml-audio-s-d      optimized sfml-audio-s
    debug sfml-system-s-d     optimized sfml-system-s

    # Dependencies for SFML 3.0 static linking on Windows.
    # Note: Audio dependencies like openal32, vorbis, etc., are no longer needed
    # as SFML 3 uses miniaudio internally.
    debug "${SFML_ROOT}/lib/freetyped.lib"   optimized "${SFML_ROOT}/lib/freetype.lib"
    opengl32
    winmm
    gdi32
)
