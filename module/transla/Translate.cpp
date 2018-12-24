#include "Translate.h"
#include "Common.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <stack>
#include <fstream>

using namespace std;



stack <int>stk;//用于暂存存值语句
stack <int>sem;//用于暂存回填地址
stack <int>whilepiece;//i_w用于while循环
stack <string>_compare;//用于暂存比较符
stack <int>array_SI;//暂存数组变量时下标为变量时变量地址


string sd[100]={"$main","=","+","-","*","/","end$main","if","el","ei","wh","do","we",">",">=","<","<=","==","!=","print","\0"};
Token fst,sed,tar;
string outen[1000];//用于存储目标代码
string oa,ob,oc;//用于string连接
int io=0,_io;//用于记录outen数组个数以及填写

int i_piece=1,i_whilepiece;//i_p用于块计数,i_w用于while循环

string _piece="PIECE",_maohao=":",_jmp="JMP ";

void translate()
{
    int i,num=quaterList.size();
    for(i=0;i<num;i++)
    {
        Quaternary qt=quaterList.get(i);
        //if(qt.getOption()==sd[8])
           // break;

        if(qt.getOption()==sd[0])//main
            tra_main(qt);

        if(qt.getOption()==sd[1])//=
            tra_equal(qt);

        if(qt.getOption()==sd[2])//+
            tra_add(qt);

        if(qt.getOption()==sd[3])//-
            tra_sub(qt);

        if(qt.getOption()==sd[4])//*
            tra_mul(qt);

        if(qt.getOption()==sd[5])// /
            tra_div(qt);

        if(qt.getOption()==sd[6])//endMain
            tra_end(qt);

        if(qt.getOption()==sd[7])//if
            tra_if(qt);

        if(qt.getOption()==sd[8])//el
            tra_el(qt);

        if(qt.getOption()==sd[9])//ie
            tra_ie(qt);

        if(qt.getOption()==sd[10])//while
            tra_while(qt);

        if(qt.getOption()==sd[11])//do
            tra_do(qt);

        if(qt.getOption()==sd[12])//we
            tra_we(qt);

        if(qt.getOption()==sd[13])//>
            tra_greater(qt);

        if(qt.getOption()==sd[14])//>=
            tra_grequal(qt);

        if(qt.getOption()==sd[15])//<
            tra_less(qt);

        if(qt.getOption()==sd[16])//<=
            tra_lesequal(qt);

        if(qt.getOption()==sd[17])//==
            tra_realequal(qt);

        if(qt.getOption()==sd[18])// !=
            tra_unequal(qt);

        if(qt.getOption()==sd[19])//print
        {
            tra_out(qt);
        }

    }



    for(i=0;i<io;i++)
    {
        fout << outen[i] << endl ;
    }


}

void intable(char _oa[100],int _ob,char _oc[100])//将翻译出的机器码存入表中
{
    oa=_oa;
    ob=to_string(_ob);
    oc=_oc;
    outen[io++]=oa+ob+oc;
}

void intable(char _oa[100],int _ob)
{
    oa=_oa;
    ob=to_string(_ob);
    outen[io++]=oa+ob;
}

void intable(char _oa[100])
{
    oa=_oa;
    outen[io++]=oa;
}

void tra_main(Quaternary qt)//(main, , , )
{
    //cout << "main" << " " << "SEGMENT" << endl ;
    //outen[io++]="main SEGMENt";
    intable("main SEGMENT");
    //cout << "L" << " " << "DB" << " " << "1000" << " " << "DUP(0)" << endl;
    //outen[io++]="L DB 1000 DUP(0)";
    intable("L DB 1000 DUP(0)");
    //cout << "ASSUME CS:example, DS:example" << endl ;
    //outen[io++]="ASSUME CS:example, DS:example";
    intable("ASSUME CS:main, DS:main");
    //cout << "STAR:" << endl ;
    //outen[io++]="STAR:";
    //intable(_piece,i_piece++,_maohao);
    outen[io++]=_piece+to_string(i_piece++)+_maohao;
    intable("LEA BX,L");
}

void tra_equal(Quaternary qt)//(=,I1, ,I2)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    if(stk.empty()==true)
    {
        if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(fst.getPosition()>=0)
            {

                intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
            }
        }
        else
        {
            if(fst.getCategory()==Token::id)
            {
                //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getCategory()==Token::integer)
            {
                // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",intList.get(fst.getOffset()));
            }
            else if(fst.getCategory()==Token::real)
            {
                //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",floatList.get(fst.getOffset()));
            }
        }

        if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(tar.getPosition()>=0)
            {

                stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
            }
            else if(tar.getPosition()<0)
            {

                array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                stk.push(synbl.getOffset(tar.getOffset()));
            }
        }
        else
        {
            stk.push(synbl.getOffset(tar.getOffset()));
        }

    }
    else if(stk.empty()==false)
    {
        if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(fst.getPosition()>=0)
            {
                if(stk.top()==intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset())&&fst.getActivity()==false)
                {
                    stk.pop();

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    if(array_SI.empty()==true)
                    {
                        intable("MOV [BX+",stk.top(),"],AX");
                        stk.pop();
                    }
                    else if(array_SI.empty()==false)
                    {
                        intable("MOV SI,[BX+",array_SI.top(),"]");
                        intable("MOV [BX+",stk.top(),"+SI],AX");
                        array_SI.pop();
                        stk.pop();
                    }

                    intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }

            }
            else if(fst.getPosition()<0)
            {
                if(stk.top()==synbl.getOffset(fst.getOffset())&&array_SI.top()==synbl.getOffset(-fst.getPosition())&&fst.getActivity()==false)
                {
                    array_SI.pop();
                    stk.pop();

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    if(array_SI.empty()==true)
                    {
                        intable("MOV [BX+",stk.top(),"],AX");
                        stk.pop();
                    }
                    else if(array_SI.empty()==false)
                    {
                        intable("MOV SI,[BX+",array_SI.top(),"]");
                        intable("MOV [BX+",stk.top(),"+SI],AX");
                        array_SI.pop();
                        stk.pop();
                    }

                    intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                    intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
            }

        }
        else
        {
            if(stk.top()==synbl.getOffset(fst.getOffset())&&fst.getActivity()==false)
            {
                stk.pop();


                if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(tar.getPosition()>=0)
                    {
                        stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));

                    }
                    else if(tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(tar.getOffset()));
                }
            }
            else
            {
                if(array_SI.empty()==true)
                {
                    intable("MOV [BX+",stk.top(),"],AX");
                    stk.pop();
                }
                else if(array_SI.empty()==false)
                {
                    intable("MOV SI,[BX+",array_SI.top(),"]");
                    intable("MOV [BX+",stk.top(),"+SI],AX");
                    array_SI.pop();
                    stk.pop();
                }

                if(fst.getCategory()==Token::id)
                {
                    //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                    intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"]");
                }
                else if(fst.getCategory()==Token::integer)
                {
                    // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",intList.get(fst.getOffset()));
                }
                else if(fst.getCategory()==Token::real)
                {
                    //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",floatList.get(fst.getOffset()));
                }

                if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(tar.getPosition()>=0)
                    {
                        stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));

                    }
                    else if(tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(tar.getOffset()));

                }

            }
        }

    }

}

