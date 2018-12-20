#include "Tool.h"
#include <iostream>

using namespace std;

int getLength(string type) {
    int len = 0;
    if(type == "int") len = typeList.getOffset(0);
    else if(type == "float") len = typeList.getOffset(1);
    else if(type == "char") len = typeList.getOffset(2);
    else len = -1;
    
    return len;
}

void printToken(Token tk) {
    switch(tk.getCategory()) {
    case Token::keyword:
        cout << "keyword: " << keywordList.get(tk.getOffset()); break;
    case Token::id:
        cout << "id: " << synbl.getName(tk.getOffset()); break;
    case Token::symbol:
        cout << "symbol: " << opList.get(tk.getOffset()); break;
    case Token::integer:
        cout << "integer: " << intList.get(tk.getOffset()); break;
    case Token::real:
        cout.setf(ios_base::fixed);
        cout.precision(8);
        cout << "float: " << floatList.get(tk.getOffset()); break;
    case Token::ch:
        cout << "ch: " << charList.get(tk.getOffset()); break;
    case Token::str:
        cout << "str: " << strList.get(tk.getOffset()); break;
    default:
        cout << "can not recognize Token type";
    }

    cout << endl;
}

void printToken1(Token tk) {
    switch(tk.getCategory()) {
    case Token::keyword:
        cout << keywordList.get(tk.getOffset()); break;
    case Token::id:
        cout << synbl.getName(tk.getOffset()); break;
    case Token::symbol:
        cout << opList.get(tk.getOffset()); break;
    case Token::integer:
        cout << intList.get(tk.getOffset()); break;
    case Token::real:
        cout.setf(ios_base::fixed);
        cout.precision(8);
        cout << floatList.get(tk.getOffset()); break;
    case Token::ch:
        cout << charList.get(tk.getOffset()); break;
    case Token::str:
        cout << strList.get(tk.getOffset()); break;
    default:
        cout << "_";
    }
}

void printQuaterList() {
    for(int i = 0; i < quaterList.size(); ++i) {
        Quaternary qt = quaterList.get(i);
        cout << "( " <<  qt.getOption() << ", ";
        printToken1(qt.getFirst()); cout << " " << ", ";
        printToken1(qt.getSecond()); cout << " " <<  ", ";
        printToken1(qt.getTarget()); cout << " ";
        cout << ")" << endl;
    }
}
