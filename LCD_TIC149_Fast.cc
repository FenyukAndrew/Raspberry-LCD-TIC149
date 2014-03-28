#include <string.h>

#include "I2CBus.h"
#include "LCD_TIC149_Fast.h"

#include "Utils.h"

#include "description_font.h"
extern sUnit8 Arial_8[256];
extern sUnit16 Tahoma_16[256];
extern sUnit24 Tahoma_24[256];
extern sUnit32 Tahoma_32[256];

LCD_TIC149_Fast::LCD_TIC149_Fast(I2CBus* m_I2CBus,const unsigned char contrast)
{
	mI2CBus=m_I2CBus;
	init_lcd(contrast);
	clear_buffer();
}

LCD_TIC149_Fast::~LCD_TIC149_Fast()
{
}

void LCD_TIC149_Fast::init_lcd(const unsigned char contrast) const
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

void LCD_TIC149_Fast::lcd_view_invert() const
{//Нормальный вид - символы чёрным (без инверсии)
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00000111);      // D = 1, E = 1; 
	
   mI2CBus->send_buffer();
} 

void LCD_TIC149_Fast::lcd_view_normal() const
{//Инвертированный вид - символы белым (фон чёрный)
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00000110);      // D = 1, E = 0; normal
	
	mI2CBus->send_buffer();
}

void LCD_TIC149_Fast::lcd_rotate_0() const
{
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00001110);      // MX = 1, MY = 1;
	mI2CBus->send_buffer();
}
void LCD_TIC149_Fast::lcd_rotate_180() const
{
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00001000);      // MX = 0, MY = 0;
	mI2CBus->send_buffer();
}

void LCD_TIC149_Fast::set_lcd_contrast(const unsigned char lcd_k) const
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

void LCD_TIC149_Fast::clear_buffer()
{
        unsigned char* pic = screen_buffer;   
        for(int font_qwe = 0; font_qwe < size_screen_buffer; font_qwe++)
        {
            *pic++=0;
        }
}


void LCD_TIC149_Fast::clear_lcd_hardware() const
{
//очистка ОЗУ драйвера
        mI2CBus->write_to_buffer(0b00000000);         // control byte

        mI2CBus->write_to_buffer(0x01);               // на основн стр
        mI2CBus->write_to_buffer(0b01000000);         // Y = 0;
        mI2CBus->write_to_buffer(0b10000000);         // X = 0;

		 mI2CBus->send_buffer();
		 
        mI2CBus->write_to_buffer(0b01000000);         // control byte

        for(int font_qwe = 0; font_qwe < size_screen_buffer; font_qwe++)
        {
            mI2CBus->write_to_buffer(0x00);
        }
		 mI2CBus->send_buffer();
}

void LCD_TIC149_Fast::push_buffer_to_lcd_screen() const
{
//буфер на индикатор
        mI2CBus->write_to_buffer(0b00000000);         // control byte
        mI2CBus->write_to_buffer(0x01);               // на основн стр
        mI2CBus->write_to_buffer(0b01000000);         // Y = 0;
        mI2CBus->write_to_buffer(0b10000000);         // X = 0;

	      mI2CBus->send_buffer();
		 
        //i2c_start();
        //i2c_restart();
    
        mI2CBus->write_to_buffer(0b01000000);         // control byte

        const unsigned char* pic = screen_buffer;   
        for(int font_qwe = 0; font_qwe < size_screen_buffer; font_qwe++)
        {
            mI2CBus->write_to_buffer(*pic++);
        }
    	  mI2CBus->send_buffer();
}

void LCD_TIC149_Fast::lcd_output_hardware_logo() const
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
        for(int font_qwe = 0; font_qwe < size_screen_buffer; font_qwe++)
        {
            mI2CBus->write_to_buffer(*pic++);
        }
		 mI2CBus->send_buffer();
}

const unsigned char LCD_TIC149_Fast::screen_logo[size_screen_buffer] = { 
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

void LCD_TIC149_Fast::print_lcd_8(char x,char y,const std::string& strIn, unsigned short width, unsigned char INVERT)
{
    std::string strCP1251=Utils::recodeUTF8toCP1251(strIn);
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
void LCD_TIC149_Fast::print_lcd_16(char x,char y,const std::string& strIn, unsigned short width, unsigned char INVERT)
{
    std::string strCP1251=Utils::recodeUTF8toCP1251(strIn);
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
void LCD_TIC149_Fast::print_lcd_24(char x,char y,const std::string& strIn, unsigned short width, unsigned char INVERT)
{
    std::string strCP1251=Utils::recodeUTF8toCP1251(strIn);
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
void LCD_TIC149_Fast::print_lcd_32(char x,char y,const std::string& strIn, unsigned short width, unsigned char INVERT)
{
    std::string strCP1251=Utils::recodeUTF8toCP1251(strIn);
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
