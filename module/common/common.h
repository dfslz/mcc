#ifndef _COMMON_H
#define _COMMON_H

#include "SymbolList.h"
#include "Token.h"
#include "TypeList.h"
#include "ConstantList.h"

//id最大长度
const int _id_len = 100;

//各类常量表
extern ConstantList<int> intList;
extern ConstantList<float> floatList;
extern ConstantList<char> charList;
extern ConstantList<char*> strList;

//符号表
extern SymbolList synbl;

//类型表
extern TypeList typelist;

#endif

