#ifndef _SCANNER_H
#define _SCANNER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../common/Common.h"
#include "Buffer.h"

class Scanner {
public:
    Scanner();
    Token next();

private:
    //常量
    const static int _buf_size = 100;
    const static int _trans_table_raw = 15;
    const static int _trans_table_column = 10;
    const static int state_begin = 0;
    const static int state_end = 8;
    
    std::string word;//存放扫描到的合法串
    Buffer buffer;//缓冲区

    static int stateTransTable[ _trans_table_raw ][ _trans_table_column ];
};

const Scanner scanner;

#endif

