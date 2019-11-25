#include "Font.h"

Font::Font(const unsigned char* data_font)
{
    value_PART_COUNT=data_font[0];//Число проходов при рисовании символа
    const unsigned char* table_width=data_font+1;//Таблица размеров символов
    const unsigned char* ptr_data=table_width+256;//C 257 байта начинается графика символа
    for(unsigned short i=0;i<number_symbols;i++)
    {
        const unsigned char width=table_width[i];
        p_Font[i]=new sSymbol(table_width[i], ptr_data);
        ptr_data+=width*value_PART_COUNT;
    }
}

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

