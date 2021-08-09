//
// Created by 40461 on 2021/8/2.
//

#include "BoardEvaluator.h"

double BoardEvaluator::evaluate(const Board &board) {
    return params[0] * board.getCount() + params[1] * board.getRowTransition() +
           params[2] * board.getColumnTransition() + params[3] * board.getNumberOfHoles() +
           params[4] * board.getWellSums();
}
