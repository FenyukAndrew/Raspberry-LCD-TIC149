#include "View_LCD.h"
#include "Row_LCD.h"
#include "Font.h"
#include "EasyBMP/EasyBMP.h"

#include "Utils.h"

#include "description_font.h"
extern sUnit8 Arial_8[256];
extern sUnit16 Tahoma_16[256];
extern sUnit24 Tahoma_24[256];
extern sUnit32 Tahoma_32[256];

View_LCD::View_LCD(const unsigned short _columns,const unsigned short _rows) : rows_in_screen(_rows), columns_in_row(_columns)
{
    rows_LCD.reserve(_rows);
    for(int i=0;i<_rows;i++)
    {
        rows_LCD.push_back(new Row_LCD(_columns));
    }
    /*rows_LCD=new Row_LCD*[_rows];
    for(int i=0;i<_rows;i++)
    {
        rows_LCD[i]=new Row_LCD(_columns);
    }*/

    //Приведение формата шрифтов - в цикле, потому что разные типы входных данных
    font_Arial_8=new Font(PART_COUNT8);
    font_Tahoma_16=new Font(PART_COUNT16);
    font_Tahoma_24=new Font(PART_COUNT24);
    font_Tahoma_32=new Font(PART_COUNT32);
    for(int i=0;i<256;i++)
    {
      (font_Arial_8->p_Font[i]).width=(Arial_8[i]).sH.cyPix;
      (font_Arial_8->p_Font[i]).b=&((Arial_8[i]).b[0]);

      (font_Tahoma_16->p_Font[i]).width=(Tahoma_16[i]).sH.cyPix;
      (font_Tahoma_16->p_Font[i]).b=&((Tahoma_16[i]).b[0]);

      (font_Tahoma_24->p_Font[i]).width=(Tahoma_24[i]).sH.cyPix;
      (font_Tahoma_24->p_Font[i]).b=&((Tahoma_24[i]).b[0]);

      (font_Tahoma_32->p_Font[i]).width=(Tahoma_32[i]).sH.cyPix;
      (font_Tahoma_32->p_Font[i]).b=&((Tahoma_32[i]).b[0]);
    }

}
View_LCD::~View_LCD() 
{
    //for (std::vector<Row_LCD*>::iterator it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
    for (auto it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
    {
        //it->~row_LCD();
        delete(*it);
    }
    rows_LCD.resize(0);

    /*for(int i=0;i<rows_in_screen;i++)
    {
        delete rows_LCD[i];
    }
    delete[] rows_LCD;*/
    printf("Destructor View_LCD\n");
};

void View_LCD::clear_lcd()
{
    for (std::vector<Row_LCD*>::iterator it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
    {
        (*it)->clear_row();
    }
}

void View_LCD::point(const short x,const short y,const unsigned char _color)
{
    /*int _rows=_height/8;
    if (_rows>=rows_in_screen) return;//Нет такой строки
    rows_LCD[_rows]->point(_column,_height % 8,_color);*/
    //if (_rows>=rows_in_screen) return;//Нет такой строки
    //rows_LCD[_rows]->point(_column,_height,_color);

    if ((y<0) || (y>=rows_in_screen)) return;//Нет такой строки
    rows_LCD[y]->point(x,_color);
}

unsigned char View_LCD::get_color(const short x,const short y) const
{
    if ((y<0) || (y>=rows_in_screen)) return 0;//Нет такой строки
    return rows_LCD[y]->get_color(x);
}

/*void View_LCD::write_byte_to_buffer(const ushort _column,const ushort _rows,const unsigned char _data)
{
    if (_rows>=rows_in_screen) return;//Нет такой строки
    rows_LCD[_rows]->write(_column,_data);
}*/

//>>>>> РИСОВАНИЕ СИМВОЛОВ

//Вроде должно быть (offset_x_buffer<width) - проверить работу этого условия

//Даже лучше не шаблон, а универсальную функцию, вызываемую из нескльких других функций
void View_LCD::print_lcd_XX(short x,const short y,const std::string& strIn, const bool transparency, unsigned short width, const unsigned char color,const Font& m_Font)
{
    const std::string str1=Utils::recodeUTF8toCP1251(strIn);//CP1251
    //const char* str1=strCP1251.c_str();

    if ((width==MAX_WIDTH_LCD) || (width>columns_in_row)) 
    {
      width=columns_in_row-((x<0) ? 0 : x);//Оставшаяся ширина экран
    }

    for(unsigned short pos_str=0;pos_str<str1.length();pos_str++)//проходим по всем символам строки
    {
       const unsigned char symbol=str1[pos_str];
       const unsigned short symbol_width=m_Font.get_width_symbol(symbol);
       if ((x+symbol_width)>=width) break;//Обрезание производиться по символ - если нет места, то символ не выводиться
       m_Font.DrawTo(x,y,*this,symbol,transparency,color);
       x+=symbol_width;
    }
}

/*void View_LCD::print_lcd_8(const short x,const short y,const std::string& strIn, ushort width, const unsigned char _color)
{
  print_lcd_XX(x,y,strIn,width,_color,font_Arial_8);
}

void View_LCD::print_lcd_16(const short x,const short y,const std::string& strIn, ushort width, const unsigned char _color)
{
  print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_16);
}

void View_LCD::print_lcd_24(const short x,const short y,const std::string& strIn, ushort width, const unsigned char _color)
{
  print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_24);
}

void View_LCD::print_lcd_32(const short x,const short y,const std::string& strIn, ushort width, const unsigned char _color)
{
  print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_32);
}*/

void View_LCD::print_lcd(const Font_height _font_height,const short x,const short y,const std::string& strIn, const bool transparency, unsigned short width, const unsigned char _color)
{
  switch(_font_height)
  {
    case Font_height::h8:
      print_lcd_XX(x,y,strIn,transparency,width,_color,*font_Arial_8);
    break;
    case Font_height::h16:
      print_lcd_XX(x,y,strIn,transparency,width,_color,*font_Tahoma_16);
    break;
    case Font_height::h24:
      print_lcd_XX(x,y,strIn,transparency,width,_color,*font_Tahoma_24);
    break;
    case Font_height::h32:
      print_lcd_XX(x,y,strIn,transparency,width,_color,*font_Tahoma_32);
    break;
    default:
    break;
  }
}

void View_LCD::print_lcd(const unsigned char _font_height,const short x,const short y,const std::string& strIn, const bool transparency, unsigned short width, const unsigned char _color)
{
  switch(_font_height)
  {
    case 8:
      print_lcd_XX(x,y,strIn,transparency,width,_color,*font_Arial_8);
    break;
    case 16:
      print_lcd_XX(x,y,strIn,transparency,width,_color,*font_Tahoma_16);
    break;
    case 24:
      print_lcd_XX(x,y,strIn,transparency,width,_color,*font_Tahoma_24);
    break;
    case 32:
      print_lcd_XX(x,y,strIn,transparency,width,_color,*font_Tahoma_32);
    break;
    default:
      std::cerr<<"Error height in function print_lcd="<<(short)_font_height<<std::endl;
    break;
  }
}

void View_LCD::debug_output_console() const
{
    for (auto it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
    {
        (*it)->debug_output_console();
    }
}

void View_LCD::debug_output_console_compact() const
{
    //Т.к. строка 8 пикселей в высоту, для печати используются 4 символа: ▀▄█  
    const std::string symbols[4]={" ","▄","▀","█"};
    for (auto it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
    {
      const unsigned char* row_up=(*it)->get_row();
      ++it;
      if (it == rows_LCD.end()) return;
      const unsigned char* row_down=(*it)->get_row();

      for(ushort i=0;i<columns_in_row;i++)
      {
        unsigned char b=((row_up[i]!=0) ? 2 : 0) | ((row_down[i]!=0) ? 1 : 0);
        std::cout<<symbols[b];
      }
      std::cout<<std::endl;
    }
}

void View_LCD::save_to_bmp(BMP& AnImage) const
{
    AnImage.SetSize(columns_in_row,rows_in_screen);
    unsigned short y=0;
    for (auto it = rows_LCD.begin() ; it != rows_LCD.end(); ++it, y++)
    {
        (*it)->save_to_bmp(y,AnImage);
    }
}

void View_LCD::line(short x1, short y1, const short x2, const short y2,const unsigned char color_code) 
{
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const char signX = x1 < x2 ? 1 : -1;
    const char signY = y1 < y2 ? 1 : -1;
    //
    int error = deltaX - deltaY;
    //
    point(x2, y2,color_code);
    while(x1 != x2 || y1 != y2) 
    {
        point(x1, y1,color_code);
        const int error2 = error * 2;
        //
        if(error2 > -deltaY) 
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX) 
        {
            error += deltaX;
            y1 += signY;
        }
    }
 
}

/* Вспомогательная функция, печатает точки, определяющие окружность */
void View_LCD::plot_circle(const int x, const int y, const short x_center, const short y_center, const unsigned char color_code)
{
  //Из-за преобразования типов, может неверно отображаться
    point(x_center+x,y_center+y,color_code);
    point(x_center+x,y_center-y,color_code);
    point(x_center-x,y_center+y,color_code);
    point(x_center-x,y_center-y,color_code);
}
 
/* Вычерчивание окружности с использованием алгоритма Брезенхэма */
void View_LCD::circle(const short x_center, const short y_center, const short radius, const unsigned char color_code)
{
    int x = 0;
    int y = radius;
    int delta=3-2*radius;
    while(x<y) 
    {
        plot_circle(x,y,x_center,y_center,color_code);
        plot_circle(y,x,x_center,y_center,color_code);
        if (delta<0)
            delta+=4*x+6;
        else 
        {
            delta+=4*(x-y)+10;
            y--;
        }
        x++;
    }
 
    if(x==y) plot_circle(x,y,x_center,y_center,color_code);
}

//#include <utility>

void View_LCD::rectangle(short x1, short y1, short x2, short y2, const unsigned char color_code)
{
  if (x2<x1) std::swap(x1,x2);
  if (y2<y1) std::swap(y1,y2);
  const short x1_original=x1;
  do
  {
    point(x1,y1,color_code);
    point(x1,y2,color_code);
    x1++;
  }
  while(x1<x2);
  do
  {
    point(x1_original,y1,color_code);
    point(x2,y1,color_code);
    y1++;
  }
  while(y1<y2);
}

//                    columns (x) m_View_LCD.columns_in_row
//        0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
//        1                                                   
//rows(y) 2                                                   
//        3                                                   
//        4                                                   
//        5                                                   
//        6                                    (x,y)          rows_in_screen
//        7                                     XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//        8                                     XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  columns_in_row
//        9                                     XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//        10                                                   
//        11                                                   
//        12                                                   
//        13                                                   
//        14                                                   
//        15                                                   
//        16                                                   
//        17                                                   
//        18                                                   
//        19                                                   
//        20                                                   

  //Нужно реальное вычисление перекрытия, чтобы не пытаться выводить ненужные пикселы
  //Для расчета потребуется пересчет из одной в другую систему ккординат - разница между системами координат (x,y)
/*
По осям X и Y можно объединить и рассматривать их отдельно.
По оси X можно свести к случаям:

x1,x2 - откуда копируются данные
x1_to,x2_to - куда копируются данные
1 случай==========================

************** x1

************** x2

-------------- x1_to

-------------- x2_to

Ничего не выводиться - нет пересечения
2 случай==========================

************** x1

-------------- x1_to
XXXXXXXXXXXXXXXXXXXXX
************** x2

-------------- x2_to

x_begin=std::max(x1,x1_to)
x_end  =std::min(x2,x2_to)
3 случай==========================

-------------- x1_to

************** x1
XXXXXXXXXXXXXXXXXXXXX
************** x2

-------------- x2_to

4 случай==========================

-------------- x1_to

************** x1
XXXXXXXXXXXXXXXXXXXXX
-------------- x2_to

************** x2

5 случай==========================

-------------- x1_to

-------------- x2_to

************** x1

************** x2

Ничего не выводиться - нет пересечения
*/

void View_LCD::drawTo(const short x,const short y,View_LCD& m_View_LCD,const bool transparency) const
{
//Координаты экрана откуда копируются данные
  const short x1=x;
  const short y1=y;
  const short x2=x+columns_in_row;
  const short y2=y+rows_in_screen;

//Координаты экрана куда копируются данные
  const short x1_to=0;
  const short y1_to=0;
  const short x2_to=m_View_LCD.columns_in_row;
  const short y2_to=m_View_LCD.rows_in_screen;

//Выход за пределы экрана
  if (x2<x1_to) return;
  if (y2<y1_to) return;
  if (x1>=x2_to) return;
  if (y1>=y2_to) return;

  const short x_begin=std::max(x1,x1_to);
  const short x_end  =std::min(x2,x2_to);
  const short y_begin=std::max(y1,y1_to);
  const short y_end  =std::min(y2,y2_to);

  for(short _y1=y_begin;_y1<y_end;_y1++)
  {
    for(short _x1=x_begin;_x1<x_end;_x1++)
    {
        unsigned char _color=get_color(_x1-x,_y1-y);//Пересчет к координатам экрана источника
        if ((transparency)&&(_color==0))
        {//Если прозрачен - 0 цвет не выводится
        }
        else
        {
          m_View_LCD.point(_x1,_y1,_color);
        }
    }
  }

  //Без оптимизации
  /*for(short _y1=0;_y1<rows_in_screen;_y1++)
  {
    for(short _x1=0;_x1<columns_in_row;_x1++)
    {
        unsigned char _color=get_color(_x1,_y1);
        if ((transparency)&&(_color==0))
        {//Если прозрачен - 0 цвет не выводится
        }
        else
        {
          m_View_LCD.point(x+_x1,y+_y1,_color);
        }
    }
  }*/
}