#ifndef _TOKEN_H
#define _TOKEN_H

class Token {
public:
    enum Categories {
        keyword, id, integer, real, ch, str, null
    };
    Token() { _offset = 0; _category = null; }
    //setter
    void setCategory(Categories category);
    void setOffset(int offset);
    //getter
    Categories getCategory();
    int getOffset();

private:
    Categories _category;
    int _offset;
};

#endif

