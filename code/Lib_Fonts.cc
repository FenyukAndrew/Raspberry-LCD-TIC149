#include "Lib_Fonts.h"
#include "Font.h"

#include "Utils.h"

#include "Arial_8.h"
#include "Tahoma_16.h"
#include "Tahoma_24.h"
#include "Tahoma_32.h"

Lib_Fonts::Lib_Fonts()
{
    m_Fonts=new std::map<Fnt,Font*>();
    //m_Fonts->insert ( std::pair<Fnt,Font*>(Fnt::h8,font_Arial_8) );
    (*m_Fonts)[Fnt::h8]=new Font(Arial_8);
    (*m_Fonts)[Fnt::h16]=new Font(Tahoma_16);
    (*m_Fonts)[Fnt::h24]=new Font(Tahoma_24);
    (*m_Fonts)[Fnt::h32]=new Font(Tahoma_32);
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
