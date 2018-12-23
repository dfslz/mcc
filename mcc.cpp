#include <iostream>
#include <string>
#include "Common.h"
#include "Scanner.h"
#include "SymbolList.h"
#include "Parser.h"
#include "Tool.h"
#include "DAG.h"

using namespace std;


int main(int argc, char** argv) {
    processParameter(argc, argv);
    synbl.pushLoacle();

    parser.parse();
    if(display_quaterList) printQuaterList();

    DAG tmp;
    while(!tmp.getPrepareForEnd()) {
        tmp.simplify();
        tmp.finalQuaterList();
        tmp.setActivityList();
    }
    if(display_quaterList_simplified) printQuaterList();

    fin.close();
    return 0;
}