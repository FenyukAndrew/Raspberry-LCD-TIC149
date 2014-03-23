#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <iconv.h>

#include "I2CBus.h"
#include "LCD_TIC149.h"

//Общий для всех шрифтов, сгенерированных LCDicon
struct sHeader
   {         
        //const char ccChar;              // optional char - Номер символа
        const int cxPix;//Повторяется для всех символов
        const int cyPix;//Необходимое количество выводимых байт (т.е. 19-x)
   };

template<int T>
struct sUnitN {
        const sHeader sH;               // header 
        const unsigned char b[T];          // Data - Верхняя строка все нечётные - нижняя все чётные 
};
constexpr unsigned char PART_COUNT8=1;
constexpr unsigned char PART_COUNT16=2;
constexpr unsigned char PART_COUNT24=3;
constexpr unsigned char PART_COUNT32=4;
//Маскимальная длина символа высотой 24 - определять по программе - скорее всего буква Щ
constexpr unsigned int SYM_8_LONG=20;
constexpr unsigned int SYM_16_LONG=30;
constexpr unsigned int SYM_24_LONG=40;//28
constexpr unsigned int SYM_32_LONG=50;
typedef sUnitN<SYM_8_LONG*PART_COUNT8> sUnit8;
typedef sUnitN<SYM_16_LONG*PART_COUNT16> sUnit16;
typedef sUnitN<SYM_24_LONG*PART_COUNT24> sUnit24;
typedef sUnitN<SYM_32_LONG*PART_COUNT32> sUnit32;

//Получаются глобальные переменные
#include "Arial_8.h"
#include "Tahoma_16.h"
#include "Tahoma_24.h"
#include "Tahoma_32.h"

Font font_Arial_8(PART_COUNT8);
Font font_Tahoma_16(PART_COUNT16);
Font font_Tahoma_24(PART_COUNT24);
Font font_Tahoma_32(PART_COUNT32);

        void sSymbol::DrawTo(const ushort x,const ushort y,View_LCD& m_View_LCD,const unsigned char value_PART_COUNT,const unsigned char color) const
        {
        for(unsigned char i_lcd = 0; i_lcd < value_PART_COUNT; i_lcd++)//Делаем в несколько проходов
        {
                ushort offset_x_buffer=x;//133 точки - от 0 до 132 , но полусается начинаем с 1 - т.к. вначале идёт ++             
                for(int count = (width-1)*value_PART_COUNT; count >= 0; count-=value_PART_COUNT)
                {
                    //(rows_LCD[y+i_lcd])->write(offset_x_buffer,b[count+i_lcd]);
                    m_View_LCD.write_byte_to_buffer(offset_x_buffer,y+i_lcd,b[count+i_lcd]);
                    offset_x_buffer++;
                }
            }
                /*while ((++pos_lcd)<=133)
                {    
                           if (!INVERT)
                           { 
                              mI2CBus->write_to_buffer(Black_New_24[symbol].b[count+i_lcd]);
                           }
                           else
                           { 
                              mI2CBus->write_to_buffer(~Black_New_24[symbol].b[count+i_lcd]);
                           }
                         }
                }*/
        };

