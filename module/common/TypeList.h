#ifndef _TYPELIST_H
#define _TYPELIST_H

class TypeList {
public:
    enum TypeValue {//类型
        integer, real, ch, boolean, array
    };

    void setType(int position, TypeValue type);
    void setOffset(int position, int offset);
    TypeValue getType(int position);
    void insert(TypeValue type, int offset);
private:
    const static int _listMax = 1e5;
    static int pos;

    struct data {
        TypeValue _type;
        int _offset;
    };
    static data list[ _listMax ];
};

#endif

