//
// Created by 40461 on 2021/8/1.
//

#ifndef TETRISSOLVER_BRICKGENERATOR_H
#define TETRISSOLVER_BRICKGENERATOR_H

#include "Brick.h"

//按照core.js中的方式来生成块序列

class BrickGenerator {
public:
    BrickGenerator();

    Brick nextBrick();
private:
    static const int A = 27073, M = 32749, C = 17713;

    int randNum, brickCount;

    void iterate();

    [[nodiscard]] int getShapeIndex() const;
};


#endif //TETRISSOLVER_BRICKGENERATOR_H