void tra_add(Quaternary qt)//(+,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    /*if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
    {

    }*/

    if(stk.empty()==true)
    {
        if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(fst.getPosition()>=0)
            {
                intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
            }
        }
        else
        {
            if(fst.getCategory()==Token::id)
            {
                //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getCategory()==Token::integer)
            {
                // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",intList.get(fst.getOffset()));
            }
            else if(fst.getCategory()==Token::real)
            {
                //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",floatList.get(fst.getOffset()));
            }
        }

        if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(sed.getPosition()>=0)
            {
                intable("ADD AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
            }
            else if(sed.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
            }
        }
        else
        {
            if(sed.getCategory()==Token::id)
            {
                //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

            }
            else if(sed.getCategory()==Token::integer)
            {
                //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                intable("ADD AX,",intList.get(sed.getOffset()));
            }
            else if(sed.getCategory()==Token::real)
            {
                //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                intable("ADD AX,",floatList.get(sed.getOffset()));
            }
        }

        if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(tar.getPosition()>=0)
            {
                stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
            }
            else if(tar.getPosition()<0)
            {
                array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                stk.push(synbl.getOffset(tar.getOffset()));
            }
        }
        else
        {
            stk.push(synbl.getOffset(tar.getOffset()));
        }

    }
    else if(stk.empty()==false)
    {
        if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(fst.getPosition()>=0)
            {
                if(stk.top()==intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset())&&fst.getActivity()==false)
                {
                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("ADD AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("ADD AX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("ADD AX,",floatList.get(sed.getOffset()));
                        }
                    }

                    stk.pop();

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    intable("MOV [BX+",stk.top(),"],AX");
                    stk.pop();

                    if(fst.getPosition()>=0)
                    {
                        intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
                    }
                    else if(fst.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                        intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
                    }

                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("ADD AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("ADD AX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("ADD AX,",floatList.get(sed.getOffset()));
                        }
                    }



                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
            }
            else if(fst.getPosition()<0)
            {
                if(stk.top()==synbl.getOffset(fst.getOffset())&&array_SI.top()==synbl.getOffset(-fst.getPosition())&&fst.getActivity()==false)
                {
                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("ADD AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("ADD AX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("ADD AX,",floatList.get(sed.getOffset()));
                        }
                    }

                    stk.pop();
                    array_SI.pop();

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    intable("MOV SI,[BX+",array_SI.top(),"]");
                    intable("MOV [BX+",stk.top(),"+SI],AX");
                    array_SI.pop();
                    stk.pop();

                    if(fst.getPosition()>=0)
                    {
                        intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
                    }
                    else if(fst.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                        intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
                    }

                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("ADD AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("ADD AX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("ADD AX,",floatList.get(sed.getOffset()));
                        }
                    }


                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }

            }

        }

        else
        {
            if(stk.top()==synbl.getOffset(fst.getOffset())&&fst.getActivity()==false)
            {
                if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(sed.getPosition()>=0)
                    {
                        intable("ADD AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                        intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                    }
                }
                else
                {
                    if(sed.getCategory()==Token::id)
                    {
                        //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                        intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                    }
                    else if(sed.getCategory()==Token::integer)
                    {
                        //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                        intable("ADD AX,",intList.get(sed.getOffset()));
                    }
                    else if(sed.getCategory()==Token::real)
                    {
                        //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                        intable("ADD AX,",floatList.get(sed.getOffset()));
                    }
                }

                stk.pop();

                if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(tar.getPosition()>=0)
                    {
                        stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                    }
                    else if(tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(tar.getOffset()));
                }
            }
            else
            {
                //cout << "MOV " << "[L+" << stk.top() << "],AX" << endl ;
                intable("MOV [BX+",stk.top(),"],AX");
                stk.pop();

                if(fst.getCategory()==Token::id)
                {
                    //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                    intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"]");
                }
                else if(fst.getCategory()==Token::integer)
                {
                    // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",intList.get(fst.getOffset()));
                }
                else if(fst.getCategory()==Token::real)
                {
                    //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",floatList.get(fst.getOffset()));
                }

                if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(sed.getPosition()>=0)
                    {
                        intable("ADD AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                        intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                    }
                }
                else
                {
                    if(sed.getCategory()==Token::id)
                    {
                        //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                        intable("ADD AX,[BX+",synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getCategory()==Token::integer)
                    {
                        //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                        intable("ADD AX,",intList.get(sed.getOffset()));
                    }
                    else if(sed.getCategory()==Token::real)
                    {
                        //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                        intable("ADD AX,",floatList.get(sed.getOffset()));
                    }
                }



                if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(tar.getPosition()>=0)
                    {
                        stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                    }
                    else if(tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(tar.getOffset()));
                }
            }
        }

    }
}

