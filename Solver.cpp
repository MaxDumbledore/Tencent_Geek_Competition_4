//
// Created by 40461 on 2021/8/1.
//

#include "Solver.h"
#include <algorithm>
#include <queue>
#include <iostream>
#include "BoardEvaluator.h"
#include "TimerUtil.h"

using namespace std;

Solver::Solver(int reservedBoardCount, double scoreWeight) : reservedBoardCount(reservedBoardCount),
                                                             scoreWeight(scoreWeight) {}

int Solver::getFallingHeight(const Brick &brick, int pos, const Board &board) {
    for (int i = 0; i < N; i++) {
        for (auto j:BrickShape[brick.index][brick.orientation]) {
            j.second += pos, j.first += i;
            if (j.first < 0)
                continue;
            if (j.first >= N || board.query(j.first, j.second))
                return i - 1;
        }
    }
    return N - 1;
}

void Solver::putNewBrick(const Brick &brick, int pos, int fallingHeight, Board &board) {
    for (auto i:BrickShape[brick.index][brick.orientation]) {
        i.first += fallingHeight, i.second += pos;
        board.set(i.first, i.second);
    }
}

void Solver::expand(Brick brick, const Board &board, const ResultType &res, unordered_map<Board, ResultType> &target) {
    int orientationCount = (int) BrickShape[brick.index].size(), count = board.getCount() + 4;
    for (int o = 0; o < orientationCount; o++) {
        int l = 1E9, r = -1E9, u = 1E9;
        for (auto &i:BrickShape[brick.index][brick.orientation]) {
            l = min(l, i.second);
            r = max(r, i.second);
            u = min(u, i.first);
        }
        auto initFallingHeight = getFallingHeight(brick, 4, board);
        if (initFallingHeight + u < 0)
            break;
//        if (initFallingHeight <= 0)
//            break;
        for (int i = 4; i >= -l; i--) {
            auto fallingHeight = getFallingHeight(brick, i, board);
            if (fallingHeight + u < 0)
                break;
            if (fallingHeight + u > 0)
                insertNewBoardAfterFall(brick, board, res, target, count, o, i, fallingHeight);
        }
        for (int i = 5; i + r < M; i++) {
            auto fallingHeight = getFallingHeight(brick, i, board);
            if (fallingHeight + u < 0)
                break;
            if (fallingHeight + u > 0)
                insertNewBoardAfterFall(brick, board, res, target, count, o, i, fallingHeight);
        }
        brick.orientation = (brick.orientation + 1) % orientationCount;
    }
}

void Solver::insertNewBoardAfterFall(const Brick &brick, const Board &board, const ResultType &res,
                                     unordered_map<Board, ResultType> &target, int count, int o, int pos,
                                     int fallingHeight) {
    Board nBoard = board;
    ResultType nRes = {make_shared<String>(), res.score};
    putNewBrick(brick, pos, fallingHeight, nBoard);
    int removed = nBoard.removeFullRow();
    nRes.score += removed * (removed + 1) / 2 * count;
    nRes.steps->s += "N,";
    if (o > 0)
        nRes.steps->s += 'C' + to_string(o) + ',';
    if (pos != 4)
        nRes.steps->s += (pos > 4 ? 'R' : 'L') + to_string(abs(pos - 4)) + ',';
    nRes.steps->s += 'D' + to_string(fallingHeight) + ',';
    nRes.steps->prev = res.steps;

    //Caution! score is not less than zero!
    auto &temp = target[nBoard];
    if (temp.steps == nullptr || temp.score < nRes.score)
        temp = move(nRes);
}

//void Solver::filterHighEvaluation(unordered_map<Board, ResultType> &oMap, int count) {
//    if (count >= oMap.size())
//        return;
//    int norm = 0;
//
//    priority_queue<pair<double, decltype(&*oMap.begin())>> Q;
//    for (auto &it:oMap) {
//        //auto evaluation = BoardEvaluator::evaluate(it.first);
//        auto evaluation =
//                BoardEvaluator::evaluate(it.first) -
//                it.second.score / 30.0;
//        /*(double) it.second.score / norm * 125;*/
//        //(it.first.getCount() > 50 && norm ? (double) it.second.score / norm * 125 : 0);
//        if (Q.size() < count || Q.size() == count && evaluation < Q.top().first) {
//            Q.emplace(evaluation, &it);
//            if (Q.size() > count)
//                Q.pop();
//        }
//    }
//    unordered_map<Board, ResultType> nMap;
//    nMap.reserve(Q.size());
//    while (!Q.empty()) {
//        nMap.emplace(*Q.top().second);
//        //Q.top().second->first.print(), fflush(stdout);
//        Q.pop();
//    }
//    oMap = move(nMap);
//}

void Solver::filterHighEvaluation(unordered_map<Board, ResultType> &oMap, int count) const {
    if (count >= oMap.size())
        return;
    struct item {
        double v;
        decltype(oMap.cbegin()) it;

        inline bool operator<(const item &t) const {
            return v < t.v;
        }
    };

    priority_queue<item> Q;
    for (auto it = oMap.cbegin(); it != oMap.cend(); it++) {
        auto evaluation =
                BoardEvaluator::evaluate(it->first) - it->second.score * scoreWeight;
        if (Q.size() < count || Q.size() == count && evaluation < Q.top().v) {
            Q.emplace(item{evaluation, it});
            if (Q.size() > count)
                Q.pop();
        }
    }
    unordered_map<Board, ResultType> nMap;
    nMap.reserve(Q.size());
    while (!Q.empty()) {
        nMap.insert(oMap.extract(Q.top().it));
        Q.pop();
    }
    oMap = move(nMap);
}

ResultType Solver::solve() {
    Board board;
    auto r = greedyForScore(0, (int) brickSeq.size(), board);
    return r;
}

void Solver::setBrickSeq(const vector<Brick> &t) {
    brickSeq = t;
}

void Solver::setBrickSeq(vector<Brick> &&t) {
    brickSeq = move(t);
}

ResultType Solver::greedyForScore(int first, int last, Board &board) {
    unordered_map<Board, ResultType> boardMap, tempMap;
    boardMap[board];
    for (int i = first; i < last; i++) {
        generateChoices(brickSeq[i], boardMap, tempMap);
        boardMap.clear();
        tempMap.swap(boardMap);
    }
    ResultType t{StringPtr(), -1};
    for (auto &item:boardMap)
        if (item.second.score > t.score)
            t = item.second, board = item.first;
    return t;
}

void Solver::generateChoices(const Brick &brick, const unordered_map<Board, ResultType> &originMap,
                             unordered_map<Board, ResultType> &targetMap) {
    unordered_map<Board, ResultType> tempMap;
    tempMap.reserve(reservedBoardCount * 10);

    for (const auto &item:originMap) {
        TimerUtil::start("expand");
        expand(brick, item.first, item.second, targetMap);
        TimerUtil::finish("expand");
    }
    cout << targetMap.size() << endl;
    filterHighEvaluation(targetMap, reservedBoardCount);
}


