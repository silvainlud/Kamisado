cmake_minimum_required(VERSION 3.9)
project(kamisado)

set(CMAKE_CXX_STANDARD 11)

add_executable(kamisado main.cpp Coordinates.cpp Coordinates.h Move.cpp Move.h State.cpp State.h enum_utils.h Engine.cpp Engine.h Node.cpp Node.h RandomPlayer.cpp RandomPlayer.h MCTSPlayer.cpp MCTSPlayer.h)