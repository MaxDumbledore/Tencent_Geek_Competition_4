//
// Created by 40461 on 2021/8/8.
//

#include "String.h"
#include <vector>

string String::getContent() {
    StringPtr t = prev;
    vector<string> v{s};
    while (t != nullptr) {
        v.emplace_back(t->s);
        t = t->prev;
    }
    reverse(v.begin(), v.end());
    string ans;
    for (auto &i:v)
        ans += i;
    return ans;
}

StringPtr String::firstItem() {
    StringPtr t = shared_from_this();
    while (t->prev != nullptr)
        t = t->prev;
    return t;
}
