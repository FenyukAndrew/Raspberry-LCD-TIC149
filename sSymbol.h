#include "common_header.h"

class View_LCD;

//Универсальная структура данных для всех шрифтов
struct sSymbol 
{
		unsigned short width;
        const unsigned char* b;          // Data

//Символ рисует сам себя
        void DrawTo(const short x,const short y,View_LCD& m_View_LCD,const unsigned char value_PART_COUNT,const bool transparency,const unsigned char color=DEFAULT_COLOR) const;
};
