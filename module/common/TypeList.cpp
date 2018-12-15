#include "TypeList.h"

int TypeList::_size = 3;
TypeList::data TypeList::list[TypeList::_listMax] = {};

TypeList::TypeList() {
    setType(0, "int"); setOffset(0, 4);
    setType(1, "float"); setOffset(1, 4);
    setType(2, "char"); setOffset(2, 1);
}

void TypeList::setType(int position, std::string type) {
    list[position]._type = type;
}

void TypeList::setOffset(int position, int offset) {
    list[position]._offset = offset;
}

std::string TypeList::getType(int position) {
    return list[position]._type;
}

void TypeList::insert(std::string type, int offset) {
    setType(_size, type);
    setOffset(_size, offset);
    ++_size;
}

int TypeList::getOffset(int position) {
    return list[position]._offset;
}

int TypeList::find(std::string type) {
    for(int i = 0; i < _size; ++i) {
        if(type == getType(i)) return i;
    }
    return -1;
}

