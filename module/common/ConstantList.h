#ifndef _CONSTANTLIST_H
#define _CONSTANTLIST_H

template<class T>
class ConstantList {
public:
    ConstantList() { _size = 0; }
    ConstantList(int size, T* arr);
    void set(int position, T value);
    T get(int position);
    int size();
    int insert(T value);
private:
    const static int listMax = 1e5;
    int _size;
    T list[ listMax ];
};

#endif

