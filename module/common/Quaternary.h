#ifndef _QUATERNARY_H
#define _QUATERNARY_H

#include <iostream>
#include "Token.h"

class Quaternary {
public:
    void setOption(std::string option);
    void setFirst(Token token);
    void setSecond(Token token);
    void setTarget(Token token);

    std::string getOption();
    Token getFirst();
    Token getSecond();
    Token getTarget();
private:
    std::string _option;
    Token _first, _second, _target;
};

#endif
