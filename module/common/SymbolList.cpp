#include "SymbolList.h"

//初始化静态变量
int SymbolList::_position = 0;
SymbolList::data SymbolList::list[ SymbolList::_listMax ] = {};

void SymbolList::setName(int position, std::string name)
{
    list[position]._name = name;
}

void SymbolList::setType(int position, int type)
{
    list[position]._type = type;
}

void SymbolList::setCategory(int position, SymbolList::Categories category)
{
    list[position]._category = category;
}

void SymbolList::setOffset(int position, int offset)
{
    list[position]._offset = offset;
}

int SymbolList::find(std::string name)
{
    int res = -1;
    for (int pos = 0; pos < _position; ++pos)
    {
        if (list[pos]._name == name)
        {
            res = pos;
            break;
        }
    }
    return res;
}

int SymbolList::insert(std::string name)
{
    int res = find(name);
    if (res == -1)
    {
        list[_position]._name = name;
        res = _position;
        ++_position;
    }
    return res;
}

std::string SymbolList::getName(int position)
{
    return list[position]._name;
}

int SymbolList::getType(int position)
{
    return list[position]._type;
}

SymbolList::Categories SymbolList::getCategory(int position)
{
    return list[position]._category;
}
