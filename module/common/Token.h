#ifndef _TOKEN_H
#define _TOKEN_H

class Token {
public:
    enum Categories {
        keyword, id, integer, real, ch, str, symbol, null
    };
    Token() { _offset = 0; _category = null; _activity = true; _position = 0; }
    
    //setter
    void setCategory(Categories category);
    void setOffset(int offset);
    void setActivity(bool activity);
    void setPosition(int position);

    //getter
    Categories getCategory();
    int getOffset();
    bool getActivity();
    int getPosition();

private:
    Categories _category;
    int _offset;
    bool _activity;
    int _position;
};

#endif

