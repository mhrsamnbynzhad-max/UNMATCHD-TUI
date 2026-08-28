cmake_minimum_required(VERSION 3.16)

project(game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(game ${SOURCES})

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

target_link_libraries(game PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

# Copy game assets to the build directory
file(COPY
    "${CMAKE_SOURCE_DIR}/assets"
    DESTINATION "${CMAKE_BINARY_DIR}"
)

# Copy existing root-level assets
file(GLOB ROOT_ASSETS
    "${CMAKE_SOURCE_DIR}/*.jpg"
    "${CMAKE_SOURCE_DIR}/*.png"
    "${CMAKE_SOURCE_DIR}/*.ttf"
)

file(COPY ${ROOT_ASSETS} DESTINATION "${CMAKE_BINARY_DIR}")
