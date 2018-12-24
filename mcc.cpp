#include <iostream>
#include <string>
#include "Common.h"
#include "Scanner.h"
#include "SymbolList.h"
#include "Parser.h"
#include "Tool.h"
#include "DAG.h"
#include "Translate.h"

using namespace std;


int main(int argc, char** argv) {
    processParameter(argc, argv);
    synbl.pushLoacle();

    parser.parse();
    if(display_quaterList) std::cout << "产生的四元式:\n",printQuaterList();

    DAG tmp;
    while(!tmp.getPrepareForEnd()) {
        tmp.simplify();
        tmp.finalQuaterList();
        tmp.setActivityList();
    }
    if(display_quaterList_simplified) std::cout << "\n优化后的四元式\n",printQuaterList();

    translate();

    fin.close();
    return 0;
}