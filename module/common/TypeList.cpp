#include "TypeList.h"

int TypeList::pos = 5;
TypeList::data TypeList::list[TypeList::listMax] = {};

void TypeList::setType(int position, TypeList::TypeValue type) {
    list[position]._type = type;
}

void TypeList::setOffset(int position, int offset) {
    list[position]._offset = offset;
}

TypeList::TypeValue  TypeList::getType(int position) {
    return list[position]._type;
}

void TypeList::insert(TypeList::TypeValue type, int offset) {
    setType(pos, type);
    setOffset(pos, offset);
    ++pos;
}

