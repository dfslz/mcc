#ifndef _BUFFER_H
#define _BUFFER_H

#include "../common/Common.h"

class Buffer {
public:
    Buffer() { _bp = 0; buf[0] = '\0'; next(); };
    void next();
    static char get(int bp = _bp, std::string buff = buf);
    static bool isFileEnd();

private:
    static int _bp;
    static std::string buf;
};

#endif
