cmake_minimum_required(VERSION 3.10)

project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(game ${SOURCES} )

include_directories(${CMAKE_SOURCE_DIR}/SFML/include)
link_directories(${CMAKE_SOURCE_DIR}/SFML/lib)

target_link_libraries(game PRIVATE 
    ${CMAKE_SOURCE_DIR}/SFML/lib/libsfml-graphics-d.a
    ${CMAKE_SOURCE_DIR}/SFML/lib/libsfml-window-d.a
    ${CMAKE_SOURCE_DIR}/SFML/lib/libsfml-system-d.a
)