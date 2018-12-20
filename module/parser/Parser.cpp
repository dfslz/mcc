#include "Parser.h"

Parser parser;

void Parser::parse() {
    tk = scanner.next();

    Token returnType;//函数返回值类型
    std::string functionName;
    if(tk.getCategory() != Token::keyword) {
        //error:函数没有返回值
        err(0);
    } else {
        //记录函数返回值类型
        returnType = tk;
    }

    tk = scanner.next();
    
    if(tk.getCategory() != Token::id) {
        //error: 函数名不正确
        err(1);
    } else {
        //记录函数名
        functionName = synbl.getName(tk.getOffset());
        synbl.setCategory(tk.getOffset(), SymbolList::function);
    }

    tk = scanner.next();

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "{") {
        //error:函数没有以大括号开头
        printToken(tk);
        err(2);
    } else {
        //生成开头四元式 (functionName, null, null, null)
        Quaternary qt;
        qt.setOption(functionName);
        qt.setFirst(Token());
        qt.setSecond(Token());
        qt.setTarget(Token());
        quaterList.insert(qt);
    }
    //进入函数块
    synbl.pushLoacle();

    tk = scanner.next();
    sentence();

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "}") {
        //error:函数没有以大括号结尾
        printToken(tk);
        err(3);
    } else {
        //生成结尾四元式 (endFunction, null, null, null)
        Quaternary qt;
        qt.setOption("endMain");
        qt.setFirst(Token());
        qt.setSecond(Token());
        qt.setTarget(Token());
        quaterList.insert(qt);
    }
    //函数块结束
    synbl.popLocale();
}

void Parser::sentence() {
    auto checkType = [&token = tk]()->bool {//检查是不是类型名
        if(token.getCategory() == Token::keyword) {
            std::string s = keywordList.get(token.getOffset());
            if(s == "int" || s == "float" || s == "char") return true;
        }
        return false;
    };
    if(checkType()) {//检查是不是类型名
        //记录type
        type = tk;
        tk = scanner.next();
    } else {
        type.setCategory(Token::null);
    }

    if(tk.getCategory() == Token::keyword && keywordList.get(tk.getOffset()) == "if") {
        tk = scanner.next();
        jmp();
    } else if(tk.getCategory() == Token::keyword && keywordList.get(tk.getOffset()) == "while") {
        tk = scanner.next();
        whloop();
    } else {
        content();
        tk = scanner.next();//防止分号被重复读取
    }

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "}") {
        //主程序未结束,继续生成句子
        sentence();
    }
}

void Parser::content() {
    if(tk.getCategory() != Token::id) {
        printToken(tk);
        err(4);
    } else {
        //检查id类型和重复定义
        if(type.getCategory() != Token::null) {//变量定义,检查是否重复定义过
            if(synbl.getCategory(tk.getOffset()) != SymbolList::null) {//变量已经定义过了
                //error:重复定义变量
                printToken(tk);
                err(5);
            } else {//定义变量
                synbl.setCategory(tk.getOffset(), SymbolList::various);//设置为变量类型
                int typePosition = typeList.find(keywordList.get(type.getOffset()));
                if(typePosition != -1) synbl.setType(tk.getOffset(), typePosition);
                //error: 未定义的类型
                else printToken(tk),err(6);
            }
        } else {//使用变量，应当判断是否定义过
            if(synbl.getCategory(tk.getOffset()) != SymbolList::various) {//未定义的变量
                printToken(tk);
                err(12);
            }
        }
        target = tk;
    }

    tk = scanner.next();

    if(tk.getCategory() != Token::symbol) {
        //error: 没有操作符
        printToken(tk);
        err(7);
    } else if(opList.get(tk.getOffset()) == "=") {
        tk = scanner.next();

        //生成赋值四元式
        Quaternary qt;
        qt.setOption("=");
        qt.setFirst(expression());//表达式解析在这里
        qt.setSecond(Token());
        qt.setTarget(target);
        quaterList.insert(qt);
    } 
    if(opList.get(tk.getOffset()) == ",") {
        tk = scanner.next();
        content();
    } else if(opList.get(tk.getOffset()) != ";") {
        //error:不能识别的符号
        printToken(tk);
        err(8);
    }

    //句子结束,清空全局变量
    type = target = Token();
    //tk = scanner.next();
}

/* 下面开始是表达式文法的解析内容
 * 采用了LL(1)文法,和上面递归下降分开
 */

//语法栈定义
enum _syn_unit {
    E = 0, E1 = 1, T = 2, T1 = 3, F = 4,//状态
    Num = 5,//数字
    Mul, Div, Add, Sub,//符号
    LF, RT,//左右括号
    OK,//表示有语义动作
    null//表示空
};
_syn_unit syn_stack[1000];
int synp;
const static _syn_unit bhv[10][6] = {
    { T, E1, null, null, null, null },
    { Add, T, OK, E1, null, null },
    { Sub, T, OK, E1, null, null },
    { null, null, null, null, null, null },
    { F, T1, null, null, null, null },
    { Mul, F, OK, T1, null, null },
    { Div, F, OK, T1, null, null },
    { null, null, null, null, null, null },
    { Num, null, null, null, null, null },
    { LF, E, RT, null, null, null }
};

