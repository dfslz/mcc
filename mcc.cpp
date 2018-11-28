#include <iostream>
#include <string>
#include "common.h"
#include "scanner.h"
#include "SymbolList.h"

int main() {
    synbl.insert("1234");
    std::cout << synbl.getName(synbl.find("1234")) << std::endl;
}