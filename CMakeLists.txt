cmake_minimum_required(VERSION 3.10)

project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(game ${SOURCES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML REQUIRED sfml-graphics sfml-audio)

target_include_directories(game PRIVATE
    ${SFML_INCLUDE_DIRS}
)

target_link_directories(game PRIVATE
    ${SFML_LIBRARY_DIRS}
)

target_link_libraries(game PRIVATE
    ${SFML_LIBRARIES}
)
