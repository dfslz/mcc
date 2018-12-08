#include "Parser.h"

Parser parser;

bool Parser::parse() {
    int subProgram;

    if(tk.getCategory() == Token::id) {
        //TODO 生成子程序开头四元式
        subProgram = tk.getOffset();
        tk = scanner.next();
        sentence();
    } else {
        //TODO error:应该输出子程序名
        return false;
    }

    if(tk.getCategory() != Token::symbol 
            || opList.get(tk.getOffset()) != ";") {
        //没有以分号结尾,返回错误1
        return false;
    }
    
    tk = scanner.next();
    if(tk.getCategory != Token::keyword
            || keywordList.get(tk.getOffset()) != "end") {
        //TODO 回到sentence();
    }

    tk = scanner.next();
    if(tk.getCategory != Token::id
            || subProgram != tk.getOffset()) {
        return false;
    }
    //TODO 合法句子,生成结尾四元式
    return true;
}

void Parser::sentence() {

}

void Parser::expression() {
    
}

void Parser::content() {

}