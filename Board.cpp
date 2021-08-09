//
// Created by 40461 on 2021/8/2.
//

#include "Board.h"
#include <cstring>

#ifndef _M_X64
#define _mm_popcnt_u32 __builtin_popcount
#define _mm_popcnt_u64 __builtin_popcountll
#else

#include <intrin.h>
#include "TimerUtil.h"

#endif

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

void Board::set(int x, int y) {
    state[x / 5] |= (uint64_t) 1 << (y + x % 5 * M);
}


bool Board::query(int x, int y) const {
    return state[x / 5] >> (y + x % 5 * M) & 1;
}

uint32_t Board::rowAt(int index) const {
    return state[index / 5] >> (index % 5 * M) & FULL_ROW;
}

int Board::getMaxHeight() const {
    uint32_t s = FULL_ROW;
    for (int i = N - 1; i >= 0; i--) {
        s &= rowAt(i);
        if (!s)
            return N - 1 - i;
    }
    return N;
}

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
        rowTransition += transition[rowAt(i)];
    }
    return rowTransition;
}

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

int Board::getWellSums() const {
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

void Board::setRow(int index, uint32_t val) {
    int t = index % 5 * M;
    uint64_t mask = ~((uint64_t) FULL_ROW << t);
    state[index / 5] &= mask;
    state[index / 5] |= (uint64_t) val << t;
}

int Board::getCount() const {
    int count = 0;
    for (auto &i:state)
        count += (int) _mm_popcnt_u64(i);
    return count;
}

void Board::print() const {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++) {
            putchar(query(i, j) ? '1' : '0');
            putchar(j + 1 == M ? '\n' : ',');
        }
    putchar('\n');
    fflush(stdout);
}




