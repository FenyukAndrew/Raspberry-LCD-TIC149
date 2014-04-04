#include "Font.h"

unsigned short Font::get_width_symbol(const unsigned char code_symbol) const
{
    sSymbol* cur=p_Font[code_symbol];
    if (!cur) return 0;

    return cur->get_width();
};

void Font::DrawTo(const short x,const short y,View_LCD& m_View_LCD,const unsigned char code_symbol,const bool transparency,const unsigned char color) const //Рисование символа
{
    if (!p_Font[code_symbol]) return;//Нет символа
    p_Font[code_symbol]->DrawTo(x,y,m_View_LCD,value_PART_COUNT,transparency,color);
};

