//Чтобы русские символы нормально отображались нужно, чтобы была кодировка файл с текстом Windows-1251 - т.е. main.cc
//Заполнение экрана данными происходит построчно (1 строка в высоту 8 пикселей) слева направо сверху вниз

//Для вывода на экран используется библиотека символов, закодированных с помощью Windows-1251

//Наборы команд можно объединить в массивы и используя sizeof сразу передавать в I2C

#include "View_LCD.h"

class I2CBus;

class LCD_TIC149 : public View_LCD
{
public:
    LCD_TIC149(I2CBus* m_I2CBus,const unsigned char contrast);
    ~LCD_TIC149();

    constexpr static unsigned short width_LCD=133;
    constexpr static unsigned short height_LCD=64;

    void init_lcd(unsigned char contrast) const;

    //>>>>>Функции экрана, работающие непосредственно с оборудованием
    void lcd_view_invert() const;
    void lcd_view_normal() const;
    void lcd_rotate_0() const;
    void lcd_rotate_180() const;
    void set_lcd_contrast(const unsigned char lcd_k) const;
    void clear_lcd_hardware() const;//Очищает только экран, а не массив
    //<<<<<Функции экрана, работающие непосредственно с оборудованием

    void push_buffer_to_lcd_screen() const;

private:
    I2CBus* mI2CBus;//По идее лучше использовать ссылку - как правильно её инициализировать?
};
