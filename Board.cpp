//
// Created by 40461 on 2021/8/2.
//

#include "Board.h"
#include <cstring>

//如果是windows x64平台，使用_mm_popcnt系列，否则在GCC上使用 __builtin_popcount系列

#ifndef _M_X64
#define _mm_popcnt_u32 __builtin_popcount
#define _mm_popcnt_u64 __builtin_popcountll
#else

#include <intrin.h>
#include "TimerUtil.h"

#endif

//预处理出一行的Transition数目，加快getRowTransition的效率

constexpr int TransitionForSingleRow[1 << M] = {
        2, 2, 4, 2, 4, 4, 4, 2, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6,
        6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6,
        8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4,
        4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6,
        6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6,
        6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 8, 8, 10, 8, 8, 8, 8, 6, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4,
        6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6,
        4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6,
        6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 8, 8, 10, 8, 8, 8, 8, 6, 6,
        6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 8, 8, 10, 8, 8, 8, 8, 6,
        8, 8, 10, 8, 10, 10, 10, 8, 8, 8, 10, 8, 8, 8, 8, 6, 6, 6, 8, 6, 8, 8, 8, 6, 8, 8, 10, 8, 8, 8, 8, 6, 6, 6, 8,
        6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 6,
        6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 8, 8, 10, 8, 8, 8,
        8, 6, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8,
        6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4,
        4, 4, 6, 4, 4, 4, 4, 2, 2, 2, 4, 2, 4, 4, 4, 2, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4,
        4, 4, 2, 4, 4, 6, 4, 6,
        6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4,
        4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4,
        4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4,
        4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6,
        6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 8, 8, 10, 8, 8, 8, 8, 6, 6, 6, 8, 6,
        8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6,
        6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4,
        4, 2, 2, 2, 4, 2, 4, 4, 4, 2, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6,
        4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4,
        6, 6, 8, 6, 6, 6, 6, 4, 6, 6, 8, 6, 8, 8, 8, 6, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6,
        6, 6, 4, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 4, 2, 2, 2, 4, 2, 4, 4, 4, 2, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4,
        6, 4, 6, 6, 6, 4, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 6, 6, 8, 6, 6, 6, 6, 4, 4, 4, 6, 4, 6, 6, 6,
        4, 4, 4, 6, 4, 4, 4, 4, 2, 2, 2, 4, 2, 4, 4, 4, 2, 4, 4, 6, 4, 4, 4, 4, 2, 4, 4, 6, 4, 6, 6, 6, 4, 4, 4, 6, 4,
        4, 4, 4, 2, 2, 2, 4, 2, 4, 4, 4, 2, 4, 4, 6, 4, 4, 4, 4, 2, 2, 2, 4, 2, 4, 4, 4, 2, 2, 2, 4, 2, 2, 2, 2, 0
};

Board::Board() : state() {}

const uint64_t *Board::getState() const {
    return state;
}

bool Board::operator==(const Board &t) const {
    return state[0] == t.state[0] && state[1] == t.state[1] && state[2] == t.state[2] && state[3] == t.state[3];
}

bool Board::operator<(const Board &t) const {
    for (int i = 0; i < 4; i++)
        if (state[i] < t.state[i])
            return true;
        else if (state[i] > t.state[i])
            return false;
    return false;
}

//设置第x行第y列被占用
void Board::set(int x, int y) {
    state[x / 5] |= (uint64_t) 1 << (y + x % 5 * M);
}

//查询第x行第y列值
bool Board::query(int x, int y) const {
    return state[x / 5] >> (y + x % 5 * M) & 1;
}

//取出第index行
uint32_t Board::rowAt(int index) const {
    return state[index / 5] >> (index % 5 * M) & FULL_ROW;
}

//返回最高的已有方格的高度
int Board::getMaxHeight() const {
    uint32_t s = FULL_ROW;
    for (int i = N - 1; i >= 0; i--) {
        s &= rowAt(i);
        if (!s)
            return N - 1 - i;
    }
    return N;
}

//返回行方向上的变换次数，注释的内容TransitionForSingleRow等价
int Board::getRowTransition() const {
    int rowTransition = 0;
    for (int i = 0; i < N; i++) {
//        bool lastGrid = true;
//        for (int j = 0; j < M; j++) {
//            if (query(i, j) != lastGrid) {
//                ++rowTransition;
//                lastGrid = !lastGrid;
//            }
//        }
//        if (!lastGrid)
//            ++rowTransition;
        rowTransition += TransitionForSingleRow[rowAt(i)];
    }
    return rowTransition;
}

//返回列方向上的变换次数
int Board::getColumnTransition() const {
    int columnTransition = 0;
    uint32_t curRow, nextRow = rowAt(0);
    for (int i = 0; i < N; i++) {
        curRow = nextRow;
        nextRow = (i + 1 == N ? FULL_ROW : rowAt(i + 1));
        columnTransition += _mm_popcnt_u32(curRow ^ nextRow);
    }
    return columnTransition;
}

//返回洞的个数
int Board::getNumberOfHoles() const {
    uint32_t holes = 0;
    int numberOfHoles = 0;
    for (int i = 0; i < N; i++) {
        holes = ~rowAt(i) & holes;
        numberOfHoles += _mm_popcnt_u32(holes);
        holes |= rowAt(i);
    }
    return numberOfHoles;
}

//返回井的深度和
int Board::getWellSums() const {
    //t记录某一列的上方有多少个井，列j处新出现一个空格那么给深度和的贡献量就是t[j]
    int t[M] = {}, wellSums = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++)
            if (query(i, j))
                t[j] = 0;
            else {
                if ((!j || query(i, j - 1)) && (j + 1 == M || query(i, j + 1)))
                    ++t[j];
                wellSums += t[j];
            }
    }
    return wellSums;
}

//删除所有的满列
int Board::removeFullRow() {
    //TimerUtil::start(__func__ );
    int pos = N;
    for (int i = N - 1; i >= 0; i--) {
        auto t = rowAt(i);
        if (t == FULL_ROW)
            continue;
        setRow(--pos, t);
    }
    for (int i = 0; i < pos; i++)
        setRow(i, 0);
    //TimerUtil::finish(__func__ );
    return pos;
}

//设置index列为整数val
void Board::setRow(int index, uint32_t val) {
    int t = index % 5 * M;
    uint64_t mask = ~((uint64_t) FULL_ROW << t);
    state[index / 5] &= mask;
    state[index / 5] |= (uint64_t) val << t;
}

//获得已有格子数
int Board::getCount() const {
    int count = 0;
    for (auto &i:state)
        count += (int) _mm_popcnt_u64(i);
    return count;
}

//打印
void Board::print() const {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            putchar(query(i, j) ? '1' : '0');
            putchar(j + 1 == M ? '\n' : ',');
        }
    putchar('\n');
    fflush(stdout);
}

//从vector<vector<int>>加载Board
Board::Board(const vector<vector<int>> &b) : state() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (b[i][j])
                set(i, j);
}




