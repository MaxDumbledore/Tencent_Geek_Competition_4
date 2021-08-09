//
// Created by 40461 on 2021/8/1.
//

#include "BrickGenerator.h"

Brick BrickGenerator::nextBrick() {
    iterate();
    int index = getShapeIndex();
    return {index, (brickCount-1) % (int) BrickShape[index].size()};
}

void BrickGenerator::iterate() {
    randNum = (randNum * A + C) % M;
    brickCount++;
}

BrickGenerator::BrickGenerator() : randNum(12358), brickCount() {}

int BrickGenerator::getShapeIndex() const {
    auto weightIndex = randNum % 29;
    int shapeIndex;

    if (weightIndex >= 0 && weightIndex <= 1) {
        shapeIndex = 0;
    } else if (weightIndex > 1 && weightIndex <= 4) {
        shapeIndex = 1;
    } else if (weightIndex > 4 && weightIndex <= 7) {
        shapeIndex = 2;
    } else if (weightIndex > 7 && weightIndex <= 11) {
        shapeIndex = 3;
    } else if (weightIndex > 11 && weightIndex <= 16) {
        shapeIndex = 4;
    } else if (weightIndex > 16 && weightIndex <= 22) {
        shapeIndex = 5;
    } else /*if (weightIndex > 22)*/ {
        shapeIndex = 6;
    }
    return shapeIndex;
}
