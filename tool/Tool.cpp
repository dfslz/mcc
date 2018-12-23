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
        cout << synbl.getName(tk.getOffset());
        if(synbl.getCategory(tk.getOffset()) == SymbolList::array) {
            cout << "[";
            if(tk.getPosition() >= 0) cout << intList.get(tk.getPosition());
            else cout << synbl.getName(-tk.getPosition());
            cout << "]";
        }
        break;
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
        printToken1(qt.getFirst()); cout << ", ";
        printToken1(qt.getSecond()); cout <<  ", ";
        printToken1(qt.getTarget()); cout << " ";
        cout << ")" << endl;
    }
}

void help() {
    std::cout << "-dt\t输出扫描器获得的token" << endl
              << "-dq\t输出语法语义分析后获得的四元式序列" << endl
              << "-dqs\t输出四元式优化后的四元式序列" << endl
              << "请按照\n\tmcc infile outfile -parameter\n格式来使用" << endl;
    exit(0);
}

void processParameter(int argc, char** argv) {
    display_quaterList = display_quaterList_simplified = display_token = false;
    if(argc < 3) {
        help();
    }
    int a = 0;
    for(int i = 1; i < argc; ++i) {
        if(argv[i][0] == '-') {//命令
            if(string(argv[i]) == "-dt") {//输出token
                display_token = true;
            } else if(string(argv[i]) == "-dq") {
                display_quaterList = true;
            } else if(string(argv[i]) == "-dqs") {
                display_quaterList_simplified = true;
            } else {
                std::cout << "不能识别的参数: " << argv[i] << endl;
                help();
            }
        } else {
            a++;
            if(a == 1) {
                fin.open(argv[i], std::ios_base::in);
                if(!fin) {
                    cout << "cannot open file: " << argv[i] << endl;
                    exit(0);
                }
            } else if(a == 2) {
                fout.open(argv[i], std::ios_base::out);
            } else {
                cout << "参数过多: " << argv[i] << endl;
                exit(0);
            }
        }
    }
    if(a != 2) help();
}