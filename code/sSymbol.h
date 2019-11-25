#ifndef SSYMBOL_H
#define SSYMBOL_H

#include "common_header.h"

class View_LCD;

//Универсальная структура данных для всех шрифтов
class sSymbol
{
public:
    sSymbol(const unsigned short n_width,const unsigned char* n_b) : width(n_width) , b(n_b) {};

    /*void set_width(unsigned short new_width) {width=new_width;};
    void set_data(const unsigned char* new_b) {b=new_b;};*/
    const unsigned char* get_data() {return b;};

    unsigned short get_width() const {return width;};
//Символ рисует сам себя
    void DrawTo(const short x,const short y,View_LCD& m_View_LCD,const unsigned char value_PART_COUNT,const bool transparency,const unsigned char color=DEFAULT_COLOR) const;

private:
    const unsigned short width;
    const unsigned char* b;          // Data
};

#endif // SSYMBOL_H