void _SUB(Token _fst,Token _sed,Token _tar)
{
    if(stk.empty()==true)
    {
        if(synbl.getCategory(_fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(_fst.getPosition()>=0)
            {
                intable("MOV AX,[BX+",intList.get(_fst.getPosition())+synbl.getOffset(_fst.getOffset()),"]");
            }
            else if(_fst.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-_fst.getPosition()),"]");
                intable("MOV AX,[BX+",synbl.getOffset(_fst.getOffset()),"+SI]");
            }
        }
        else
        {
            if(_fst.getCategory()==Token::id)
            {
                //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                intable("MOV AX,[BX+",synbl.getOffset(_fst.getOffset()),"]");
            }
            else if(_fst.getCategory()==Token::integer)
            {
                // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",intList.get(_fst.getOffset()));
            }
            else if(_fst.getCategory()==Token::real)
            {
                //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",floatList.get(_fst.getOffset()));
            }
        }

        if(synbl.getCategory(_sed.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(_sed.getPosition()>=0)
            {
                intable("SUB AX,[BX+",intList.get(_sed.getPosition())+synbl.getOffset(_sed.getOffset()),"]");
            }
            else if(_sed.getPosition()<0)
            {
                intable("SUB SI,[BX+",synbl.getOffset(-_sed.getPosition()),"]");
                intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"+SI]");
            }
        }
        else
        {
            if(_sed.getCategory()==Token::id)
            {
                //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"]");

            }
            else if(_sed.getCategory()==Token::integer)
            {
                //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                intable("SUB AX,",intList.get(_sed.getOffset()));
            }
            else if(_sed.getCategory()==Token::real)
            {
                //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                intable("SUB AX,",floatList.get(_sed.getOffset()));
            }
        }

        if(synbl.getCategory(_tar.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(_tar.getPosition()>=0)
            {
                stk.push(intList.get(_tar.getPosition())+synbl.getOffset(_tar.getOffset()));
            }
            else if(_tar.getPosition()<0)
            {
                array_SI.push(synbl.getOffset(-_tar.getPosition()));//下标变量地址压栈
                stk.push(synbl.getOffset(_tar.getOffset()));
            }
        }
        else
        {
            stk.push(synbl.getOffset(_tar.getOffset()));
        }

    }
    else if(stk.empty()==false)
    {
        if(synbl.getCategory(_fst.getOffset())==SymbolList::array)
        {
            if(_fst.getPosition()>=0)
            {
                if(stk.top()==intList.get(_fst.getPosition())+synbl.getOffset(_fst.getOffset())&&_fst.getActivity()==false)
                {
                    if(synbl.getCategory(_sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(_sed.getPosition()>=0)
                        {
                            intable("SUB AX,[BX+",intList.get(_sed.getPosition())+synbl.getOffset(_sed.getOffset()),"]");
                        }
                        else if(_sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-_sed.getPosition()),"]");
                            intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(_sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"]");

                        }
                        else if(_sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("SUB AX,",intList.get(_sed.getOffset()));
                        }
                        else if(_sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("SUB AX,",floatList.get(_sed.getOffset()));
                        }
                    }

                    stk.pop();

                    if(synbl.getCategory(_tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(_tar.getPosition()>=0)
                        {
                            stk.push(intList.get(_tar.getPosition())+synbl.getOffset(_tar.getOffset()));
                        }
                        else if(_tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-_tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(_tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(_tar.getOffset()));
                    }
                }
                else
                {
                    intable("MOV [BX+",stk.top(),"],AX");
                    stk.pop();

                    if(_fst.getPosition()>=0)
                    {
                        intable("MOV AX,[BX+",intList.get(_fst.getPosition())+synbl.getOffset(_fst.getOffset()),"]");
                    }
                    else if(_fst.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-_fst.getPosition()),"]");
                        intable("MOV AX,[BX+",synbl.getOffset(_fst.getOffset()),"+SI]");
                    }

                    if(synbl.getCategory(_sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(_sed.getPosition()>=0)
                        {
                            intable("SUB AX,[BX+",intList.get(_sed.getPosition())+synbl.getOffset(_sed.getOffset()),"]");
                        }
                        else if(_sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-_sed.getPosition()),"]");
                            intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(_sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"]");

                        }
                        else if(_sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("SUB AX,",intList.get(_sed.getOffset()));
                        }
                        else if(_sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("SUB AX,",floatList.get(_sed.getOffset()));
                        }
                    }



                    if(synbl.getCategory(_tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(_tar.getPosition()>=0)
                        {
                            stk.push(intList.get(_tar.getPosition())+synbl.getOffset(_tar.getOffset()));
                        }
                        else if(_tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-_tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(_tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(_tar.getOffset()));
                    }
                }
            }
            else if(_fst.getPosition()<0)
            {
                if(stk.top()==synbl.getOffset(_fst.getOffset())&&array_SI.top()==synbl.getOffset(-_fst.getPosition())&&_fst.getActivity()==false)
                {
                    if(synbl.getCategory(_sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(_sed.getPosition()>=0)
                        {
                            intable("SUB AX,[BX+",intList.get(_sed.getPosition())+synbl.getOffset(_sed.getOffset()),"]");
                        }
                        else if(_sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-_sed.getPosition()),"]");
                            intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(_sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"]");

                        }
                        else if(_sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("SUB AX,",intList.get(_sed.getOffset()));
                        }
                        else if(_sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("SUB AX,",floatList.get(_sed.getOffset()));
                        }
                    }

                    stk.pop();
                    array_SI.pop();

                    if(synbl.getCategory(_tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(_tar.getPosition()>=0)
                        {
                            stk.push(intList.get(_tar.getPosition())+synbl.getOffset(_tar.getOffset()));
                        }
                        else if(_tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-_tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(_tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(_tar.getOffset()));
                    }
                }
                else
                {
                    intable("MOV SI,[BX+",array_SI.top(),"]");
                    intable("MOV [BX+",stk.top(),"+SI],AX");
                    array_SI.pop();
                    stk.pop();

                    if(_fst.getPosition()>=0)
                    {
                        intable("MOV AX,[BX+",intList.get(_fst.getPosition())+synbl.getOffset(_fst.getOffset()),"]");
                    }
                    else if(_fst.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-_fst.getPosition()),"]");
                        intable("MOV AX,[BX+",synbl.getOffset(_fst.getOffset()),"+SI]");
                    }

                    if(synbl.getCategory(_sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(_sed.getPosition()>=0)
                        {
                            intable("SUB AX,[BX+",intList.get(_sed.getPosition())+synbl.getOffset(_sed.getOffset()),"]");
                        }
                        else if(_sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-_sed.getPosition()),"]");
                            intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(_sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"]");

                        }
                        else if(_sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("SUB AX,",intList.get(_sed.getOffset()));
                        }
                        else if(_sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("SUB AX,",floatList.get(_sed.getOffset()));
                        }
                    }


                    if(synbl.getCategory(_tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(_tar.getPosition()>=0)
                        {
                            stk.push(intList.get(_tar.getPosition())+synbl.getOffset(_tar.getOffset()));
                        }
                        else if(_tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-_tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(_tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(_tar.getOffset()));
                    }
                }

            }

        }

        else
        {
            if(stk.top()==synbl.getOffset(_fst.getOffset())&&_fst.getActivity()==false)
            {
                if(synbl.getCategory(_sed.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(_sed.getPosition()>=0)
                    {
                        intable("SUB AX,[BX+",intList.get(_sed.getPosition())+synbl.getOffset(_sed.getOffset()),"]");
                    }
                    else if(_sed.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-_sed.getPosition()),"]");
                        intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"+SI]");
                    }
                }
                else
                {
                    if(_sed.getCategory()==Token::id)
                    {
                        //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                        intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"]");

                    }
                    else if(_sed.getCategory()==Token::integer)
                    {
                        //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                        intable("SUB AX,",intList.get(_sed.getOffset()));
                    }
                    else if(_sed.getCategory()==Token::real)
                    {
                        //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                        intable("SUB AX,",floatList.get(_sed.getOffset()));
                    }
                }

                stk.pop();

                if(synbl.getCategory(_tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(_tar.getPosition()>=0)
                    {
                        stk.push(intList.get(_tar.getPosition())+synbl.getOffset(_tar.getOffset()));
                    }
                    else if(_tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-_tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(_tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(_tar.getOffset()));
                }
            }
            else
            {
                //cout << "MOV " << "[L+" << stk.top() << "],AX" << endl ;
                intable("MOV [BX+",stk.top(),"],AX");
                stk.pop();

                if(_fst.getCategory()==Token::id)
                {
                    //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                    intable("MOV AX,[BX+",synbl.getOffset(_fst.getOffset()),"]");
                }
                else if(_fst.getCategory()==Token::integer)
                {
                    // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",intList.get(_fst.getOffset()));
                }
                else if(_fst.getCategory()==Token::real)
                {
                    //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",floatList.get(_fst.getOffset()));
                }

                if(synbl.getCategory(_sed.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(_sed.getPosition()>=0)
                    {
                        intable("SUB AX,[BX+",intList.get(_sed.getPosition())+synbl.getOffset(_sed.getOffset()),"]");
                    }
                    else if(_sed.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-_sed.getPosition()),"]");
                        intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"+SI]");
                    }
                }
                else
                {
                    if(_sed.getCategory()==Token::id)
                    {
                        //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                        intable("SUB AX,[BX+",synbl.getOffset(_sed.getOffset()),"]");
                    }
                    else if(_sed.getCategory()==Token::integer)
                    {
                        //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                        intable("SUB AX,",intList.get(_sed.getOffset()));
                    }
                    else if(_sed.getCategory()==Token::real)
                    {
                        //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                        intable("SUB AX,",floatList.get(_sed.getOffset()));
                    }
                }



                if(synbl.getCategory(_tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(_tar.getPosition()>=0)
                    {
                        stk.push(intList.get(_tar.getPosition())+synbl.getOffset(_tar.getOffset()));
                    }
                    else if(_tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-_tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(_tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(_tar.getOffset()));
                }
            }
        }

    }
}

