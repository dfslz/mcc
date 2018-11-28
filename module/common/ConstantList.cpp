#include "ConstantList.h"

template<class T>
void ConstantList<T>::set(int position, T value) {
    list[position] = value;
}

template<class T>
T ConstantList<T>::get(int position) {
    return list[position];
}

template<class T>
int ConstantList<T>::insert(T value) {
    set(pos, value);
    return pos++;
}

