#include "Quaternary.h"

void Quaternary::setOption(std::string option) {
    _option = option;
}

void Quaternary::setFirst(Token token) {
    _first = token;
}

void Quaternary::setSecond(Token token) {
    _second = token;
}

void Quaternary::setTarget(Token token) {
    _target = token;
}

std::string Quaternary::getOption() {
    return _option;
}

Token Quaternary::getFirst() {
    return _first;
}

Token Quaternary::getSecond() {
    return _second;
}

Token Quaternary::getTarget() {
    return _target;
}