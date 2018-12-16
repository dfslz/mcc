#ifndef _TYPELIST_H
#define _TYPELIST_H

#include <iostream>

class TypeList {
public:
    //enum TypeValue {//类型,需要定义成字符串,这里得改,为了方便利用关键字查找对于的类型
    //    int, float, char, bool, array
    //};

    TypeList();
    void setType(int position, std::string type);
    void setOffset(int position, int offset);
    std::string getType(int position);
    int getOffset(int position);
    void insert(std::string type, int offset);
    int find(std::string type);
private:
    const static int _listMax = 1e5;
    static int _size;

    struct data {
        std::string _type;
        int _offset;
    };
    data list[ _listMax ];
};

#endif

