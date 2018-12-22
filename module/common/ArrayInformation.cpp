#include "ArrayInformation.h"

int ArrayInformation::_position = 0;

void ArrayInformation::setLength(int position, int length) {
    _list[position]._length = length;
}

void ArrayInformation::setSize(int position, int size) {
    _list[position]._size = size;
}

void ArrayInformation::setType(int position, int type) {
    _list[position]._type = type;
}

int ArrayInformation::getLength(int position) {
    return _list[position]._length;
}

int ArrayInformation::getType(int position) {
    return _list[position]._type;
}

int ArrayInformation::getSize(int position) {
    return _list[position]._size;
}

int ArrayInformation::insert(int length, int type, int size) {
    _list[_position]._length = length;
    _list[_position]._type = type;
    _list[_position]._size = size;
    return _position++;
}