View_LCD::View_LCD(const ushort _columns,const ushort _rows) : rows_in_screen(_rows), columns_in_row(_columns)//height_LCD/8
{
    rows_LCD.reserve(_rows);
    for(int i=0;i<_rows;i++)
    {
        rows_LCD.push_back(new row_LCD(_columns));
    }
    /*rows_LCD=new row_LCD*[_rows];
    for(int i=0;i<_rows;i++)
    {
        rows_LCD[i]=new row_LCD(_columns);
    }*/

    //Приведение формата шрифтов - в цикле, потому что разные типы входных данных
    for(int i=0;i<256;i++)
    {
      (font_Arial_8.p_Font[i]).width=(Arial_8[i]).sH.cyPix;
      (font_Arial_8.p_Font[i]).b=&((Arial_8[i]).b[0]);

      (font_Tahoma_16.p_Font[i]).width=(Tahoma_16[i]).sH.cyPix;
      (font_Tahoma_16.p_Font[i]).b=&((Tahoma_16[i]).b[0]);

      (font_Tahoma_24.p_Font[i]).width=(Tahoma_24[i]).sH.cyPix;
      (font_Tahoma_24.p_Font[i]).b=&((Tahoma_24[i]).b[0]);

      (font_Tahoma_32.p_Font[i]).width=(Tahoma_32[i]).sH.cyPix;
      (font_Tahoma_32.p_Font[i]).b=&((Tahoma_32[i]).b[0]);
    }

}
View_LCD::~View_LCD() 
{
    //for (std::vector<row_LCD*>::iterator it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
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
    for (std::vector<row_LCD*>::iterator it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
    {
        (*it)->clear_row();
    }
}

void View_LCD::point(const ushort _column,const ushort _height,const unsigned char _color)
{
    int _rows=_height/8;
    if (_rows>=rows_in_screen) return;//Нет такой строки
    rows_LCD[_rows]->point(_column,_height % 8,_color);
}

void View_LCD::write_byte_to_buffer(const ushort _column,const ushort _rows,const unsigned char _data)
{
    if (_rows>=rows_in_screen) return;//Нет такой строки
    rows_LCD[_rows]->write(_column,_data);
}

std::string View_LCD::iconv_recode(const std::string& from, const std::string& to, std::string text)
{
    iconv_t cnv = iconv_open(to.c_str(), from.c_str());
    if (cnv == (iconv_t) - 1)
    {
        iconv_close(cnv);
        return "";
    }
    char *outbuf;
    if ((outbuf = (char *) malloc(text.length()*2 + 1)) == NULL)
    {
        iconv_close(cnv);
        return "";
    }
    char *ip = (char *) text.c_str(), *op = outbuf;
    size_t icount = text.length(), ocount = text.length()*2;

    if (iconv(cnv, &ip, &icount, &op, &ocount) != (size_t) - 1)
    {
        outbuf[text.length()*2 - ocount] = '\0';
        text = outbuf;// после вызова iconv адреса разные. Результат работы iconv находится в outbuf, а op указывает на другой не коректный адрес
    }
    else
    {
        text = "";
    }

    free(outbuf);
    iconv_close(cnv);
    return text;
}

std::string View_LCD::recodeUTF8toCP1251(const std::string& text)
{
    return iconv_recode("UTF-8","CP1251",text);
}

//>>>>> РИСОВАНИЕ СИМВОЛОВ


//Пример кода с http://programmersclub.ru/29/
//Шаблоны функций класса http://msdn.microsoft.com/ru-ru/library/swta9c6e.aspx
//http://valera.asf.ru/cpp/book/c10.html
/*template<class T,class T1> void show_array( T *array,T1 count)
{
   T1 index;
   for (index =0; index < count; index++) cout << array[index] “ ' ';
   cout << endl;
}

void show_array(int *, int);

void show_array(float *, unsigned);

void main(void)

{
   int pages[] = { 100, 200, 300, 400, 500 };
   float pricesH = { 10.05, 20.10, 30.15 };
   show_array(pages, 5);
   show_array(prices, 3);
} */
   
/*template<typename T>
class X
{
public:
   template<typename U>
   void mf(const U &u);
};

template<typename T> template <typename U>
void X<T>::mf(const U &u)
{
}*/

//PART_COUNT, Tahoma_24
//x считается в пикелях
//y в строках (каждая строка 8 бит (пикселей)

//16.1. Определение шаблона класса
//http://valera.asf.ru/cpp/book/c16.shtml
//template <int PART_COUNTX>


/*template <class int1>
class Queue {
public:
   Queue() { }
   ~Queue();
private:
    int1 I1;
};*/

//Чтобы создать класс Queue для объектов типа string, надо написать:
//Queue<string> qs;

//Шаблон должен принимать на вход массив экземпляторов классов и число проходов при рисовании
//template <int PART_COUNTX, class[] Font>//,sUnit24 Tahoma_24
//Вроде должно быть (offset_x_buffer<width) - проверить работу этого условия
    //Использовать как заготовку для шаблона
    //PART_COUNT8 - можно сделать несколько классов, в которых определить как константу
    //Arial_8 - в классе возможно определить переменную и присвоить ей значение глобальной переменной

//Нужен шаблон функции или класса, что же все таки нужно?
//10.1. Определение шаблона функции
//http://cpp.com.ru/lippman/c10.html
//Но при конкретизации - ничего не задается
/*template <class Type, int size>
Type min( const Type (&r_array)[size] )
{
   /* параметризованная функция для отыскания
    * минимального значения в массиве */
/*   Type min_val = r_array[0];
   for ( int i = 1; i < size; ++i )
      if ( r_array[i] < min_val )
         min_val = r_array[i];
   return min_val;
}*/

//Шаблон должен принимать на вход массив экземпляторов классов и число проходов при рисовании
//template <int PART_COUNTX, class[] Font>//,sUnit24 Tahoma_24
//Вроде должно быть (offset_x_buffer<width) - проверить работу этого условия

//Даже лучше не шаблон, а универсальную функцию, вызываемую из нескльких других функций
void View_LCD::print_lcd_XX(const ushort x,const ushort y,const std::string& strIn, ushort width, const unsigned char color,const Font& m_Font)
{
    const std::string str1=recodeUTF8toCP1251(strIn);//CP1251
    //const char* str1=strCP1251.c_str();

    if (width==MAX_WIDTH_LCD) width=columns_in_row-x;//Оставшаяся ширина экран
    if (width>columns_in_row) width=columns_in_row-x;

    ushort offset_x_buffer=x;//133 точки - от 0 до 132 , но полусается начинаем с 1 - т.к. вначале идёт ++             
    for(ushort pos_str=0;pos_str<str1.length();pos_str++)//проходим по всем символам строки
    {
       const unsigned char symbol=str1[pos_str];
       const ushort symbol_width=m_Font.get_width_symbol(symbol);
       if ((offset_x_buffer+symbol_width)>=width) break;//Обрезание производиться по символ - если нет места, то символ не выводиться
       m_Font.DrawTo(offset_x_buffer,y,*this,symbol,color);
       offset_x_buffer+=symbol_width;
    }


/*    const unsigned char value_PART_COUNT=m_Font.value_PART_COUNT;
     for(unsigned char i_lcd = 0; i_lcd < value_PART_COUNT; i_lcd++)//Делаем в несколько проходов
     {
                ushort offset_x_buffer=x;//133 точки - от 0 до 132 , но полусается начинаем с 1 - т.к. вначале идёт ++             
                for(unsigned char pos_str=0;pos_str<strlen(str1);pos_str++)//проходим по всем символам строки
                {
                     unsigned char symbol=str1[pos_str];
                     const sSymbol* const p_Symbol=&m_Font.p_Font[symbol];
                      for(int count = (p_Symbol->width-1)*value_PART_COUNT; count >= 0; count-=value_PART_COUNT)
                      {
                         if (offset_x_buffer<width)//Лишнее обрезаем, т.е. не выводим на дисплей
                         {
                            ushort offset_y_buffer=y+i_lcd;
                            if (offset_y_buffer<rows_in_screen)//Чтобы не было ошибки при выводе ниже нижней границы экрана
                            {
                                (rows_LCD[offset_y_buffer])->write(offset_x_buffer,p_Symbol->b[count+i_lcd]);
                                offset_x_buffer++;
                            }
                        }
                /*while ((++pos_lcd)<=133)
                {    
                           if (!INVERT)
                           { 
                              mI2CBus->write_to_buffer(Black_New_24[symbol].b[count+i_lcd]);
                           }
                           else
                           { 
                              mI2CBus->write_to_buffer(~Black_New_24[symbol].b[count+i_lcd]);
                           }
                         }
                }*/
/*                   }
                }
    }*/
}

void View_LCD::print_lcd_8(const ushort x,const ushort y,const std::string& strIn, ushort width, const unsigned char _color)
{
  print_lcd_XX(x,y,strIn,width,_color,font_Arial_8);
}

void View_LCD::print_lcd_16(const ushort x,const ushort y,const std::string& strIn, ushort width, const unsigned char _color)
{
  print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_16);
}

