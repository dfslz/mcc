#include "Token.h"

void Token::setOffset(int offset) {
    _offset = offset;
}

void Token::setCategory(Token::Categories category) {
    _category = category;
}

Token::Categories Token::getCategory() {
    return _category;
}

int Token::getOffset() {
    return _offset;
}

