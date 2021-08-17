#include <iostream>
#include "Solver.h"
#include "BrickGenerator.h"
#include "BoardEvaluator.h"
#include "TimerUtil.h"
#include <fstream>

using namespace std;

//一些测试工作
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
    Board board(t);

    cout << board.getMaxHeight() << endl
         << board.getRowTransition() << endl
         << board.getColumnTransition() << endl
         << board.getNumberOfHoles() << endl
         << board.getWellSums() << endl
         << board.getCount() << endl
         << BoardEvaluator::evaluate(board);
}

int main(int argc, char *argv[]) {
    //生成块序列
    BrickGenerator brickGenerator;
    vector<Brick> brickSeq;
    brickSeq.reserve(10000);
    for (int i = 0; i < brickSeq.capacity(); i++)
        brickSeq.emplace_back(brickGenerator.nextBrick());

    Solver solver(1000, 1.0 / 38);
    solver.setBrickSeq(move(brickSeq));

    TimerUtil::start("ALL");
    Board board;
    auto[res, score]=solver.solve(board);
    res->s.pop_back();
    TimerUtil::finish("ALL");
    TimerUtil::printAllTime();

    //输出答案到answer.txt，第一行为复盘命令，第二行为提交命令
    cout << score << endl;
    ofstream out("answer.txt");
    out << "game.pause();game.playRecord('" << res->getContent() << "'.split(','));" << endl;
    out << "axios.post(`api/upload`,{record:'" << res->getContent() << "',score: " << score
        << "}).then(({data})=>{ console.log('提交结果', data); if(data.info) {console.log(data.info)} });";
    out.close();
    return 0;
}
