#include "Buffer.h"

int Buffer::_bp = 0;
std::string Buffer::buf;

void Buffer::next() {
    ++_bp;
    if(buf[_bp] == '\0') {
        if(fin >> buf) {
            _bp = 0;
        }
    }
}

char Buffer::get(int bp, std::string buff) {
    return buff[bp];
}

bool Buffer::isFileEnd() {
    if(get() == '\0') return true;
    return false;
}
