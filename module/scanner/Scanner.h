#ifndef _SCANNER_H
#define _SCANNER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "Common.h"
#include "Buffer.h"

class Scanner {
public:
    Scanner() { _state = 0; _type = -1; }
    Token next();

private:
    //常量
    const static int _buf_size = 100;
    const static int _trans_table_raw = 15;
    const static int _trans_table_column = 10;
    const static int stateBegin = 0;
    const static int stateEnd = 8;
    
    std::string word;//存放扫描到的合法串
    Buffer buffer;//缓冲区
    int _state;
    int _type;

    static int stateTransTable[ _trans_table_raw ][ _trans_table_column ];

    bool behavior(char ch);
    void stateTransition(char ch);
    int getOrder(char ch);
    bool issymbol(char ch);
};

extern Scanner scanner;

#endif

