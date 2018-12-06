#include "Quaternary.h"

//Quaternary quater;

Quaternary::Quat Quaternary::list[] = {};

int Quaternary::insert(Option option, Token first, Token second, Token target) {
    list[_size]._option = option;
    list[_size]._fisrt = first;
    list[_size]._second = second;
    list[_size]._target = target;
    ++_size;
}

Quaternary::Quat Quaternary::get(int position) {
    return list[position];
}

int Quaternary::size() {
    return _size;
}