void View_LCD::print_lcd_24(const ushort x,const ushort y,const std::string& strIn, ushort width, const unsigned char _color)
{
  print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_24);
}

void View_LCD::print_lcd_32(const ushort x,const ushort y,const std::string& strIn, ushort width, const unsigned char _color)
{
  print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_32);
}

void View_LCD::print_lcd(const e_font_height _font_height,const ushort x,const ushort y,const std::string& strIn, ushort width, const unsigned char _color)
{
  switch(_font_height)
  {
    case e_font_height::size_8:
      print_lcd_XX(x,y,strIn,width,_color,font_Arial_8);
    break;
    case e_font_height::size_16:
      print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_16);
    break;
    case e_font_height::size_24:
      print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_24);
    break;
    case e_font_height::size_32:
      print_lcd_XX(x,y,strIn,width,_color,font_Tahoma_32);
    break;
    default:
    break;
  }
}

void View_LCD::Debug_output_console()
{
    for (auto it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
    {
        (*it)->Debug_output_console();
    }

}
//<<<<< РИСОВАНИЕ СИМВОЛОВ

LCD_TIC149::LCD_TIC149(I2CBus* m_I2CBus,const unsigned char contrast) : View_LCD(width_LCD,height_LCD/8)
{
	mI2CBus=m_I2CBus;
	init_lcd(contrast);
}

LCD_TIC149::~LCD_TIC149()
{
}

void LCD_TIC149::init_lcd(const unsigned char contrast)
{
/*  -------------------------------------------------------------------------
 *  инициализация драйвера PCF8535
 */
//Переходим вначале на нужную страницу - пример 0b00001110 // на стр 110
//Потом посылаем нужную команду
//ПРИМЕЧАНИЕ: вначале идут 2 бита, а только потом сами данные
        mI2CBus->write_to_buffer(0b00000000);      // control byte
        mI2CBus->write_to_buffer(0x01);            // на основн стр

        mI2CBus->write_to_buffer(0b00001110);      // на стр 110
        mI2CBus->write_to_buffer(0b00010010);      // BIAS = 1/9
        mI2CBus->write_to_buffer(0b10000100);      // MUX = 1/65
        mI2CBus->write_to_buffer(0b00000100);      // D = 0, E = 0;
        mI2CBus->write_to_buffer(0b00100100);      // IB = 1
//      i2c_write(0b00100000);      // IB = 0
        mI2CBus->write_to_buffer(0b00001110);      // MX = 1, MY = 1;
//      i2c_write(0b00001100);      // MX = 1, MY = 0;

        mI2CBus->write_to_buffer(0x01);            // на основн стр
        mI2CBus->write_to_buffer(0b00010000);      // PD = 0, V = 0; горизонтальная адресация
//      i2c_write(0b00010010);      // PD = 0, V = 0; вертикальная

        mI2CBus->write_to_buffer(0b00001011);      // на стр 011
        mI2CBus->write_to_buffer(0b01011000);      // TRS = 1, BRS = 1;
        mI2CBus->write_to_buffer(0b00000101);      // DM = 1

		//Тест проверки инициализации
		/*mI2CBus->send_buffer();
        usleep(1);
        mI2CBus->write_to_buffer(0b00000000);      // control byte*/
		//Тест проверки инициализации

        mI2CBus->write_to_buffer(0x01);            // на основн стр
        mI2CBus->write_to_buffer(0b00001101);      // на стр 101

        mI2CBus->write_to_buffer(0b00001000);      // S[1;0] = 00, mul factor = 2
//      mI2CBus->write_to_buffer(0b00000110);      // PRS = 1, Vlcd programing range high
        mI2CBus->write_to_buffer(0b00000100);      // PRS = 0, Vlcd programing range low

        mI2CBus->write_to_buffer(contrast | 0x80);       // установка напряжения
        mI2CBus->write_to_buffer(0b00000101);      // включение генератора

		//mI2CBus->send_buffer();
        //delay_ms(1);                // should be более 20 мкс
        mI2CBus->write_to_buffer(0b00001001);      // S[1;0] = 01, mul factor = 3
		//mI2CBus->send_buffer();
        //delay_ms(1);
        mI2CBus->write_to_buffer(0b00001010);      // S[1;0] = 10, mul factor = 4
		//mI2CBus->send_buffer();
        //delay_ms(1);
        mI2CBus->write_to_buffer(0b00001011);      // S[1;0] = 11, mul factor = 5

        mI2CBus->write_to_buffer(0x01);            // на основн стр
        mI2CBus->write_to_buffer(0b00001011);      // на стр 011

        mI2CBus->write_to_buffer(0b00000100);      // DM = 0

        mI2CBus->write_to_buffer(0x01);            // на основн стр
        mI2CBus->write_to_buffer(0b00001110);      // на стр 110
        mI2CBus->write_to_buffer(0b00000110);      // D = 1, E = 0; normal*/

        //Измерение тмпературы
        //i2c_write(0x01);            // на основн стр
        //i2c_write(0b00001101);      // на стр 101
        //i2c_write(0b00100001);      // запуск измерения температуры

	mI2CBus->send_buffer();
}

void LCD_TIC149::lcd_view_invert()
{//Нормальный вид - символы чёрным (без инверсии)
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00000111);      // D = 1, E = 1; 
	
   mI2CBus->send_buffer();
} 

