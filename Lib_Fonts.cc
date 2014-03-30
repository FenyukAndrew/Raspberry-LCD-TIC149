#include "Lib_Fonts.h"
#include "Font.h"

#include "Utils.h"

#include "description_font.h"
extern sUnit8 Arial_8[256];
extern sUnit16 Tahoma_16[256];
extern sUnit24 Tahoma_24[256];
extern sUnit32 Tahoma_32[256];

Lib_Fonts::Lib_Fonts()
{
    //Некрасиво - но такие входные данные
    //Приведение формата шрифтов - в цикле, потому что разные типы входных данных
    Font* font_Arial_8=new Font(PART_COUNT8);
    Font* font_Tahoma_16=new Font(PART_COUNT16);
    Font* font_Tahoma_24=new Font(PART_COUNT24);
    Font* font_Tahoma_32=new Font(PART_COUNT32);
    for(int i=0; i<256; i++)
    {
        sUnit8& u8=Arial_8[i];
        font_Arial_8->set_Symbol(i,
                                 new sSymbol(u8.sH.cyPix, u8.b));

        sUnit16& u16=Tahoma_16[i];
        font_Tahoma_16->set_Symbol(i,
                                 new sSymbol(u16.sH.cyPix, u16.b));

        sUnit24& u24=Tahoma_24[i];
        font_Tahoma_24->set_Symbol(i,
                                 new sSymbol(u24.sH.cyPix, u24.b));

        sUnit32& u32=Tahoma_32[i];
        font_Tahoma_32->set_Symbol(i,
                                 new sSymbol(u32.sH.cyPix, u32.b));
    }

    m_Fonts=new std::map<Fnt,Font*>();
    //m_Fonts->insert ( std::pair<Fnt,Font*>(Fnt::h8,font_Arial_8) );
    (*m_Fonts)[Fnt::h8]=font_Arial_8;
    (*m_Fonts)[Fnt::h16]=font_Tahoma_16;
    (*m_Fonts)[Fnt::h24]=font_Tahoma_24;
    (*m_Fonts)[Fnt::h32]=font_Tahoma_32;
}

Lib_Fonts::~Lib_Fonts()
{
}

Font* Lib_Fonts::get_Font(Fnt e_Fnt)
{
    return m_Fonts->find(e_Fnt)->second;
}

unsigned short Lib_Fonts::get_width(Fnt e_Fnt,const std::string& str)
{
    const std::string str1=Utils::recodeUTF8toCP1251(str);//CP1251

    Font* cur_font=get_Font(e_Fnt);

    unsigned short width=0;

    for(unsigned short pos_str=0; pos_str<str1.length(); pos_str++) //проходим по всем символам строки
    {
        const unsigned char symbol=str1[pos_str];
        width+=cur_font->get_width_symbol(symbol);
    }
    return width;
}
