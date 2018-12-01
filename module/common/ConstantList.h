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

#endif

