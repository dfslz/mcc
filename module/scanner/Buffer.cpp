#include "Buffer.h"

int Buffer::_bp = 0;
char Buffer::buf[] = "";

void Buffer::next() {
    if(get() == '\0') {
        if(fin.getline(buf, _buf_size)) {
            _bp = 0;
        }
    } else {
        ++_bp;
    }
}

char Buffer::get() {
    return buf[_bp];
}

bool Buffer::isFileEnd() {
    if(get() == '\0') next();//排除未初始化的情况
    if(get() == '\0') return true;
    return false;
}