//语义栈
Token sem_stack[1000];
int semp;

//LL(1)分析表
const static int LLOne[5][8] = {
    //i + - * / ( ) ;
    { 0, -1, -1, -1, -1, 0, -1, -1 },
    { -1, 1, 2, -1, -1, -1, 3, 3 },
    { 4, -1, -1, -1, -1, 4, -1, -1 },
    { -1, 7, 7, 5, 6, -1, 7, 7 },
    { 8, -1, -1, -1, -1, 9, -1, -1 }
};

//记录生成的四元式数量,用于判断表达式为f=10;这样的简单赋值语句
int count = 0;
Token returnToken;

bool Parser::isConstantOrIdentify(Token::Categories cat) {
    if(cat == Token::integer || cat == Token::real || cat == Token::ch) {
        return true;
    } else if(cat == Token::id) {
        if(synbl.getCategory(tk.getOffset()) != SymbolList::various) {
            printToken(tk);
            err(12);//使用了未定义的变量
        }
        return true;//为了不触发表达式非法的错误返回正确
    }
    return false;
}

void Parser::exp_bhv() {
    if(syn_stack[synp] < 5) {
        int code = -1;
        if(isConstantOrIdentify(tk.getCategory())) code = 0;
        else if(tk.getCategory() != Token::symbol) {
            printToken(tk);
            err(9);
        } else {
            //lambda表达式判断是否是当前符号
            auto symcmp = [&t = tk](std::string s) -> bool { return s == opList.get(t.getOffset()); };
            
            if(symcmp("+")) code = 1;
            else if(symcmp("-")) code = 2;
            else if(symcmp("*")) code = 3;
            else if(symcmp("/")) code = 4;
            else if(symcmp("(")) code = 5;
            else if(symcmp(")")) code = 6;
            else if(symcmp(";") || symcmp(",") || symcmp(">") || symcmp("<")
                || symcmp(">=") || symcmp("<=") || symcmp("==") || symcmp("{")) code = 7;
            else printToken(tk), err(10);
            //else error:非法符号
        }
        int order = syn_stack[synp];
        --synp;//语法栈弹出

        //lambda表达式把推导式逆序压栈
        auto pushsyn = [&o = ok, &t = tk](int p){
            if(p == -1) {
                o = false;
                printToken(t);
                err(11);
            } else {
                for(int i = 5; i >= 0; --i) {
                    if(bhv[p][i] != null) {
                        syn_stack[++synp] = bhv[p][i];
                    }
                }
            }
        };
        //压栈
        pushsyn(LLOne[order][code]);
    } else {//到这里非终结符处理结束了
        auto opcmp = []()->bool {//判断是不是运算符或者操作数
            _syn_unit a = syn_stack[synp];
            if(a == Add || a == Sub || a == Mul || a == Div || a == Num) return true;
            return false;
        };

        if(opcmp()) {//是运算符或操作数进语义栈
            --synp;
            sem_stack[semp++] = tk;
            returnToken = tk;//记录最后一个操作数,用于生成没有表达式的情况
            tk = scanner.next();
        } else if(syn_stack[synp] == LF || syn_stack[synp] == RT) {
            --synp;
            tk = scanner.next();
        } else if(syn_stack[synp] == OK) {
            //生成四元式,并将临时变量写入符号表
            --synp;
            Quaternary qt;
            qt.setSecond(sem_stack[--semp]);
            qt.setOption(opList.get(sem_stack[--semp].getOffset()));
            qt.setFirst(sem_stack[--semp]);

            Token target = getTarget();
            qt.setTarget(target);
            quaterList.insert(qt);//插入四元式序列
            sem_stack[semp++] = target;//结果单元压语义栈
            ++count;//四元式计数增加
        }
    }
}

Token Parser::expression() {
    //初始化语法语义栈
    synp = semp = count = 0;
    returnToken = Token();
    syn_stack[0] = E;
    while(synp >= 0) {
        auto idOrConst = [&t = tk]()->bool {//判断是不是id或者常数
            Token::Categories cat = t.getCategory();
            if(cat == Token::id || cat == Token::real || cat == Token::integer
                    || cat == Token::ch) 
                return true;
            return false;
        };
        if(idOrConst()) returnToken = tk;

        exp_bhv();
    }
    //把表达式结果四元式赋值给
    if(count != 0) {
        Quaternary qt = quaterList.get(quaterList.size() - 1);
        returnToken = qt.getTarget();
    }
    return returnToken;
}

