#include "Token.h"

void Token::setOffset(int offset) {
    _offset = offset;
}

void Token::setCategory(SymbolList::Categories category) {
    _category = category;
}

SymbolList::Categories Token::getCategory() {
    return _category;
}

int Token::getOffset() {
    return _offset;
}