void LCD_TIC149::lcd_view_normal()
{//Инвертированный вид - символы белым (фон чёрный)
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00000110);      // D = 1, E = 0; normal
	
	mI2CBus->send_buffer();
}

void LCD_TIC149::lcd_rotate_0()
{
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00001110);      // MX = 1, MY = 1;
	mI2CBus->send_buffer();
}
void LCD_TIC149::lcd_rotate_180()
{
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00001000);      // MX = 0, MY = 0;
	mI2CBus->send_buffer();
}

void LCD_TIC149::set_lcd_contrast(const unsigned char lcd_k)
{
         mI2CBus->write_to_buffer(0b00000000);      // control byte

         mI2CBus->write_to_buffer(0x01);            // на основн стр
         mI2CBus->write_to_buffer(0b00001101);      // на стр 101

         mI2CBus->write_to_buffer(0b00001000);      // S[1;0] = 00, mul factor = 2
//       i2c_write(0b00000110);      // PRS = 1, Vlcd programing range high
         mI2CBus->write_to_buffer(0b00000100);      // PRS = 0, Vlcd programing range low

         mI2CBus->write_to_buffer(lcd_k | 0x80);       // установка напряжения
         mI2CBus->write_to_buffer(0b00000101);      // включение генератора
		 mI2CBus->send_buffer();
}

