#include "Buffer.h"

void Buffer::next() {
    ++_bp;
    if(buf[_bp] == '\0') {
        if(fin >> buf) {
            _bp = 0;
        }
    }
}

char Buffer::get() {
    return buf[_bp];
}

bool Buffer::isFileEnd() {
    if(get() == '\0') return true;
    return false;
}