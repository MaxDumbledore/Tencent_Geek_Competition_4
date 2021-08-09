#include <iostream>
#include "Solver.h"
#include "BrickGenerator.h"
#include "BoardEvaluator.h"
#include "TimerUtil.h"
#include <fstream>

using namespace std;

void testEvaluator() {

    vector<vector<int>> t = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                             {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
                             {1, 0, 1, 1, 0, 1, 1, 0, 0, 0},
                             {1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                             {1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
                             {0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
                             {1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
                             {1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                             {0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                             {0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
                             {1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
                             {0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                             {1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 1, 1, 1, 1, 0, 1, 1}};
    Board board;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (t[i][j])
                board.set(i, j);
    cout << board.getMaxHeight() << endl
         << board.getRowTransition() << endl
         << board.getColumnTransition() << endl
         << board.getNumberOfHoles() << endl
         << board.getWellSums() << endl
         << BoardEvaluator::evaluate(board);
}

#ifdef DEBUG

void debug() {
    vector<vector<int>> t = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                             {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                             {1, 1, 0, 0, 1, 0, 0, 0, 0, 0},
                             {1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
                             {1, 1, 1, 0, 1, 1, 1, 1, 1, 1}};
    Board board;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (t[i][j])
                board.set(i, j);
//    unordered_map<Board, ResultType> g;
//    Solver::expand(Brick{5, 1}, board, ResultType(), g);
//    for (auto &i:g)
//        i.first.print();
}

#endif

int main(int argc, char *argv[]) {
    BrickGenerator brickGenerator;
    vector<Brick> brickSeq;
    brickSeq.reserve(10000);
    for (int i = 0; i < brickSeq.capacity(); i++)
        brickSeq.emplace_back(brickGenerator.nextBrick());

    Solver solver(1000, 1.0 / 38);
    solver.setBrickSeq(move(brickSeq));
    TimerUtil::start("ALL");
    auto[res, score]=solver.solve();
    res->s.pop_back();
    TimerUtil::finish("ALL");
    TimerUtil::printAllTime();

    cout << score << endl;
    ofstream out("answer.txt");
    out << "game.pause();game.playRecord('" << res->getContent() << "'.split(','));" << endl;
    out << "axios.post(`api/upload`,{record:'" << res->getContent() << "',score: " << score
        << "}).then(({data})=>{ console.log('提交结果', data); if(data.info) {console.log(data.info)} });";
    out.close();
    return 0;
}
