//
// Created by 40461 on 2021/8/2.
//

#ifndef TETRISSOLVER_BOARDEVALUATOR_H
#define TETRISSOLVER_BOARDEVALUATOR_H

#include "Board.h"

class BoardEvaluator {
public:
    static double evaluate(const Board &board);

private:
    inline static constexpr double params[4] = {
            //4.500158825082766,
            //-1,
            -1,
            3.2178882868487753, /*1,*/
            9.348695305445199,
            7.899265427351652
//            3.3855972247263626
    };
};


#endif //TETRISSOLVER_BOARDEVALUATOR_H
