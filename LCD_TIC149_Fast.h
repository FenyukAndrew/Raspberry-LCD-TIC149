#ifndef LCD_TIC149_FAST
#define LCD_TIC149_FAST 1
/*
Класс LCD_TIC149_Fast используется для вывода на TIC_149
Минимум функционала, зато быстро работает
Предназначен для использования на низкоскоростных процессорах
Использует 1Kb буфер, для хранения 1 пикселя используется 1 бит

Для сокращения операций копирования можно непосредственно выводить в буфер I2C
реализовать быстрый вывод без использования двойного буфера - взять указатель сразу на байт, следующий после команды управления на экран
*/

#include <string>
#include "common_header.h"

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
    /*void write_lcd_screen_buffer(const unsigned short pos, const unsigned char value)
    {
        screen_buffer[pos]=value;
    };*/

//x считается в пикелях
//y в строках (каждая строка 8 бит (пикселей)
    void print_lcd(Fnt e_fnt,char x,char y,const std::string& str1, unsigned short width=width_LCD, unsigned char INVERT=0);
//Вывод на экран строки определённой ширины, если больше - то не выводятся

    void point(const short x,const short y,const unsigned char _color=DEFAULT_COLOR);

    void line(short x1, short y1, const short x2, const short y2,const unsigned char color_code=DEFAULT_COLOR);
    void circle(const short x_center, const short y_center, const short radius, const unsigned char color_code=DEFAULT_COLOR);
    void rectangle(short x1, short y1, short x2, short y2, const unsigned char color_code=DEFAULT_COLOR);

    void horizontal_line(short x1, const short x2, const unsigned char color_code=DEFAULT_COLOR);
    void vertical_line(short y1, const short y2,const unsigned char color_code=DEFAULT_COLOR);

private:
    I2CBus* mI2CBus;//По идее лучше использовать ссылку - как правильно её инициализировать?

    constexpr static unsigned short size_screen_buffer=1064;
    constexpr static unsigned short size_full_screen_buffer=1064+1;//Храниться 1 байт управляющей команды
    const static unsigned char screen_logo[size_full_screen_buffer];
    unsigned char full_screen_buffer[size_full_screen_buffer];
    unsigned char* screen_buffer;//Указатель на экранные данные

    void plot_circle(const int x, const int y, const short x_center, const short y_center, const unsigned char color_code);
};
#endif