void LCD_TIC149::clear_lcd_hardware()
{
//очистка ОЗУ драйвера
        mI2CBus->write_to_buffer(0b00000000);         // control byte

        mI2CBus->write_to_buffer(0x01);               // на основн стр
        mI2CBus->write_to_buffer(0b01000000);         // Y = 0;
        mI2CBus->write_to_buffer(0b10000000);         // X = 0;

		 mI2CBus->send_buffer();
		 
        mI2CBus->write_to_buffer(0b01000000);         // control byte

        for(int font_qwe = 0; font_qwe < 1064; font_qwe++)
        {
            mI2CBus->write_to_buffer(0x00);
        }
		 mI2CBus->send_buffer();
}

void LCD_TIC149::lcd_screen_buffer()
{
//картинка на индикатор
        mI2CBus->write_to_buffer(0b00000000);         // control byte
        mI2CBus->write_to_buffer(0x01);               // на основн стр
        mI2CBus->write_to_buffer(0b01000000);         // Y = 0;
        mI2CBus->write_to_buffer(0b10000000);         // X = 0;

        mI2CBus->send_buffer();
     
        //i2c_start();
        //i2c_restart();
    
        mI2CBus->write_to_buffer(0b01000000);         // control byte

        const unsigned char* pic = screen_buffer;   
        for(int font_qwe = 0; font_qwe < 1064; font_qwe++)
        {
            mI2CBus->write_to_buffer(*pic++);
        }
        mI2CBus->send_buffer();
}

void LCD_TIC149::lcd_screen_buffer_old()
{
//картинка на индикатор
        mI2CBus->write_to_buffer(0b00000000);         // control byte
        mI2CBus->write_to_buffer(0x01);               // на основн стр
        mI2CBus->write_to_buffer(0b01000000);         // Y = 0;
        mI2CBus->write_to_buffer(0b10000000);         // X = 0;

	      mI2CBus->send_buffer();
		 
        //i2c_start();
        //i2c_restart();
    
        mI2CBus->write_to_buffer(0b01000000);         // control byte

        const unsigned char* pic = screen_buffer;   
        for(int font_qwe = 0; font_qwe < 1064; font_qwe++)
        {
            mI2CBus->write_to_buffer(*pic++);
        }
    	  mI2CBus->send_buffer();
}

void LCD_TIC149::lcd_screen_old()
{
//картинка на индикатор
        mI2CBus->write_to_buffer(0b00000000);         // control byte
        mI2CBus->write_to_buffer(0x01);               // на основн стр
        mI2CBus->write_to_buffer(0b01000000);         // Y = 0;
        mI2CBus->write_to_buffer(0b10000000);         // X = 0;

	mI2CBus->send_buffer();
		 
        //i2c_start();
        //i2c_restart();
    
        mI2CBus->write_to_buffer(0b01000000);         // control byte

        const unsigned char* pic = screen_logo;   
        for(int font_qwe = 0; font_qwe < 1064; font_qwe++)
        {
            mI2CBus->write_to_buffer(*pic++);
        }
		 mI2CBus->send_buffer();
}