void tra_sub(Quaternary qt)//(-,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    _SUB(fst,sed,tar);


}

void tra_mul(Quaternary qt)//(*,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    if(stk.empty()==true)
    {
        if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(fst.getPosition()>=0)
            {
                intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
            }
        }
        else
        {
            if(fst.getCategory()==Token::id)
            {
                //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getCategory()==Token::integer)
            {
                // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",intList.get(fst.getOffset()));
            }
            else if(fst.getCategory()==Token::real)
            {
                //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",floatList.get(fst.getOffset()));
            }
        }

        if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(sed.getPosition()>=0)
            {
                intable("MOV CX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
            }
            else if(sed.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
            }


        }
        else
        {
            if(sed.getCategory()==Token::id)
            {
                //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"]");

            }
            else if(sed.getCategory()==Token::integer)
            {
                //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                intable("MOV CX,",intList.get(sed.getOffset()));
            }
            else if(sed.getCategory()==Token::real)
            {
                //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                intable("MOV CX,",floatList.get(sed.getOffset()));
            }
        }

        intable("MUL CX");

        if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(tar.getPosition()>=0)
            {
                stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
            }
            else if(tar.getPosition()<0)
            {
                array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                stk.push(synbl.getOffset(tar.getOffset()));
            }
        }
        else
        {
            stk.push(synbl.getOffset(tar.getOffset()));
        }

    }
    else if(stk.empty()==false)
    {
        if(synbl.getCategory(fst.getOffset())==SymbolList::array)
        {
            if(fst.getPosition()>=0)
            {
                if(stk.top()==intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset())&&fst.getActivity()==false)
                {
                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("MOV CX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("MOV CX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("MOV CX,",floatList.get(sed.getOffset()));
                        }
                    }

                    intable("MUL CX");

                    stk.pop();

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    intable("MOV [BX+",stk.top(),"],AX");
                    stk.pop();

                    if(fst.getPosition()>=0)
                    {
                        intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
                    }
                    else if(fst.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                        intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
                    }

                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("MOV CX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("MOV CX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("MOV CX,",floatList.get(sed.getOffset()));
                        }
                    }

                    intable("MUL CX");

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
            }
            else if(fst.getPosition()<0)
            {
                if(stk.top()==synbl.getOffset(fst.getOffset())&&array_SI.top()==synbl.getOffset(-fst.getPosition())&&fst.getActivity()==false)
                {
                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("MOV CX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("MOV CX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("MOV CX,",floatList.get(sed.getOffset()));
                        }
                    }

                    intable("MUL CX");
                    stk.pop();
                    array_SI.pop();

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    intable("MOV SI,[BX+",array_SI.top(),"]");
                    intable("MOV [BX+",stk.top(),"+SI],AX");
                    array_SI.pop();
                    stk.pop();

                    if(fst.getPosition()>=0)
                    {
                        intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
                    }
                    else if(fst.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                        intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
                    }

                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("MOV CX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("MOV CX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("MOV CX,",floatList.get(sed.getOffset()));
                        }
                    }

                    intable("MUL CX");

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }

            }

        }

        else
        {
            if(stk.top()==synbl.getOffset(fst.getOffset())&&fst.getActivity()==false)
            {
                if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(sed.getPosition()>=0)
                    {
                        intable("MOV CX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                        intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                    }
                }
                else
                {
                    if(sed.getCategory()==Token::id)
                    {
                        //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                        intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                    }
                    else if(sed.getCategory()==Token::integer)
                    {
                        //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                        intable("MOV CX,",intList.get(sed.getOffset()));
                    }
                    else if(sed.getCategory()==Token::real)
                    {
                        //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                        intable("MOV CX,",floatList.get(sed.getOffset()));
                    }
                }

                intable("MUL CX");
                stk.pop();

                if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(tar.getPosition()>=0)
                    {
                        stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                    }
                    else if(tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(tar.getOffset()));
                }
            }
            else
            {
                //cout << "MOV " << "[L+" << stk.top() << "],AX" << endl ;
                intable("MOV [BX+",stk.top(),"],AX");
                stk.pop();

                if(fst.getCategory()==Token::id)
                {
                    //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                    intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"]");
                }
                else if(fst.getCategory()==Token::integer)
                {
                    // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",intList.get(fst.getOffset()));
                }
                else if(fst.getCategory()==Token::real)
                {
                    //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",floatList.get(fst.getOffset()));
                }

                if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(sed.getPosition()>=0)
                    {
                        intable("MOV CX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                        intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                    }
                }
                else
                {
                    if(sed.getCategory()==Token::id)
                    {
                        //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                        intable("MOV CX,[BX+",synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getCategory()==Token::integer)
                    {
                        //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                        intable("MOV CX,",intList.get(sed.getOffset()));
                    }
                    else if(sed.getCategory()==Token::real)
                    {
                        //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                        intable("MOV CX,",floatList.get(sed.getOffset()));
                    }
                }

                intable("MUL CX");

                if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(tar.getPosition()>=0)
                    {
                        stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                    }
                    else if(tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(tar.getOffset()));
                }
            }
        }

    }


}

