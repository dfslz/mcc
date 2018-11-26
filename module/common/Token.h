#ifndef _TOKEN_H
#define _TOKEN_H

#include "SymbolList.h"

class Token {
public:
    //setter
    void setCategory(SymbolList::Categories category);
    void setOffset(int offset);
    //getter
    SymbolList::Categories getCategory();
    int getOffset();

private:
    SymbolList::Categories _category;
    int _offset;
};

#endif