//下面开始是if语句的语法语义解析部分
void Parser::jmp() {
    //开始if语句块
    synbl.pushLoacle();

    Token fst, snd;//用于记录第一第二操作数
    Token cmpOpt;//比较运算符

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "(") {
        printToken(tk);
        err(13);
    }

    tk = scanner.next();
    expression();
    fst = count == 0? returnToken : quaterList.get(quaterList.size()-1).getTarget();

    auto isCmp = [&t = tk]() {//比较运算符
        Token::Categories cat = t.getCategory();
        if(cat != Token::symbol) printToken(t),err(9);
        std::string s = opList.get(t.getOffset());
        if(s != ">" && s != ">=" && s != "==" && s != "<=" && s != "<") printToken(t),err(10);
    };
    
    isCmp();
    cmpOpt = tk;

    tk.setCategory(Token::symbol);
    tk.setOffset(opList.find("("));
    expression();
    snd = count == 0 ? returnToken : quaterList.get(quaterList.size()-1).getTarget();

    Quaternary qt;
    qt.setOption(opList.get(cmpOpt.getOffset()));
    qt.setFirst(fst);
    qt.setSecond(snd);
    qt.setTarget(getTarget());
    quaterList.insert(qt);//比较四元式

    qt.setFirst(quaterList.get(quaterList.size() - 1).getTarget());
    qt.setSecond(Token());
    qt.setTarget(Token());
    qt.setOption("if");
    quaterList.insert(qt);//if开头四元式

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "{") {
        printToken(tk);
        err(14);
    }

    tk = scanner.next();
    sentence();//内部语句块

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "}") {
        printToken(tk);
        err(14);
    }

    tk = scanner.next();

    qt.setOption("el");
    qt.setFirst(Token());
    qt.setSecond(Token());
    qt.setTarget(Token());
    quaterList.insert(qt);

    if(tk.getCategory() == Token::keyword && keywordList.get(tk.getOffset()) == "else") {
        tk = scanner.next();
        if(tk.getCategory() == Token::keyword && keywordList.get(tk.getOffset()) == "if") {
            tk = scanner.next();
            jmp();//递归if语句
        } else if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "{") {
            printToken(tk);
            err(14);
        } else {
            tk = scanner.next();
            sentence();//else语句的内部语句块
            if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "}") {
                printToken(tk);
                err(14);
            }
            tk = scanner.next();
        }
    }
    qt.setOption("ei");
    quaterList.insert(qt);
    //结束if语句块
    synbl.popLocale();
}

Token Parser::getTarget() {//生成临时变量存放结果单元,并将临时变量写入符号表
    Token temp;
    temp.setCategory(Token::id);
    std::string name = "@t";
    static int a = 0;
    int pos = synbl.insert(name + std::to_string(a++));
    temp.setOffset(pos);
    synbl.setCategory(pos, SymbolList::various);
    //TODO: 设置类型,需要向上转换,比如int遇到float转换成float
    return temp;
};

void Parser::whloop() {
    //开始while语句块
    synbl.pushLoacle();

    Quaternary qt;
    qt.setOption("wh");
    quaterList.insert(qt);//生成开头四元式

    Token fst, snd;//用于记录第一第二操作数
    Token cmpOpt;//比较运算符

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "(") {
        printToken(tk);
        err(15);
    }

    tk = scanner.next();
    expression();
    fst = count == 0? returnToken : quaterList.get(quaterList.size()-1).getTarget();

    auto isCmp = [&t = tk]() {//比较运算符
        Token::Categories cat = t.getCategory();
        if(cat != Token::symbol) printToken(t),err(9);
        std::string s = opList.get(t.getOffset());
        if(s != ">" && s != ">=" && s != "==" && s != "<=" && s != "<") printToken(t),err(10);
    };
    
    isCmp();
    cmpOpt = tk;

    tk.setCategory(Token::symbol);
    tk.setOffset(opList.find("("));
    expression();
    snd = count == 0 ? returnToken : quaterList.get(quaterList.size()-1).getTarget();

    qt.setOption(opList.get(cmpOpt.getOffset()));
    qt.setFirst(fst);
    qt.setSecond(snd);
    qt.setTarget(getTarget());
    quaterList.insert(qt);//比较四元式生成完毕

    //条件判断四元式,和if一致
    qt.setOption("do");
    qt.setFirst(qt.getTarget());
    qt.setSecond(Token());
    qt.setTarget(Token());
    quaterList.insert(qt);//插入条件判断四元式

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "{") {
        printToken(tk);
        err(16);
    }
    tk = scanner.next();

    sentence();//块内语句

    if(tk.getCategory() != Token::symbol || opList.get(tk.getOffset()) != "}") {
        printToken(tk);
        err(16);
    }
    qt.setOption("we");
    qt.setFirst(Token());
    qt.setSecond(Token());
    qt.setTarget(Token());
    quaterList.insert(qt);//生成结尾四元式
    tk = scanner.next();

    //结束while语句块
    synbl.popLocale();
}