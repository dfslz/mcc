#include "Quaternary.h"
#include "Common.h"

void translate();

void intable(char _oa[100],int _ob,char _oc[100]);
void intable(char _oa[100],int _ob);
void intable(char _oa[100]);

void tra_main(Quaternary qt);//main
void tra_equal(Quaternary qt);//=
void tra_add(Quaternary qt);//+
void tra_sub(Quaternary qt);//-
void tra_mul(Quaternary qt);//*
void tra_div(Quaternary qt);// /
void tra_out(Quaternary qt);//
void tra_end(Quaternary qt);//endMain
void tra_if(Quaternary qt);//if
void tra_el(Quaternary qt);//else
void tra_ie(Quaternary qt);//ie
void tra_while(Quaternary qt);//while
void tra_do(Quaternary qt);//do
void tra_we(Quaternary qt);//we
void tra_greater(Quaternary qt);//>
void tra_grequal(Quaternary qt);//>=
void tra_less(Quaternary qt);//<
void tra_lesequal(Quaternary qt);//<=
void tra_realequal(Quaternary qt);//==
void tra_unequal(Quaternary qt);// !=

void _SUB(Token _fst,Token _sed,Token _tar);//用于额外的减法运算

