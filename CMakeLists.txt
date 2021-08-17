cmake_minimum_required(VERSION 3.16)
project(TetrisSolver)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)

#SET(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O3")


add_executable(TetrisSolver main.cpp Solver.cpp Solver.h BrickGenerator.cpp BrickGenerator.h Brick.h Board.cpp Board.h BoardEvaluator.cpp BoardEvaluator.h TimerUtil.cpp TimerUtil.h String.cpp String.h)