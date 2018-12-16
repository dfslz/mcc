#include <iostream>
#include <string>
#include "Common.h"
#include "Scanner.h"
#include "SymbolList.h"
#include "Parser.h"
#include "Tool.h"

using namespace std;


int main(int argc, char** argv) {
    if(argc < 2) {
         cout << "please input target file" << endl;
         return 0;
    }

    fin.open(argv[1], std::ios_base::in);
    if(!fin) {
        cout << "not a valide input" << endl;
        return 0;
    }

    int ok = parser.parse();
    if(ok) printQuaterList();

    fin.close();
    return 0;
}