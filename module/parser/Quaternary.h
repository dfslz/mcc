#ifndef _QUATERNARY_H
#define _QUATERNARY_H

#include "Common.h"

class Quaternary {
public:
    enum Option{
        add, sub, mul, div, equal
    };

    struct Quat {
        Option _option;
        Token _fisrt, _second, _target;
    };

    Quaternary() { _size = 0; }
    int insert(Option option, Token first, Token second, Token target);
    Quat get(int position);
    int size();
private:
    const static int _quat_max = 1e6;
    int _size;
    static Quat list[_quat_max];
};

//extern Quaternary quater;

#endif
