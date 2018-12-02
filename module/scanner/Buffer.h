#ifndef _BUFFER_H
#define _BUFFER_H

#include "../common/Common.h"

class Buffer {
public:
    Buffer() { _bp = 0; buf[1] = '\0'; next(); };
    static void next();
    static char get();
    static bool isFileEnd();

private:
    const static int _buf_size = _id_len << 4;
    static int _bp;
    static char buf[ _buf_size ];
};

#endif
