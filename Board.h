//
// Created by 40461 on 2021/8/2.
//

#ifndef TETRISSOLVER_BOARD_H
#define TETRISSOLVER_BOARD_H

#include <cstdint>
#include <algorithm>

inline const int N = 20, M = 10;
inline const uint32_t FULL_ROW = (1 << M) - 1;

class Board {
public:
    Board();

    [[nodiscard]] const uint64_t *getState() const;

    bool operator==(const Board &t) const;

    bool operator<(const Board &t) const;

    void set(int x, int y);

    void setRow(int index, uint32_t val);

    [[nodiscard]] bool query(int x, int y) const;

    [[nodiscard]] uint32_t rowAt(int index) const;

    [[nodiscard]] int getMaxHeight() const;

    [[nodiscard]] int getRowTransition() const;

    [[nodiscard]] int getColumnTransition() const;

    [[nodiscard]] int getNumberOfHoles() const;

    [[nodiscard]] int getWellSums() const;

    [[nodiscard]] int getCount() const;

    int removeFullRow();

    void print() const;

private:
    uint64_t state[4];
};

inline uint64_t splitMix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

namespace std {
    template<>
    struct hash<Board> {
        size_t operator()(const Board &b) const {
            auto t = b.getState();
            return (size_t) (splitMix64(t[0]) ^ splitMix64(t[1]) ^ splitMix64(t[2]) ^ splitMix64(t[3]));
        }
    };
}


#endif //TETRISSOLVER_BOARD_H
