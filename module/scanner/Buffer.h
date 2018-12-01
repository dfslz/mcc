#ifndef _BUFFER_H
#define _BUFFER_H

#include "../common/Common.h"

class Buffer {
public:
    Buffer() { _bp = 0; };
    void next();
    char get();
    bool isFileEnd();

private:
    const static int _size = 100;
    int _bp;
    char buf[ _size ];
};

#endif
