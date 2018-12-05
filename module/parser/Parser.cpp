#include "Parser.h"

Parser parser;

bool Parser::analyze() {
    tk = scanner.next();
    if(tk.getCategory() == Token::id) {
        //TODO 记录子程序id
        tk = scanner.next();
        sentence();
    } else {
        //TODO error:应该输出子程序名
        return false;
    }

    if(tk.getCategory() != Token::symbol 
            || opList.get(tk.getOffset()) == ";") {
        //没有以分号结尾
        return false;
    }
    return true;
}

void Parser::sentence() {

}

void Parser::expression() {
    
}