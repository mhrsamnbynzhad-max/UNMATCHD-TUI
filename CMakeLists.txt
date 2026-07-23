cmake_minimum_required(VERSION 4.0.0)

project(game)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(game ${SOURCES} )