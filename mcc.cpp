#include <iostream>
#include <string>
#include "./module/common/common.h"
#include "./module/scanner/scanner.h"

int main() {
    synbl.insert("1234");
    std::cout << synbl.getName(synbl.find("1234")) << std::endl;
}