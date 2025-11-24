cmake_minimum_required(VERSION 3.10)
project(Logic)

set(CMAKE_CXX_STANDARD 17)

add_executable(Logic src/main.cpp src/Game.cpp src/Board.cpp src/Tile.cpp src/Player.cpp src/CardDeck.cpp)
