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
    set(_size, value);
    return _size++;
}

template<class T>
ConstantList<T>::ConstantList(int size, T* arr) {
    for(int i = 0; i < size; ++i) {
        this->insert(arr[i]);
    }
}

template<class T>
int ConstantList<T>::size() {
    return _size;
}
