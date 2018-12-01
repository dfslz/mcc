#ifndef _SYMBOLLIST_H
#define _SYMBOLLIST_H

#include <string>

class SymbolList {
public:
    enum Categories {
        constant, type, various
    };

    //setter
    void setName(int position, std::string name);
    void setType(int position, int type);
    void setCategory(int position, Categories category);
    void setOffset(int position, int offset);

    //function
    int find(std::string name);
    int insert(std::string name);

    //getter
    std::string getName(int position);
    int getType(int position);
    Categories getCategory(int position);

private:
    const static int _listMax = 1e5;//符号表大小
    static int _position;//当前符号表中未被使用的最低位置

    //存储结构体
    struct data{
        std::string _name;
        int _type;
        Categories _category;
        int _offset;
    };
    static data list[ _listMax ];
};

#endif

