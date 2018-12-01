#include <iostream>
#include <string>
#include "Common.h"
#include "Scanner.h"
#include "SymbolList.h"

using namespace std;

int main(int argc, char** argv) {
    if(argc < 2) {
         cout << "please input target file";
         return 0;
    }

    fin.open(argv[1], std::ios_base::in);

    while(!Buffer::isFileEnd()) {
        Token tk = scanner.next();
        cout << tk.getOffset() << endl;
    }
    return 0;
}