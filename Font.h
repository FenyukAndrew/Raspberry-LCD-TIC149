#include "common_header.h"

class View_LCD;

#include "sSymbol.h"

class Font
{
public:
    Font(const unsigned char PART_COUNT) : value_PART_COUNT(PART_COUNT)
    {};

    unsigned short get_width_symbol(const unsigned char code_symbol) const;
    void DrawTo(const short x,const short y,View_LCD& m_View_LCD,const unsigned char code_symbol,const bool transparency,const unsigned char color=DEFAULT_COLOR) const; //Рисование символа

    /*sSymbol& get_Symbol(unsigned char code)
    {
        return p_Font[code];
    };*/

    void set_Symbol(unsigned char code,sSymbol* symbol)
    {
        p_Font[code]=symbol;
    };

private:
    sSymbol* p_Font[256];

    const unsigned char value_PART_COUNT;
};
