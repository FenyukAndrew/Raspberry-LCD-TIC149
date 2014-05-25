#ifndef FONT_H
#define FONT_H

#include "common_header.h"
#include "sSymbol.h"

class View_LCD;

class Font
{
public:
    Font(const unsigned char* data_font);

    unsigned short get_width_symbol(const unsigned char code_symbol) const;
    void DrawTo(const short x,const short y,View_LCD& m_View_LCD,const unsigned char code_symbol,const bool transparency,const unsigned char color=DEFAULT_COLOR) const; //Рисование символа

    sSymbol* get_Symbol(unsigned char code)
    {
        return p_Font[code];
    };

    unsigned char get_PART_COUNT()
    {
        return value_PART_COUNT;
    };

    /*void set_Symbol(unsigned char code,sSymbol* symbol)
    {
        p_Font[code]=symbol;
    };*/

private:
    const static unsigned short number_symbols=256;
    sSymbol* p_Font[number_symbols];

    unsigned char value_PART_COUNT;
};

#endif // FONT_H
