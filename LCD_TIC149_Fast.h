/*
Класс LCD_TIC149_Fast используется для вывода на TIC_149
Минимум функционала, зато быстро работает
Предназначен для использования на низкоскоростных процессорах
Использует 1Kb буфер, для хранения 1 пикселя используется 1 бит

Для сокращения операций копирования можно непосредственно выводить в буфер I2C
реализовать быстрый вывод без использования двойного буфера - взять указатель сразу на байт, следующий после команды управления на экран
*/

#include <string>

class I2CBus;

class LCD_TIC149_Fast
{
public:
	LCD_TIC149_Fast(I2CBus* m_I2CBus,const unsigned char contrast);
	~LCD_TIC149_Fast();

	constexpr static unsigned short width_LCD=133;
	constexpr static unsigned short height_LCD=64;
	
	//>>>>>Функции экрана, работающие непосредственно с оборудованием
	void init_lcd(unsigned char contrast) const;
	void lcd_view_invert() const;	
	void lcd_view_normal() const;
	void lcd_rotate_0() const;
	void lcd_rotate_180() const;
	void set_lcd_contrast(const unsigned char lcd_k) const;
	void clear_lcd_hardware() const;//Очищает только экран, а не экранный буфер
	void lcd_output_hardware_logo() const;
	//<<<<<Функции экрана, работающие непосредственно с оборудованием
	
	void clear_buffer();//Очищает только экранный буфер
	void push_buffer_to_lcd_screen() const;
	void write_lcd_screen_buffer(const unsigned short pos, const unsigned char value) {screen_buffer[pos]=value;};
	
//x считается в пикелях
//y в строках (каждая строка 8 бит (пикселей)
	void print_lcd_8(char x,char y,const std::string& str1, unsigned short width=width_LCD, unsigned char INVERT=0);
	void print_lcd_16(char x,char y,const std::string& str1, unsigned short width=width_LCD, unsigned char INVERT=0);
	void print_lcd_24(char x,char y,const std::string& str1, unsigned short width=width_LCD, unsigned char INVERT=0);
	void print_lcd_32(char x,char y,const std::string& str1, unsigned short width=width_LCD, unsigned char INVERT=0);
//Вывод на экран строки определённой ширины, если больше - то не выводятся

private:
	I2CBus* mI2CBus;//По идее лучше использовать ссылку - как правильно её инициализировать?

	constexpr static unsigned short size_screen_buffer=1064;
	const static unsigned char screen_logo[size_screen_buffer];
	unsigned char screen_buffer[size_screen_buffer];

//Получаются переменные класса или можно сделать глобальными переменными
//Проблема копируются вместе с каждым экземпляром класса
/*#include "Arial_8.h"
#include "Tahoma_16.h"
#include "Tahoma_24.h"
#include "Tahoma_32.h"*/
};