const unsigned char LCD_TIC149::screen_logo[1064] = { 
    0 ,  7 ,  8 , 16 , 16 , 16 , 16 , 16 ,  0 ,  0
 ,  0 ,  4 ,  4 ,  4 ,  4 ,  3 ,  0 ,  7 ,  2 ,  4
 ,  7 ,  2 ,  4 ,  7 ,  0 ,  7 ,  2 ,  4 ,  7 ,  2
 ,  4 ,  7 ,  0 ,  0 ,  0 ,  4 ,  4 ,  4 ,  4 ,  3
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 , 14 , 18 , 17 , 17 , 16 , 16 ,  0 ,  0 ,  7 ,  2
 ,  4 ,  4 ,  4 ,  3 ,  0 ,  0 ,127 ,  2 ,  4 ,  4
 ,  4 ,  3 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 , 16 , 16 , 16 , 63 ,  0 ,  0 ,  0 ,  0 ,  0 , 32
 , 34 , 34 , 34 , 29 ,  0 ,  0 ,  0 , 32 , 34 , 34
 , 34 , 29 ,  0 ,  0 ,  4 ,  2 ,  1 ,  1 ,  2 ,  4
 ,  0 ,  0 , 15 , 17 , 34 , 34 , 34 ,  1 ,  0 ,  0
 ,  1 ,  2 , 12 , 16 , 63 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,192 , 32 , 16 , 16 ,144 ,144
 ,240 ,  0 ,  0 , 96 ,144 ,144 ,144 ,144 ,240 , 16
 ,240 ,  0 ,  0 ,240 ,  0 ,  0 ,240 ,  0 ,240 ,  0
 ,  0 ,240 ,  0 ,  0 ,240 ,  0 ,  0 , 96 ,144 ,144
 ,144 ,144 ,240 , 16 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 , 16 , 16 , 16 , 16 ,144 ,224 ,  0
 ,  0 ,254 , 32 , 16 , 16 , 16 ,224 ,  0 ,  0 ,240
 , 32 , 16 , 16 , 16 ,224 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  3 , 18 , 18 , 18 ,242 , 18 , 18 , 18
 ,  2 ,  2 , 18 , 18 , 18 , 18 ,226 ,  2 ,  2 ,  2
 , 18 , 18 , 18 , 18 ,226 ,  2 ,  2 , 18 , 34 ,194
 ,194 , 34 , 18 ,  2 ,  2 ,194 , 34 , 18 , 18 , 18
 ,226 ,  2 ,  2 ,130 ,130 ,130 ,130 ,242 ,130 ,  2
 ,  3 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  3 ,  7 , 15 , 15 , 31 , 63
 , 63 , 63 , 63 ,127 ,127 ,127 ,127 ,127 , 63 , 63
 , 63 , 63 , 31 , 15 , 15 ,  7 ,  3 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 ,  1
 ,  3 ,  7 ,  7 , 15 , 31 , 31 , 63 , 63 , 31 , 31
 , 15 ,  7 ,  7 ,  3 ,  1 ,  1 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  7 ,  4 ,  4
 ,  4 ,  4 ,  4 ,  4 ,  4 ,255 ,  4 ,  4 ,  4 ,  4
 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4
 ,  4 ,  4 ,  4 ,  4 ,  4 ,127 , 68 , 68 , 68 , 68
 , 68 , 68 ,127 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4
 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4
 ,  4 ,  4 ,  4 ,255 ,  4 ,  4 ,  4 ,  4 ,  7 ,  0
 ,  0 ,  0 ,  0 ,  0 , 15 ,127 ,255 ,255 ,255 ,255
 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255
 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255
 ,255 ,127 , 15 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,128 ,128 ,128 ,128 ,128 ,128 ,128 ,255 ,255 ,255
 ,255 ,255 ,255 ,255 ,128 ,128 ,128 ,128 ,128 ,128
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,254 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,255 ,  2
 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2
 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,255 ,  2
 ,  2 ,  2 ,  2 ,  2 ,  2 ,255 ,  2 ,  2 ,  2 ,  2
 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2
 ,  2 ,  2 ,  2 ,  2 ,  2 ,  2 ,255 ,  2 ,  2 ,  2
 ,  2 ,254 ,  0 ,  0 ,  0 ,  0 ,  0 ,128 ,240 ,248
 ,254 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255
 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255 ,255
 ,255 ,255 ,254 ,248 ,240 ,128 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  8 , 12 , 12 , 14 , 15 , 15 , 15
 ,255 ,255 ,255 ,255 ,255 ,255 ,255 , 15 , 15 , 14
 , 12 , 12 ,  8 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,255 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,255 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,255 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,255
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,128 ,128 ,192 ,224 ,224
 ,224 ,224 ,240 ,240 ,240 ,240 ,240 ,224 ,224 ,224
 ,224 ,192 ,128 ,128 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,128 ,192 ,192 ,224 ,224 ,192 ,192 ,128
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,252 ,  4 ,  4 ,  4 ,  4 ,  4
 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4
 ,  4 ,  4 ,  4 ,  4 ,228 , 36 , 36 , 36 , 36 , 36
 , 36 ,228 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4
 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4 ,  4
 ,  4 ,  4 ,252 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 , 15 ,  8 ,  8 , 15 ,  0 ,  0 , 15 ,  4
 ,  8 ,  8 ,  8 ,  7 ,  0 ,  0 ,  0 ,  9 ,  9 ,  9
 ,  9 ,  7 ,  0 ,  0 , 15 ,128 , 65 , 66 , 68 ,143
 ,  0 ,  0 , 15 ,  9 ,  9 ,  9 ,  9 ,  6 ,  0 ,  0
 ,  3 , 13 ,  9 ,  9 ,  9 ,  7 ,  0 ,  0 , 15 ,  4
 ,  8 ,  8 ,  8 ,  7 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 , 63 , 33 , 33 , 33 , 34 , 30 ,  0 , 15
 , 16 , 32 , 32 , 32 , 32 , 32 ,  0 ,  0 , 63 , 34
 , 34 , 34 , 34 , 32 ,  0 ,  0 , 57 , 70 , 68 , 68
 , 74 , 49 ,  0 ,  0 ,  0 ,124 , 68 , 68 , 66 , 67
 ,  0 ,  0 ,  0 , 64 , 68 , 68 , 68 , 59 ,  0 ,  0
 ,  0 ,124 , 68 , 68 , 66 , 67 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 , 56 , 32 ,224 , 32 , 32 , 32 ,224 , 56
 ,  0 ,252 , 64 , 32 , 32 , 32 ,192 ,  0 ,  0 ,192
 , 32 , 32 , 32 , 32 ,224 , 32 ,  0 ,224 , 64 ,128
 ,  0 ,  0 ,224 ,  0 ,  0 ,224 , 32 , 32 , 32 , 32
 ,192 ,  0 ,  0 ,192 , 32 , 32 , 32 , 32 , 32 ,  0
 ,  0 ,252 , 64 , 32 , 32 , 32 ,192 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,224 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,128 , 64 , 32 , 32 , 32 , 32 , 32 ,  0
 ,  0 ,224 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,192
 , 32 , 32 , 32 , 32 ,192 ,  0 ,  0 ,  0 , 32 , 32
 , 32 , 32 ,192 ,  0 ,  0 ,  0 , 32 , 32 , 32 , 32
 ,192 ,  0 ,  0 ,  0 , 32 , 32 , 32 , 32 ,192 ,  0
 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  0
 ,  0 ,  0 ,  0 ,  0
 };

