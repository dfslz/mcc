#ifndef _SCANNER_H
#define _SCANNER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "../common/common.h"


Token scan(FILE* fp);//扫描器,填写符号表并返回token

char _getchar(FILE* fp);//获取buffer中的字符
void scanner_bhv(int& state, char& ch);//自动机动作
void scanner_state_change(int& state, char&ch);//状态转移函数
bool issyn(char& ch);//判断是不是符号
bool file_end(FILE* fp);//判断文件结束并且缓冲区内无字符

#endif

