//
// Created by 40461 on 2021/8/3.
//

#ifndef TETRISSOLVER_TIMERUTIL_H
#define TETRISSOLVER_TIMERUTIL_H

#include <ctime>
#include <map>
#include <string>

using namespace std;

/**
 * 一个比较粗略的计时器，注意其中map的耗时对准确性的影响
 */

//Caution! Don't take string object as parameter! only literal string is recommended!

class TimerUtil {
public:
    static void start(string_view name);

    static void finish(string_view name);

    static double getElapsedTime(string_view name);

    static void printAllTime();

private:
    inline static map<string_view , pair<clock_t, double>> timer;
};

#endif //TETRISSOLVER_TIMERUTIL_H
