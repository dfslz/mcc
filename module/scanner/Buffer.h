#ifndef _BUFFER_H
#define _BUFFER_H

#include "Common.h"

class Buffer {
public:
    Buffer() { _bp = 0; buf[0] = '\0'; };
    static void next();
    static char get();
    static bool isFileEnd();

private:
    const static int _buf_size = _id_len << 6;
    static int _bp;
    static char buf[ _buf_size ];
};

#endif
