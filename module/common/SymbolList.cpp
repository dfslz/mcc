#include "SymbolList.h"
#include <exception>

//初始化静态变量
int SymbolList::_lv_size[] = {};
int SymbolList::_totalLevel = 0;
int SymbolList::_totalMemory = 0;

void SymbolList::setName(int position, std::string name) {
    list[position]._name = name;
}

void SymbolList::setType(int position, int type) {
    list[position]._type = type;
}

void SymbolList::setCategory(int position, SymbolList::Categories category) {
    list[position]._category = category;
}

void SymbolList::setOffset(int position, int offset) {
    list[position]._offset = offset;
}

void SymbolList::setActivity(int position, bool activity) {
    list[position]._activity = activity;
}

int SymbolList::find(std::string name) {
    try {
        int lv = _locale.top();
        int res = -1;
        for(int i = 0; i < _lv_size[lv]; ++i) {
            if(name == getName(lv*_level_size+i)) return lv*_level_size+i;
        }
        while(lv && res == -1) res = find(name, getSuper(lv)), lv = getSuper(lv);
        return res;
    } catch(std::exception &e) {
        std::cout << "read null stack" << std::endl;
        return -1;
    }
}

int SymbolList::insert(std::string name) {
    int res = find(name, getLocale());//查找当前块中是否存在同名变量
    if (res == -1) {//不存在
        int p = getLocale() * _level_size + _lv_size[getLocale()];
        list[p]._name = name;
        res = p;
        ++_lv_size[getLocale()];
    }
    return res;
}

std::string SymbolList::getName(int position) {
    return list[position]._name;
}

int SymbolList::getType(int position) {
    return list[position]._type;
}

SymbolList::Categories SymbolList::getCategory(int position) {
    return list[position]._category;
}

bool SymbolList::getActivity(int position) {
    return list[position]._activity;
}

int SymbolList::size() {
    int res = 0;
    for(int i = 0; i < _totalLevel; ++i) {
        res += _lv_size[i];
    }
    return res;
}

void SymbolList::pushLoacle() {
    try {
        static int level = 0;
        if(!_locale.empty()) _super[level] = _locale.top();
        else _super[level] = 0;
        _locale.push(level++);
        ++_totalLevel;
    } catch(std::exception &e) {
        std::cout << "read a null stack: _locale" << std::endl;
    }
}

void SymbolList::popLocale() {
    _locale.pop();
}

int SymbolList::getLocale() {
    int res;
    try {
        res = _locale.top();
    } catch(std::exception &e) {
        std::cout << "getLocale: _locale stack is empty()!" << std::endl;
        res = -1;
    }
    return res;
}

int SymbolList::getSuper(int level) {
    return _super[level];
}

int SymbolList::find(std::string name, int level) {
    for(int i = 0; i < _lv_size[level]; ++i) {
        if(getName(level*_level_size+i) == name) return level*_level_size+i;
    }
    return -1;
}

int SymbolList::getLevelSize() {
    return _level_size;
}

int SymbolList::getOffset(int position) {
    return list[position]._offset;
}

int SymbolList::placeVarious(int size) {
    _totalMemory += size;
    return _totalMemory - size;
}