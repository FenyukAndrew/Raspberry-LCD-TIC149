#include "common_header.h"

class View_LCD;

#include "sSymbol.h"
//class sSymbol;

class Font
{
public:
  Font(const unsigned char PART_COUNT) : value_PART_COUNT(PART_COUNT)
  {};

  unsigned short get_width_symbol(const unsigned char code_symbol) const 
  {
  	return p_Font[code_symbol].width;
  };
  void DrawTo(const short x,const short y,View_LCD& m_View_LCD,const unsigned char code_symbol,const bool transparency,const unsigned char color=DEFAULT_COLOR) const //Рисование символа
  {
  	 p_Font[code_symbol].DrawTo(x,y,m_View_LCD,value_PART_COUNT,transparency,color);
  };

//private:
  sSymbol p_Font[256];
  //Может лучше массив указателей? тогда и инициализацию проще сделать
private:
  const unsigned char value_PART_COUNT;
};
