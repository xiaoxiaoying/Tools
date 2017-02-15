//
// Created by Administrator on 2016/10/27/027.
//

#ifndef TEST4_STRTO_H
#define TEST4_STRTO_H
using namespace std;

#include <string>
#include <sstream>

template<typename T>
std::string to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}

#endif //TEST4_STRTO_H
