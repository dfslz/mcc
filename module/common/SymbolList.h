#ifndef _SYMBOLLIST_H
#define _SYMBOLLIST_H

#include <iostream>
#include <stack>
#include <string>
#include <utility>

class SymbolList {
public:
    enum Categories {
        label, various, function, null
    };

    //setter
    void setName(int position, std::string name);
    void setType(int position, int type);
    void setCategory(int position, Categories category);
    void setOffset(int position, int offset);
    void setActivity(int position, bool activity);
    void pushLoacle();//生成一个新的局部变量块开始
    void popLocale();//一个局部变量块结束

    //function
    int find(std::string name);//查找当前块及其冲突域中super块变量
    int find(std::string name, int level);//查找指定的某一个块中的变量
    int insert(std::string name);

    //getter
    std::string getName(int position);
    int getType(int position);
    Categories getCategory(int position);
    bool getActivity(int position);
    int getLocale();//得到当前块的level
    int getSuper(int level);//得到level的super块
    int size();

private:
    const static int _listMax = 1e5;//符号表大小
    static int _lv_size[_listMax / 99];//第i层中变量的数量
    static int _totalLevel;
    const static int _level_size = 100;

    //存储结构体
    struct data{
        data() { _category = null; }
        std::string _name;
        int _type;
        Categories _category;
        int _offset;
        bool _activity;
    };
    data list[ _listMax ];
    int _super[_listMax / 100];
    std::stack<int> _locale;
};

#endif

