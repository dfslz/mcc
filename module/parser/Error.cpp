#include "Error.h"
#include <iostream>
#include <cstdlib>

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
    "11.语法错误",
    "12.使用了未定义的变量",
    "13.if小括号不正确",
    "14.if后面大括号不正确",
    "15.while后面小括号不正确",
    "16.while后面的大括号有问题"
};

void err(int code) {
    if(code < 17 && code >= 0) cout << errList[code] << endl;
    else cout << "未定义的错误类型" << endl;
    exit(0);
}