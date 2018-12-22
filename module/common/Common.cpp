#include "Common.h"

//目标文件输入流
std::ifstream fin;

//常数表
ConstantList<int> intList;
ConstantList<float> floatList;
ConstantList<char> charList;
ConstantList<std::string> strList;

//长度表
ConstantList<int> len;

//符号表
SymbolList synbl;

//类型表
TypeList typeList;

//关键字表
const int kwd_num = 10;
std::string kwd[kwd_num] = {
    "int",
    "float",
    "char",
    "void",
    "string",
    "if",
    "else",
    "while",
    "for",
    "do"
};
ConstantList<std::string> keywordList(kwd_num, kwd);

//界符和运算符表
const int op_num = 19;
std::string opt[op_num] = {
    "+", "-", "*", "/", "=", "==",
    ">", "<", ">=", "<=", "(", ")", "{", "}", ";", ",", "!=", "[", "]"
};
ConstantList<std::string> opList(op_num, opt);

//四元式序列
ConstantList<Quaternary> quaterList;

//数组信息表
ArrayInformation arrayList;