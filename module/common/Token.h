#ifndef _TOKEN_H
#define _TOKEN_H

class Token {
public:
    enum Categories {
        keyword, id, integer, real, ch, str, symbol, null
    };
    Token() { _offset = 0; _category = null; _activity = true; }
    
    //setter
    void setCategory(Categories category);
    void setOffset(int offset);
    void setActivity(bool activity);

    //getter
    Categories getCategory();
    int getOffset();
    bool getActivity();

private:
    Categories _category;
    int _offset;
    bool _activity;
};

#endif

