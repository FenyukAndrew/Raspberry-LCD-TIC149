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

#include "I2CBus.h"
#include "LCD_TIC149.h"

LCD_TIC149::LCD_TIC149(I2CBus* m_I2CBus,unsigned char contrast)
{
	mI2CBus=m_I2CBus;
	init_lcd(contrast);
}

LCD_TIC149::~LCD_TIC149()
{
}

void LCD_TIC149::init_lcd(unsigned char contrast)
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

void LCD_TIC149::set_lcd_contrast(unsigned char lcd_k)
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

void LCD_TIC149::clear_lcd()
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


void LCD_TIC149::lcd_screen()
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

//Black_New[Номер символа]
//Black_New[pos_str].sH.cyPix;//Необходимое количество выводимых байт (т.е. 19-x)
//Black_New[pos_str].b[];//Верхняя строка все нечётные - нижняя все чётные 
   
//Общий для всех шрифтов, сгенерированных LCDicon
struct sHeader
   {         
		//flash char ccChar;              // optional char 
		int cxPix;
		int cyPix;
   };

template<int T>
struct sUnitN {
		sHeader sH;               // header 
		unsigned char b[T];          // Data 
};
const int PART_COUNT8=1;
const int PART_COUNT16=2;
const int PART_COUNT24=3;
const int PART_COUNT32=4;
//Маскимальная длина символа высотой 24 - определять по программе - скорее всего буква Щ
const int SYM_8_LONG=20;
const int SYM_16_LONG=30;
const int SYM_24_LONG=40;//28
const int SYM_32_LONG=50;
typedef sUnitN<SYM_8_LONG*PART_COUNT8> sUnit8;
typedef sUnitN<SYM_16_LONG*PART_COUNT16> sUnit16;
typedef sUnitN<SYM_24_LONG*PART_COUNT24> sUnit24;
typedef sUnitN<SYM_32_LONG*PART_COUNT32> sUnit32;

#include "Arial_8.h"
#include "Tahoma_16.h"
#include "Tahoma_24.h"
#include "Tahoma_32.h"

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

//Шаблон должен принимать на вход массив экземпляторов классов и число проходов при рисовании
//template <int PART_COUNTX, class[] Font>//,sUnit24 Tahoma_24
//Вроде должно быть (offset_x_buffer<width) - проверить работу этого условия
void LCD_TIC149::print_lcd_8(char x,char y,char* str1, int width, unsigned char INVERT)
{
	if (width>=width_LCD) width=width_LCD;
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
                   }
                }
	}
}
void LCD_TIC149::print_lcd_16(char x,char y,char* str1, int width, unsigned char INVERT)
{
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
                          /* if (!INVERT)
                           { 
                              mI2CBus->write_to_buffer(Black_New_24[symbol].b[count+i_lcd]);
                           }
                           else
                           { 
                              mI2CBus->write_to_buffer(~Black_New_24[symbol].b[count+i_lcd]);
                           }
                         }*/
                   }
                }
	}
}
void LCD_TIC149::print_lcd_24(char x,char y,char* str1, int width, unsigned char INVERT)
{
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
                          /* if (!INVERT)
                           { 
                              mI2CBus->write_to_buffer(Black_New_24[symbol].b[count+i_lcd]);
                           }
                           else
                           { 
                              mI2CBus->write_to_buffer(~Black_New_24[symbol].b[count+i_lcd]);
                           }
                         }*/
                   }
                }
	}
}
void LCD_TIC149::print_lcd_32(char x,char y,char* str1, int width, unsigned char INVERT)
{
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
                          /* if (!INVERT)
                           { 
                              mI2CBus->write_to_buffer(Black_New_24[symbol].b[count+i_lcd]);
                           }
                           else
                           { 
                              mI2CBus->write_to_buffer(~Black_New_24[symbol].b[count+i_lcd]);
                           }
                         }*/
                   }
                }
	}
}