void tra_div(Quaternary qt)//(/,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    if(stk.empty()==true)
    {
        if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(fst.getPosition()>=0)
            {
                intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
            }
        }
        else
        {
            if(fst.getCategory()==Token::id)
            {
                //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getCategory()==Token::integer)
            {
                // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",intList.get(fst.getOffset()));
            }
            else if(fst.getCategory()==Token::real)
            {
                //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                intable("MOV AX,",floatList.get(fst.getOffset()));
            }
        }

        if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(sed.getPosition()>=0)
            {
                intable("DIV AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
            }
            else if(sed.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
            }
        }
        else
        {
            if(sed.getCategory()==Token::id)
            {
                //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

            }
            else if(sed.getCategory()==Token::integer)
            {
                //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                intable("DIV AX,",intList.get(sed.getOffset()));
            }
            else if(sed.getCategory()==Token::real)
            {
                //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                intable("DIV AX,",floatList.get(sed.getOffset()));
            }
        }

        intable("OR AX,0FH");

        if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(tar.getPosition()>=0)
            {
                stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
            }
            else if(tar.getPosition()<0)
            {
                array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                stk.push(synbl.getOffset(tar.getOffset()));
            }
        }
        else
        {
            stk.push(synbl.getOffset(tar.getOffset()));
        }

    }
    else if(stk.empty()==false)
    {
        if(synbl.getCategory(fst.getOffset())==SymbolList::array)
        {
            if(fst.getPosition()>=0)
            {
                if(stk.top()==intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset())&&fst.getActivity()==false)
                {
                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("DIV AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("DIV AX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("DIV AX,",floatList.get(sed.getOffset()));
                        }
                    }

                    intable("OR AX,0FH");
                    stk.pop();

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    intable("MOV [BX+",stk.top(),"],AX");
                    stk.pop();

                    if(fst.getPosition()>=0)
                    {
                        intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
                    }
                    else if(fst.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                        intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
                    }

                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("DIV AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("DIV AX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("DIV AX,",floatList.get(sed.getOffset()));
                        }
                    }

                    intable("OR AX,0FH");

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
            }
            else if(fst.getPosition()<0)
            {
                if(stk.top()==synbl.getOffset(fst.getOffset())&&array_SI.top()==synbl.getOffset(-fst.getPosition())&&fst.getActivity()==false)
                {
                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("DIV AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("DIV AX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("DIV AX,",floatList.get(sed.getOffset()));
                        }
                    }

                    intable("OR AX,0FH");
                    stk.pop();
                    array_SI.pop();

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    intable("MOV SI,[BX+",array_SI.top(),"]");
                    intable("MOV [BX+",stk.top(),"+SI],AX");
                    array_SI.pop();
                    stk.pop();

                    if(fst.getPosition()>=0)
                    {
                        intable("MOV AX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
                    }
                    else if(fst.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                        intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
                    }

                    if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(sed.getPosition()>=0)
                        {
                            intable("DIV AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                        }
                        else if(sed.getPosition()<0)
                        {
                            intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                            intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                        }
                    }
                    else
                    {
                        if(sed.getCategory()==Token::id)
                        {
                            //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                            intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                        }
                        else if(sed.getCategory()==Token::integer)
                        {
                            //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                            intable("DIV AX,",intList.get(sed.getOffset()));
                        }
                        else if(sed.getCategory()==Token::real)
                        {
                            //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                            intable("DIV AX,",floatList.get(sed.getOffset()));
                        }
                    }

                    intable("OR AX,0FH");

                    if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                    {
                        if(tar.getPosition()>=0)
                        {
                            stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                        }
                        else if(tar.getPosition()<0)
                        {
                            array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                            stk.push(synbl.getOffset(tar.getOffset()));
                        }
                    }
                    else
                    {
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }

            }

        }

        else
        {
            if(stk.top()==synbl.getOffset(fst.getOffset())&&fst.getActivity()==false)
            {
                if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(sed.getPosition()>=0)
                    {
                        intable("DIV AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                        intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                    }
                }
                else
                {
                    if(sed.getCategory()==Token::id)
                    {
                        //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                        intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"]");

                    }
                    else if(sed.getCategory()==Token::integer)
                    {
                        //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                        intable("DIV AX,",intList.get(sed.getOffset()));
                    }
                    else if(sed.getCategory()==Token::real)
                    {
                        //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                        intable("DIV AX,",floatList.get(sed.getOffset()));
                    }
                }

                intable("OR AX,0FH");
                stk.pop();

                if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(tar.getPosition()>=0)
                    {
                        stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                    }
                    else if(tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(tar.getOffset()));
                }
            }
            else
            {
                //cout << "MOV " << "[L+" << stk.top() << "],AX" << endl ;
                intable("MOV [BX+",stk.top(),"],AX");
                stk.pop();

                if(fst.getCategory()==Token::id)
                {
                    //cout << "MOV AX," << "[L+" << fst.getOffset() << "]" << endl ;
                    intable("MOV AX,[BX+",synbl.getOffset(fst.getOffset()),"]");
                }
                else if(fst.getCategory()==Token::integer)
                {
                    // cout << "MOV AX," << intList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",intList.get(fst.getOffset()));
                }
                else if(fst.getCategory()==Token::real)
                {
                    //cout << "MOV AX," << floatList.get(fst.getOffset()) << endl ;
                    intable("MOV AX,",floatList.get(fst.getOffset()));
                }

                if(synbl.getCategory(sed.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(sed.getPosition()>=0)
                    {
                        intable("DIV AX,[BX+",intList.get(sed.getPosition())+synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getPosition()<0)
                    {
                        intable("MOV SI,[BX+",synbl.getOffset(-sed.getPosition()),"]");
                        intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"+SI]");
                    }
                }
                else
                {
                    if(sed.getCategory()==Token::id)
                    {
                        //cout << "ADD AX," << "[L+" << sed.getOffset() << "]" << endl ;
                        intable("DIV AX,[BX+",synbl.getOffset(sed.getOffset()),"]");
                    }
                    else if(sed.getCategory()==Token::integer)
                    {
                        //cout << "ADD AX," << intList.get(sed.getOffset()) << endl ;
                        intable("DIV AX,",intList.get(sed.getOffset()));
                    }
                    else if(sed.getCategory()==Token::real)
                    {
                        //cout << "ADD AX," << floatList.get(sed.getOffset()) << endl ;
                        intable("DIV AX,",floatList.get(sed.getOffset()));
                    }
                }

                intable("OR AX,0FH");

                if(synbl.getCategory(tar.getOffset())==SymbolList::array)//判断是否为数组元素
                {
                    if(tar.getPosition()>=0)
                    {
                        stk.push(intList.get(tar.getPosition())+synbl.getOffset(tar.getOffset()));
                    }
                    else if(tar.getPosition()<0)
                    {
                        array_SI.push(synbl.getOffset(-tar.getPosition()));//下标变量地址压栈
                        stk.push(synbl.getOffset(tar.getOffset()));
                    }
                }
                else
                {
                    stk.push(synbl.getOffset(tar.getOffset()));
                }
            }
        }

    }

}

void tra_out(Quaternary qt)//(print,I1, , )
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()!=Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }
    fst = qt.getFirst();

    if(stk.empty()==true)
    {
        if(array_SI.empty()==false)
        {
            cout << "error" << endl ;
            exit(0);
        }

        if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(fst.getPosition()>=0)
            {

                intable("MOV DL,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                intable("MOV DL,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
            }
        }
        else
        {
            if(fst.getCategory()==Token::id)
            {
                intable("MOV DL,[BX+",synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getCategory()==Token::integer)
            {
                intable("MOV DL,",intList.get(fst.getOffset()));
            }
            else if(fst.getCategory()==Token::real)
            {
                intable("MOV DL,",floatList.get(fst.getOffset()));
            }

        }

        intable("ADD DL,30H");
        intable("MOV AH,02H");
        intable("INT 21H");

    }
    else if(stk.empty()==false)
    {
        if(array_SI.empty()==true)
        {
            intable("MOV [BX+",stk.top(),"],AX");
            stk.pop();
        }
        else if(array_SI.empty()==false)
        {
            intable("MOV SI,[BX+",array_SI.top(),"]");
            intable("MOV [BX+",stk.top(),"+SI],AX");
            array_SI.pop();
            stk.pop();
        }

        if(stk.empty()==false||array_SI.empty()==false)
        {
            cout << "error" << endl ;
            exit(0);
        }

        if(array_SI.empty()==false)
        {
            cout << "error" << endl ;
            exit(0);
        }

        if(synbl.getCategory(fst.getOffset())==SymbolList::array)//判断是否为数组元素
        {
            if(fst.getPosition()>=0)
            {

                intable("MOV DX,[BX+",intList.get(fst.getPosition())+synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getPosition()<0)
            {
                intable("MOV SI,[BX+",synbl.getOffset(-fst.getPosition()),"]");
                intable("MOV DX,[BX+",synbl.getOffset(fst.getOffset()),"+SI]");
            }
        }
        else
        {
            if(fst.getCategory()==Token::id)
            {
                intable("MOV DX,[BX+",synbl.getOffset(fst.getOffset()),"]");
            }
            else if(fst.getCategory()==Token::integer)
            {
                intable("MOV DX,",intList.get(fst.getOffset()));
            }
            else if(fst.getCategory()==Token::real)
            {
                intable("MOV DX,",floatList.get(fst.getOffset()));
            }

        }

        intable("ADD DL,30H");
        intable("MOV AH,02H");
        intable("INT 21H");


    }

}

void tra_end(Quaternary qt)//(endMain, , , )
{
    if(qt.getFirst().getCategory()!=Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()!=Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    if(stk.empty()==true)
    {
        if(array_SI.empty()==false)
        {
            cout << "error" << endl ;
            exit(0);
        }

        outen[io++]=_piece+to_string(i_piece++)+_maohao;
        intable("MOV AH,4CH");

        intable("INT 21H");

        intable("main ENDS");

        intable("END PIECE1");
    }
    else if(stk.empty()==false)
    {
        /*int j,n=stk.size();
        for(j=0;j<n;j++)
        {

        }*/
        //cout << "MOV " << "[L+" << stk.top() << "],AX" << endl ;
        if(array_SI.empty()==true)
        {
            intable("MOV [BX+",stk.top(),"],AX");
            stk.pop();
        }
        else if(array_SI.empty()==false)
        {
            intable("MOV SI,[BX+",array_SI.top(),"]");
            intable("MOV [BX+",stk.top(),"+SI],AX");
            array_SI.pop();
            stk.pop();
        }


        if(stk.empty()==false||array_SI.empty()==false)
        {
            //cout << stk.top() <<endl ;

            cout << "error" << endl ;
            exit(0);
        }

        outen[io++]=_piece+to_string(i_piece++)+_maohao;
        intable("MOV AH,4CH");
        intable("INT 21H");
        //cout << "INT 21H" << endl ;
        //cout << "RET" << endl ;
        //cout << "main ENDS" << endl ;
        intable("main ENDS");
        //cout << "ENDSTAR" << endl ;
        intable("END PIECE1");
    }

}

void tra_if(Quaternary qt)//(if,I1, , )
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()!=Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();

    if(stk.empty()==true||stk.top()!=synbl.getOffset(fst.getOffset()))
    {
        //intable("MOV [L+",stk.top(),"],AX");
        //stk.pop();
        //cout<<stk.top()<<","<<fst.getOffset()<<endl;
        cout << "error" << endl ;
        exit(0);
    }

    stk.pop();


    if(stk.empty()==false)
    {
        //cout << stk.top() <<endl ;
        cout << "error" << endl ;
        exit(0);
    }


    if(_compare.top()==">")
    {


        intable("CMP AX,0");

        intable("JLE ");//不大于
        _io=io;
        _io--;
        sem.push(_io);
    }

    if(_compare.top()==">=")
    {

        intable("CMP AX,0");

        intable("JL ");//不大于等于
        _io=io;
        _io--;
        sem.push(_io);
    }

    if(_compare.top()=="<")
    {


        intable("CMP AX,0");

        intable("JGE ");//不小于
        _io=io;
        _io--;
        sem.push(_io);

    }

    if(_compare.top()=="<=")
    {


        intable("CMP AX,0");

        intable("JG ");//不小于等于
        _io=io;
        _io--;
        sem.push(_io);

    }

    if(_compare.top()=="==")
    {


        intable("CMP AX,0");

        intable("JNE ");//不等于
        _io=io;
        _io--;
        sem.push(_io);

    }

    if(_compare.top()=="!=")
    {

        intable("CMP AX,0");

        intable("JE ");//等于
        _io=io;
        _io--;
        sem.push(_io);

    }

    _compare.pop();

}

void tra_el(Quaternary qt)//(el, , , )
{
    if(qt.getFirst().getCategory()!=Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()!=Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    if(stk.empty()==true)
    {
        outen[sem.top()]=outen[sem.top()]+_piece+to_string(i_piece);//回填
        sem.pop();
        intable("JMP ");
        _io=io;
        _io--;
        sem.push(_io);

        outen[io++]=_piece+to_string(i_piece++)+_maohao;//下一个块的头
    }
    else if(stk.empty()==false)
    {
        if(array_SI.empty()==true)
        {
            intable("MOV [BX+",stk.top(),"],AX");
            stk.pop();
        }
        else if(array_SI.empty()==false)
        {
            intable("MOV SI,[BX+",array_SI.top(),"]");
            intable("MOV [BX+",stk.top(),"+SI],AX");
            array_SI.pop();
            stk.pop();
        }

        outen[sem.top()]=outen[sem.top()]+_piece+to_string(i_piece);//回填
        sem.pop();
        intable("JMP ");
        _io=io;
        _io--;
        sem.push(_io);

        outen[io++]=_piece+to_string(i_piece++)+_maohao;//下一个块的头
    }

}

void tra_ie(Quaternary qt)//(ie, , , )
{
    if(qt.getFirst().getCategory()!=Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()!=Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    if(stk.empty()==true)
    {
        if(array_SI.empty()==false)
        {
            cout << "error" << endl ;
            exit(0);
        }
        outen[sem.top()]=outen[sem.top()]+_piece+to_string(i_piece);//回填
        sem.pop();

        outen[io++]=_piece+to_string(i_piece++)+_maohao;//下一个块的头
    }
    else if(stk.empty()==false)
    {
        if(array_SI.empty()==true)
        {
            intable("MOV [BX+",stk.top(),"],AX");
            stk.pop();
        }
        else if(array_SI.empty()==false)
        {
            intable("MOV SI,[BX+",array_SI.top(),"]");
            intable("MOV [BX+",stk.top(),"+SI],AX");
            array_SI.pop();
            stk.pop();
        }


        if(stk.empty()==false||array_SI.empty()==false)
        {
            //cout << stk.top() <<endl ;
            cout << "error" << endl ;
            exit(0);
        }

        outen[sem.top()]=outen[sem.top()]+_piece+to_string(i_piece);//回填
        sem.pop();

        outen[io++]=_piece+to_string(i_piece++)+_maohao;//下一个块的头
    }

}

void tra_while(Quaternary qt)//(wh, , , )
{
    if(qt.getFirst().getCategory()!=Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()!=Token::null)
    {
        cout << "error1" << endl ;
        exit(0);
    }

    if(stk.empty()==true)
    {
        if(array_SI.empty()==false)
        {
            cout << "error" << endl ;
            exit(0);
        }

        outen[io++]=_piece+to_string(i_piece++)+_maohao;//本块头

        i_whilepiece=i_piece;
        i_whilepiece--;//将本块头存入
        whilepiece.push(i_whilepiece);
    }

    if(stk.empty()==false)
    {
        if(array_SI.empty()==true)
        {
            intable("MOV [BX+",stk.top(),"],AX");
            stk.pop();
        }
        else if(array_SI.empty()==false)
        {
            intable("MOV SI,[BX+",array_SI.top(),"]");
            intable("MOV [BX+",stk.top(),"+SI],AX");
            array_SI.pop();
            stk.pop();
        }

        outen[io++]=_piece+to_string(i_piece++)+_maohao;//本块头

        i_whilepiece=i_piece;
        i_whilepiece--;//将本块头存入
        whilepiece.push(i_whilepiece);
    }

}

void tra_do(Quaternary qt)//(do,I1, , )
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()!=Token::null)
    {
        cout << "error3" << endl ;
        exit(0);
    }

    fst=qt.getFirst();

    if(stk.empty()==true||stk.top()!=synbl.getOffset(fst.getOffset()))
    {
        //intable("MOV [L+",stk.top(),"],AX");
        //stk.pop();
        cout << "error4" << endl ;
        exit(0);
    }

    stk.pop();

    if(stk.empty()==false)
    {
        //cout << stk.top() <<endl ;
        cout << "error4" << endl ;
        exit(0);
    }



    if(_compare.top()==">")
    {


        intable("CMP AX,0");

        intable("JLE ");//不大于
        _io=io;
        _io--;
        sem.push(_io);
    }

    if(_compare.top()==">=")
    {

        intable("CMP AX,0");

        intable("JL ");//不大于等于
        _io=io;
        _io--;
        sem.push(_io);
    }

    if(_compare.top()=="<")
    {

        intable("CMP AX,0");

        intable("JGE ");//不小于
        _io=io;
        _io--;
        sem.push(_io);

    }

    if(_compare.top()=="<=")
    {

        intable("CMP AX,0");

        intable("JG ");//不小于等于
        _io=io;
        _io--;
        sem.push(_io);

    }

    if(_compare.top()=="==")
    {

        intable("CMP AX,0");

        intable("JNE ");//不等于
        _io=io;
        _io--;
        sem.push(_io);

    }

    if(_compare.top()=="!=")
    {

        intable("CMP AX,0");

        intable("JE ");//等于
        _io=io;
        _io--;
        sem.push(_io);

    }

    _compare.pop();

}

void tra_we(Quaternary qt)//(we, , , )
{
    if(qt.getFirst().getCategory()!=Token::null||qt.getSecond().getCategory()!=Token::null
       ||qt.getTarget().getCategory()!=Token::null)
    {
        cout << "error5" << endl ;
        exit(0);
    }

    if(stk.empty()==true)
    {
        if(array_SI.empty()==false)
        {
            cout << "error" << endl ;
            exit(0);
        }
        outen[sem.top()]=outen[sem.top()]+_piece+to_string(i_piece);//回填
        sem.pop();

        outen[io++]=_jmp+_piece+to_string(whilepiece.top());//循环
        whilepiece.pop();

        outen[io++]=_piece+to_string(i_piece++)+_maohao;//下一个块的头

    }
    else if(stk.empty()==false)
    {
        if(array_SI.empty()==true)
        {
            intable("MOV [BX+",stk.top(),"],AX");
            stk.pop();
        }
        else if(array_SI.empty()==false)
        {
            intable("MOV SI,[BX+",array_SI.top(),"]");
            intable("MOV [BX+",stk.top(),"+SI],AX");
            array_SI.pop();
            stk.pop();
        }

        if(stk.empty()==false)
        {
            //cout << stk.top() <<endl ;
            cout << "error6" << endl ;
            exit(0);
        }

        outen[sem.top()]=outen[sem.top()]+_piece+to_string(i_piece);//回填
        sem.pop();

        outen[io++]=_jmp+_piece+to_string(whilepiece.top());//循环
        whilepiece.pop();

        outen[io++]=_piece+to_string(i_piece++)+_maohao;//下一个块的头
    }

}

void tra_greater(Quaternary qt)//(>,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    _SUB(fst,sed,tar);//计算fst-sed
    _compare.push(">");//将>入栈


}

void tra_grequal(Quaternary qt)//(>=,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    _SUB(fst,sed,tar);//计算fst-sed
    _compare.push(">=");//将>=入栈


}

void tra_less(Quaternary qt)//(<,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    _SUB(fst,sed,tar);//计算fst-sed
    _compare.push("<");//将<入栈


}

void tra_lesequal(Quaternary qt)//(<=,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    _SUB(fst,sed,tar);//计算fst-sed
    _compare.push("<=");//将<=入栈

}

void tra_realequal(Quaternary qt)//(==,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    _SUB(fst,sed,tar);//计算fst-sed
    _compare.push("==");//将==入栈

}

void tra_unequal(Quaternary qt)//(!=,I1,I2,I3)
{
    if(qt.getFirst().getCategory()==Token::null||qt.getSecond().getCategory()==Token::null
       ||qt.getTarget().getCategory()==Token::null)
    {
        cout << "error" << endl ;
        exit(0);
    }

    fst=qt.getFirst();
    sed=qt.getSecond();
    tar=qt.getTarget();

    _SUB(fst,sed,tar);//计算fst-sed
    _compare.push("!=");//将!=入栈

}
