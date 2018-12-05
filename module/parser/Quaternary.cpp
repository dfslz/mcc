#include "Quaternary.h"

Quaternary quater;

Quaternary::Quat Quaternary::list[] = {};

int Quaternary::insert(Option option, std::string first, std::string second, std::string target) {
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