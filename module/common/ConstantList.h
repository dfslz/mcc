#ifndef _CONSTANTLIST_H
#define _CONSTANTLIST_H

template<class T>
class ConstantList {
public:
    ConstantList() { pos = 0; }
    void set(int position, T value);
    T get(int position);
    int insert(T value);
private:
    const static int listMax = 1e5;
    int pos;
    T list[listMax];
};

#endif

