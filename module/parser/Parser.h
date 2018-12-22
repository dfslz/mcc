#ifndef _PARSER_H
#define _PARSER_H

#include "Common.h"
#include "Scanner.h"
#include "Tool.h"
#include "Error.h"

class Parser {
public:
    void parse();//分析一个句子,返回是否合法并将合法句子生成四元式

private:
    Token tk, type, target;
    std::string option;
    bool sentence1;//用于决定sentence是否能够递归扩充

    bool ok;
    void sentence();
    Token expression();
    void content();
    void exp_bhv();
    void jmp();
    void whloop();
    void forloop();
    void dowhloop();
    bool isConstantOrIdentify(Token::Categories cat);
    Token getTarget(int type);
};

extern Parser parser;

#endif
