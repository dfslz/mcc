#ifndef _PARSER_H
#define _PARSER_H

#include "Common.h"
#include "Scanner.h"
#include "Tool.h"
#include "Error.h"

class Parser {
public:
    bool parse();//分析一个句子,返回是否合法并将合法句子生成四元式

private:
    Token tk, type, target;
    std::string option;
    bool ok;
    void sentence();
    Token expression();
    void content();
    void exp_bhv();
    bool isConstantOrIdentify(Token::Categories cat);
};

extern Parser parser;

#endif
