//
// Created by 40461 on 2021/8/1.
//

#ifndef TETRISSOLVER_SOLVER_H
#define TETRISSOLVER_SOLVER_H

#include "Brick.h"
#include <string>
#include "Board.h"
#include <map>
#include "String.h"
#include <unordered_map>
//#define unordered_map map
//#define DEBUG

struct ResultType {
    StringPtr steps;
    int score = 0;
};

class Solver {
public:
    Solver(int reservedBoardCount, double scoreWeight);

    ResultType solve(Board &board);

    void setBrickSeq(const vector<Brick> &t);

    void setBrickSeq(vector<Brick> &&t);

private:
    int reservedBoardCount;
    double scoreWeight;
    vector<Brick> brickSeq;
#ifdef DEBUG
    public:
#endif

    static int getFallingHeight(const Brick &brick, int pos, const Board &board);

    static void putNewBrick(const Brick &brick, int pos, int fallingHeight, Board &board);

    static void insertNewBoardAfterFall(const Brick &brick, const Board &board, const ResultType &res,
                                        unordered_map<Board, ResultType> &target, int count, int o, int pos,
                                        int fallingHeight);

    static void
    expand(Brick brick, const Board &board, const ResultType &res, unordered_map<Board, ResultType> &target);

    void filterHighEvaluation(unordered_map<Board, ResultType> &oMap, int count) const;

    ResultType greedyForScore(int first, int last, Board &board);

    void generateChoices(const Brick &brick,const unordered_map<Board, ResultType> &originMap, unordered_map<Board,ResultType> &targetMap);
};


#endif //TETRISSOLVER_SOLVER_H
