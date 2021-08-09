//
// Created by 40461 on 2021/8/3.
//

#include "TimerUtil.h"
#include <iostream>

//void TimerUtil::start() {
//    sta = clock();
//}
//
//void TimerUtil::finish() {
//    t += (double) (clock() - sta) / CLOCKS_PER_SEC;
//}
//
//double TimerUtil::getElapsedTime() {
//    return t;
//}

void TimerUtil::start(string_view name) {
    timer[name].first = clock();
}

void TimerUtil::finish(string_view name) {
    auto &item = timer[name];
    item.second += (double) (clock() - item.first) / CLOCKS_PER_SEC;
}

double TimerUtil::getElapsedTime(string_view name) {
    return timer[name].second;
}

void TimerUtil::printAllTime() {
    for (auto &[name, item]:timer)
        cout << name << ": " << item.second << endl;
}
