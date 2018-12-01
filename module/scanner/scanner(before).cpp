

#define START 0
#define END 8

const int scan_conv_table[15][10] = {//状态转移矩阵
    { 7, 1, 7, 6, 7, 9, 8, 0, 10, 12 },
    { 8, 1, 8, 2, 3, 8, 8, 8, 8, 8 },
    { 8, 2, 8, 8, 3, 8, 8, 8, 8, 8 },
    { 8, 5, 8, 8, 8, -1, 4, -1, -1, -1 },
    { 8, 5, 8, 8, 8, -1, 8, -1, -1, -1 },
    { 8, 5, 8, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 2, 8, 8, 8, -1, 8, -1, -1, -1 },
    { 7, 7, 7, 8, 7, 8, 8, 8, 8, 8 },
    { 7, 1, 7, 6, 7, 8, 8, 0, 10, 12 },
    { 8, 8, 8, 8, 8, 9, 8, 8, 8, 8 },
    { 11, 11, 11, 11, 11, 11, 11, 11, -1, 11 },
    { -1, -1, -1, -1, -1, -1, -1, -1, 14, -1 },
    { 13, 13, 13, 13, 13, 13, 13, 13, 13, 14 },
    { 13, 13, 13, 13, 13, 13, 13, 13, 13, 14 },
    { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 }
};


const int _buf_size = _id_len << 1;//buffer大小
int bp = 0;//缓冲区指针

char buf[_buf_size];//缓冲区

//全局变量
int ival, sign, exp, exp_sign;
float fval;
char cval;
char word[ _id_len ];
int wp, type;



//扫描器
Token scan(FILE* fp) {
    Token tk;

    int state = START;
    char ch = ' ';

    ival = 0, fval = 0, sign = 1, exp = 0, exp_sign = 1;
    wp = 0, type = -1;

    while(true) {
        scanner_bhv(state, ch);
        if(state == -1) {
            //TODO err(0) std::cout << "err: 词法错误,存在不能识别的单词\n";
            break;
        }

        if(state == END) break;
        ch = _getchar(fp);
        scanner_state_change(state, ch);
        if(ch == '\0') break;
    }

    //分类处理
    if(type == 0) {//整数
        ival = std::stoi(word);
        tk.setCategory(SymbolList::constant);
        intList.insert(ival);
    } else if(type == 2) {//浮点数
        fval = std::stof(word);
        tk.type = _cf;
        float_list[ flp ] = fval;
        tk.sp = flp++;
    } else if(type == 1) {//字符串,查关键字和标识符表
        for(int i = 0; i < _keyword_size; ++i) {
            if(strcmp(word, keyword[i]) == 0) {
                tk.type = _k;
                tk.sp = i;
            }
        }

        if(tk.type == _none) {
            for(int i = 0; i < sp; ++i) {
                if(strcmp(word, symbol_list[ i ].name) == 0) {
                    //tk.sp._symbol = &symbol_list[ i ];
                    tk.sp = i;
                    tk.type = _i;
                }
            }
            if(tk.type == _none) {
                strcpy(symbol_list[ sp ].name, word);
                tk.sp = sp++;
                tk.type = _i;
            }
        }
    } else if(type == 3) {//界符
        for(int i = 0; i < _syn_size; ++i) {
            if(strcmp(word, syn[ i ]) == 0) {
                tk.type = _p;
                tk.sp = i;
            }
        }
        if(tk.type == _none) err(0);
    } else if(type == 5) {//字符串常量
        tk.type = _cs;
        str_list[ slp ] = new char[strlen(word) - 1];
        strncpy(str_list[ slp ], &word[1], strlen(word) - 2); str_list[ slp ][ strlen(word) - 2 ] = '\0';
        tk.sp = slp++;
    } else if(type == 4) {//字符常量
        tk.type = _cc;
        char_list[ clp ] = word[1];
        tk.sp = clp++;
    } 

    return tk;
}

char _getchar(FILE* fp) {
    if(buf[bp] == '\0' && !feof(fp)) bp = 0;//缓冲区循环
    if(!bp) {
        if(fgets(buf, _buf_size, fp) == NULL) {
            buf[ bp ] = '\0';
        }
    }
    return buf[ (buf[ bp ] == '\0' ? bp : bp++) ];
}

void scanner_bhv(int& state, char& ch) {
    if(state != END) word[ wp++ ] = ch;
    else word[ wp++ ] = '\0';
    if(isspace(ch)) --wp;

    if(state == 1) type = 0;//整形
    else if(state == 7) type = 1;//关键字或标识符
    else if(state == 2 || state == 3) type = 2;//浮点型
    else if(state == 9) type = 3;//符号
    else if(state == 10) type = 4;//字符常量
    else if(state == 12) type = 5;//字符串常量
}

void scanner_state_change(int& state, char& ch) {
    int column;
    if(isalpha(ch)) column = 0;
    else if(isdigit(ch)) column = 1;
    else if(ch == '_') column = 2;
    else if(ch == '.') column = 3;
    else if(ch == 'e' || ch == 'E') column = 4;
    else if(issyn(ch)) column = 5;
    else if(ch == '+') column = 6;
    else if(ch == '\'') column = 8;
    else if(ch == '\"') column = 9;
    else column = 7;

    if(state == 3 && ch == '-') column = 6;//正负号与单目符＇－＇冲突
    if(state == 1 || state == 2) {//特判e/E和字母冲突的状态
        if(ch == 'e' || ch == 'E') column = 4;
    }
    state = scan_conv_table[state][column];
    if(state == 8) bp--;
    if(state == -1) std::cout << "s:" << column << std::endl;
}

bool issyn(char& ch) {
    for(int i = 0; i < _syn_size; ++i) {
        if(ch == syn[i][wp]) return true;
    }
    return false;
}

bool file_end(FILE* fp) {
    if(buf[ bp ] == '\0' && feof(fp)) return true;
    return false;
}

