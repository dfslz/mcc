#ifndef _COMMON_H
#define _COMMON_H

#include <iostream>
#include <fstream>
#include "SymbolList.h"
#include "Token.h"
#include "TypeList.h"
#include "ConstantList.h"
#include "Quaternary.h"
#include "ArrayInformation.h"

//参数命令控制
extern bool display_token;
extern bool display_quaterList;
extern bool display_quaterList_simplified;

//目标文件输入流
extern std::ifstream fin;
extern std::ofstream fout;

//id最大长度
const int _id_len = 100;

//各类常量表
extern ConstantList<int> intList;
extern ConstantList<float> floatList;
extern ConstantList<char> charList;
extern ConstantList<std::string> strList;

//长度表
extern ConstantList<int> len;

//符号表
extern SymbolList synbl;

//类型表
extern TypeList typeList;

//关键字表
extern ConstantList<std::string> keywordList;

//界符和运算符表
extern ConstantList<std::string> opList;

//四元式序列
extern ConstantList<Quaternary> quaterList;

//数组信息表
extern ArrayInformation arrayList;

#endif

