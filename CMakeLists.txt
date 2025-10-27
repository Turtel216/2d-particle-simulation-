cmake_minimum_required(VERSION 3.16)
project(sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json (important for LSPs like clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Fetch SFML from GitHub
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1   # or "master" for latest
)

FetchContent_MakeAvailable(SFML)

# Add your main executable
add_executable(sim
    src/main.cpp
    src/render.cpp
    src/particle.cpp
)

# Link SFML
target_link_libraries(sim PRIVATE sfml-graphics)

# Optionally include SFML headers explicitly (LSP visibility)
target_include_directories(sim PRIVATE ${SFML_SOURCE_DIR}/include)
