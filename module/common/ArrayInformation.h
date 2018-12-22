#ifndef _ARRAYINFORMATION_H
#define _ARRAYINFORMATION_H

class ArrayInformation {
public:
    void setLength(int position, int length);
    void setType(int position, int type);
    void setSize(int position, int size);

    int insert(int length, int type, int size);

    int getLength(int position);
    int getType(int position);
    int getSize(int position);
private:
    const static int _array_info_max = 10000;
    static int _position;
    struct data {
        int _length, _type, _size;
    };
    data _list[_array_info_max];
};


#endif
