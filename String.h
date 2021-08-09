//
// Created by 40461 on 2021/8/8.
//

#ifndef TETRISSOLVER_STRING_H
#define TETRISSOLVER_STRING_H

#include <string>
#include <memory>

using namespace std;

class String;

using StringPtr = shared_ptr<String>;

struct String : enable_shared_from_this<String> {
    string s;
    StringPtr prev;

    string getContent();

    StringPtr firstItem();
};

#endif //TETRISSOLVER_STRING_H
