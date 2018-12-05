#ifndef _PARSER_H
#define _PARSER_H

#include "Common.h"
#include "Scanner.h"
#include "Quaternary.h"

class Parser {
public:
    bool analyze();//分析一个句子,返回是否合法并将合法句子生成四元式
private:
    Token tk;
    void sentence();
    void expression();
};

extern Parser parser;

#endif
