#include "Tool.h"
#include <iostream>

using namespace std;

int getLength(TypeList::TypeValue type) {
    int len = 0;
    switch(type) {
    case TypeList::integer:
        len = typeList.getOffset(0);
        break;
    case TypeList::real:
        len = typeList.getOffset(1);
        break;
    case TypeList::ch:
        len = typeList.getOffset(2);
        break;
    default:
        break;
    }
    return len;
}

void printToken(Token& tk) {
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