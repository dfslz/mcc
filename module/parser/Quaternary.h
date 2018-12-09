#ifndef _QUATERNARY_H
#define _QUATERNARY_H

#include "Common.h"

class Quaternary {
public:
    struct Quat {
        std::string _option;
        Token _fisrt, _second, _target;
    };

    Quaternary() { _size = 0; }
    int insert(std::string option, Token first, Token second, Token target);
    Quat get(int position);
    int size();
private:
    const static int _quat_max = 1e6;
    int _size;
    static Quat list[_quat_max];
};


#endif
