#include <iostream>
#include <string>
#include "Common.h"
#include "Scanner.h"
#include "SymbolList.h"

using namespace std;

void print(Token& tk) {
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

int main(int argc, char** argv) {
    if(argc < 2) {
         cout << "please input target file";
         return 0;
    }

    fin.open(argv[1], std::ios_base::in);

    Scanner scanner;
    while(!Buffer::isFileEnd()) {
        Token tk = scanner.next();
        print(tk);
    }
    fin.close();
    return 0;
}