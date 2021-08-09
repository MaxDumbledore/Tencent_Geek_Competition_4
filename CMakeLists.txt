cmake_minimum_required(VERSION 3.16)
project(TetrisSolver)

set(CMAKE_CXX_STANDARD 17)

add_executable(TetrisSolver main.cpp Solver.cpp Solver.h BrickGenerator.cpp BrickGenerator.h Brick.h Board.cpp Board.h BoardEvaluator.cpp BoardEvaluator.h TimerUtil.cpp TimerUtil.h String.cpp String.h)