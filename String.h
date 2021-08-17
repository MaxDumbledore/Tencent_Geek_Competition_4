//
// Created by 40461 on 2021/8/8.
//

#ifndef TETRISSOLVER_STRING_H
#define TETRISSOLVER_STRING_H

#include <string>
#include <memory>

using namespace std;

/**
 * String类中包含StringPtr指针，用于记录前驱，在状态转移的过程中一个状态不必记录完整的字符串，可以和其他状态大量共用前驱，节省内存
 */

class String;

using StringPtr = shared_ptr<String>;

struct String : enable_shared_from_this<String> {
    string s;
    StringPtr prev;

    string getContent();

    StringPtr firstItem();
};

#endif //TETRISSOLVER_STRING_H
