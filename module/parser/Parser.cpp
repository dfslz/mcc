#include "Parser.h"

Parser parser;

bool Parser::parse() {
    tk = scanner.next();
    ok = true;

    Token returnType;//函数返回值类型
    std::string functionName;
    if(tk.getCategory() != Token::keyword) {
        //error:函数没有返回值
        ok = false;
    } else {
        //记录函数返回值类型
        returnType = tk;
    }

    tk = scanner.next();
    
    if(tk.getCategory() != Token::id) {
        //error: 函数名不正确
        ok = false;
    } else {
        //记录函数名
        functionName = synbl.getName(tk.getOffset());
    }

    
    return ok;
}

void Parser::sentence() {

}

void Parser::expression() {
    
}

void Parser::content() {

}