void LCD_TIC149::print_lcd_8_old(char x,char y,const std::string& strIn, ushort width, unsigned char INVERT)
{
    std::string strCP1251=recodeUTF8toCP1251(strIn);
    const char* str1=strCP1251.c_str();
    if (width>width_LCD) width=width_LCD;
     for(unsigned char i_lcd = 0; i_lcd < PART_COUNT8; i_lcd++)//Делаем в 3 прохода
     {
				unsigned int offset_y_buffer=(i_lcd+y)*width_LCD;
				unsigned char offset_x_buffer=x;//133 точки - от 0 до 132 , но полусается начинаем с 1 - т.к. вначале идёт ++             
				
                for(unsigned char pos_str=0;pos_str<strlen(str1);pos_str++)//проходим по всем символам строки
                {
                     unsigned char symbol=str1[pos_str];
                     //if (symbol<32) symbol=0; else symbol-=32;
                      for(int count = (Arial_8[symbol].sH.cyPix-1)*PART_COUNT8; count >= 0; count-=PART_COUNT8)
                      {
                         if (offset_x_buffer<width)//Лишнее обрезаем, т.е. не выводим на дисплей
                         {
							if ((offset_y_buffer+offset_x_buffer)<size_screen_buffer)//Чтобы не было ошибки при выводе ниже нижней границы экрана
							{
								screen_buffer[offset_y_buffer+offset_x_buffer]=Arial_8[symbol].b[count+i_lcd];
								offset_x_buffer++;
							}
						}
                   }
                }
	}
}
void LCD_TIC149::print_lcd_16_old(char x,char y,const std::string& strIn, ushort width, unsigned char INVERT)
{
    std::string strCP1251=recodeUTF8toCP1251(strIn);
    const char* str1=strCP1251.c_str();
    if (width>width_LCD) width=width_LCD;
     for(unsigned char i_lcd = 0; i_lcd < PART_COUNT16; i_lcd++)//Делаем в 3 прохода
     {
				unsigned int offset_y_buffer=(i_lcd+y)*width_LCD;
				unsigned char offset_x_buffer=x;//133 точки - от 0 до 132 , но полусается начинаем с 1 - т.к. вначале идёт ++             
				
                for(unsigned char pos_str=0;pos_str<strlen(str1);pos_str++)//проходим по всем символам строки
                {
                     unsigned char symbol=str1[pos_str];
                     //if (symbol<32) symbol=0; else symbol-=32;
                      for(int count = (Tahoma_16[symbol].sH.cyPix-1)*PART_COUNT16; count >= 0; count-=PART_COUNT16)
                      {
                         if (offset_x_buffer<width)//Лишнее обрезаем, т.е. не выводим на дисплей
                         {
							if ((offset_y_buffer+offset_x_buffer)<size_screen_buffer)//Чтобы не было ошибки при выводе ниже нижней границы экрана
							{
								screen_buffer[offset_y_buffer+offset_x_buffer]=Tahoma_16[symbol].b[count+i_lcd];
								offset_x_buffer++;
							}
						}
                   }
                }
	}
}
void LCD_TIC149::print_lcd_24_old(char x,char y,const std::string& strIn, ushort width, unsigned char INVERT)
{
    std::string strCP1251=recodeUTF8toCP1251(strIn);
    const char* str1=strCP1251.c_str();
    if (width>width_LCD) width=width_LCD;
     for(unsigned char i_lcd = 0; i_lcd < PART_COUNT24; i_lcd++)//Делаем в 3 прохода
     {
				unsigned int offset_y_buffer=(i_lcd+y)*width_LCD;
				unsigned char offset_x_buffer=x;//133 точки - от 0 до 132 , но полусается начинаем с 1 - т.к. вначале идёт ++             
				
                for(unsigned char pos_str=0;pos_str<strlen(str1);pos_str++)//проходим по всем символам строки
                {
                     unsigned char symbol=str1[pos_str];
                     //if (symbol<32) symbol=0; else symbol-=32;
                      for(int count = (Tahoma_24[symbol].sH.cyPix-1)*PART_COUNT24; count >= 0; count-=PART_COUNT24)
                      {
                         if (offset_x_buffer<width)//Лишнее обрезаем, т.е. не выводим на дисплей
                         {
							if ((offset_y_buffer+offset_x_buffer)<size_screen_buffer)//Чтобы не было ошибки при выводе ниже нижней границы экрана
							{
								screen_buffer[offset_y_buffer+offset_x_buffer]=Tahoma_24[symbol].b[count+i_lcd];
								offset_x_buffer++;
							}
						}
                   }
                }
	}
}
void LCD_TIC149::print_lcd_32_old(char x,char y,const std::string& strIn, ushort width, unsigned char INVERT)
{
    std::string strCP1251=recodeUTF8toCP1251(strIn);
    const char* str1=strCP1251.c_str();
    if (width>width_LCD) width=width_LCD;
     for(unsigned char i_lcd = 0; i_lcd < PART_COUNT32; i_lcd++)//Делаем в 3 прохода
     {
				unsigned int offset_y_buffer=(i_lcd+y)*width_LCD;
				unsigned char offset_x_buffer=x;//133 точки - от 0 до 132 , но полусается начинаем с 1 - т.к. вначале идёт ++             
				
                for(unsigned char pos_str=0;pos_str<strlen(str1);pos_str++)//проходим по всем символам строки
                {
                     unsigned char symbol=str1[pos_str];
                     //if (symbol<32) symbol=0; else symbol-=32;
                      for(int count = (Tahoma_32[symbol].sH.cyPix-1)*PART_COUNT32; count >= 0; count-=PART_COUNT32)
                      {
                         if (offset_x_buffer<width)//Лишнее обрезаем, т.е. не выводим на дисплей
                         {
							if ((offset_y_buffer+offset_x_buffer)<size_screen_buffer)//Чтобы не было ошибки при выводе ниже нижней границы экрана
							{
								screen_buffer[offset_y_buffer+offset_x_buffer]=Tahoma_32[symbol].b[count+i_lcd];
								offset_x_buffer++;
							}
						}
                   }
                }
	}
}

