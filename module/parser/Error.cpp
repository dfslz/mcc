#include "Error.h"
#include <iostream>

using namespace std;

string errList[] = {
    "0.函数没有返回值类型",
    "1.错误的函数名" ,
    "2.函数没有以大括号开头", 
    "3.函数没有以大括号结尾",
    "4.错误的变量名",
    "5.重定义变量",
    "6.未定义的类型",
    "7.没有操作符",
    "8.不能识别的符号",
    "9.不是操作符",
    "10.非法符号",
    "11.语法错误"
};

void err(int code) {
    cout << errList[code] << endl;
}