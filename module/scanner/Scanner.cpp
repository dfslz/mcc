#include "Scanner.h"

Scanner scanner;

//Buffer Scanner::buffer;

Token Scanner::next() {
    Token tk;
    _state = stateBegin;
    _type = -1;
    word = "";

    while(_state != stateEnd) {
        char ch = buffer.get();//只读当前字符,buffer指针不动
        if(behavior(ch)) {//只有behavior()接收了字符才移动buffer指针
            buffer.next();
        }
        //stateTransition(ch);//状态转移函数
        if(_state == -1) {
            std::cout << "state changed into -1\n";
            _type = -1;
            break;
        }
    }
    //std::cout << std::endl;

    //按照得到的类型分别生成对应的token
    if(_type == 0) {//整型
        tk.setCategory(Token::integer);
        tk.setOffset(intList.insert(std::stoi(word)));
    } else if(_type == 1) {//关键字或标识符
        int pos = keywordList.find(word);
        if(pos != -1) {//是关键字
            tk.setCategory(Token::keyword);
            tk.setOffset(pos);
        } else {//是标识符
            tk.setCategory(Token::id);
            pos = synbl.find(word);
            if(pos != -1) tk.setOffset(pos);//已定义的标识符
            else tk.setOffset(synbl.insert(word));//加入新定义标识符
        }
    } else if(_type == 2) {//浮点数
        tk.setCategory(Token::real);
        tk.setOffset(floatList.insert(std::stof(word)));
    } else if(_type == 3) {//符号
        tk.setCategory(Token::symbol);
        tk.setOffset(opList.find(word));
    } else if(_type == 4) {//字符常量
        tk.setCategory(Token::ch);
        tk.setOffset(charList.insert(word[1]));
    } else if(_type == 5) {//字符串
        tk.setCategory(Token::str);
        tk.setOffset(strList.insert(word));
    } else {
        std::cout <<"can not clearfy token type" << std::endl;
        tk.setCategory(Token::null);
        tk.setOffset(-1);
    }
    
    return tk;
}

bool Scanner::behavior(char ch) {
    bool ok = false;
    stateTransition(ch);
    if(_state != -1 && _state != stateEnd) {
        if(_state != stateBegin) word += ch;
        ok = true;
    }
    //判断合法字符的类型
    if (ok) {
        if (_state == 1) _type = 0; //整形
        else if (_state == 7) _type = 1; //关键字或标识符
        else if (_state == 2 || _state == 3) _type = 2; //浮点型
        else if (_state == 9) _type = 3; //符号
        else if (_state == 10) _type = 4; //字符常量
        else if (_state == 12) _type = 5; //字符串常量
    }
    return ok;
}

bool Scanner::issymbol(char ch) {
    bool ok = false;
    for(int i = 0; i < opList.size(); ++i) {
        std::string s = opList.get(i);
        if(s.size() > word.size() && ch == s[word.size()]) {
            //std::cout << s[word.size()] << ":" << ch << std::endl;
            ok = true;
            break;
        }
    }
    //getchar();
    return ok;
}

void Scanner::stateTransition(char ch) {
    _state = stateTransTable[_state][getOrder(ch)];
}

int Scanner::getOrder(char ch) {//获取对于于转移表的顺序位置
    int order;
    if(isalpha(ch)) order = 0;//这里后面要对指数'E/e'特殊处理一下
    else if(isdigit(ch)) order = 1;
    else if(ch == '_') order = 2;
    else if(ch == '.') order = 3;//空缺的4是指数E/e
    else if(ch == '+' || ch == '-') order = 6;//正负号要放在界符前
    else if(issymbol(ch)) order = 5;
    else if(ch == '\'') order = 8;//单引号
    else if(ch == '\"') order = 9;//双引号
    else order = 7;

    if(_state == 1 || _state == 2) {
        if(ch == 'E' || ch == 'e') order = 4;
    }
    return order;
}

//状态转移表,具体含义见文档 "扫描器自动机.pdf" 和 "扫描器状态转移表.ods"
int Scanner::stateTransTable[_trans_table_raw][_trans_table_column] = {//状态转移矩阵
    { 7, 1, 7, 6, 7, 9, 9, 0, 10, 12 },
    { 8, 1, 8, 2, 3, 8, 8, 8, 8, 8 },
    { 8, 2, 8, 8, 3, 8, 8, 8, 8, 8 },
    { 8, 5, 8, 8, 8, -1, 4, -1, -1, -1 },
    { 8, 5, 8, 8, 8, -1, -1, -1, -1, -1 },
    { 8, 5, 8, 8, 8, 8, 8, 8, 8, 8 },
    { 8, 2, 8, 8, 8, -1, -1, -1, -1, -1 },
    { 7, 7, 7, 8, 7, 8, 8, 8, 8, 8 },
    { 7, 1, 7, 6, 7, 8, 8, 0, 10, 12 },
    { 8, 8, 8, 8, 8, 9, 8, 8, 8, 8 },
    { 11, 11, 11, 11, 11, 11, 11, 11, -1, 11 },
    { -1, -1, -1, -1, -1, -1, -1, -1, 14, -1 },
    { 13, 13, 13, 13, 13, 13, 13, 13, 13, 14 },
    { 13, 13, 13, 13, 13, 13, 13, 13, 13, 14 },
    { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 }
};