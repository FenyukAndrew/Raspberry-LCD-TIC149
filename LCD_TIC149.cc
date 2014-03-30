#include "I2CBus.h"

#include "LCD_TIC149.h"
#include "Row_LCD.h"

#include "EasyBMP/EasyBMP.h"

LCD_TIC149::LCD_TIC149(I2CBus* m_I2CBus,const unsigned char contrast) : View_LCD(width_LCD,height_LCD)
{
    mI2CBus=m_I2CBus;
    init_lcd(contrast);
}

LCD_TIC149::~LCD_TIC149()
{
}

void LCD_TIC149::init_lcd(const unsigned char contrast) const
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

void LCD_TIC149::lcd_view_invert() const
{
    //Нормальный вид - символы чёрным (без инверсии)
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00000111);      // D = 1, E = 1;

    mI2CBus->send_buffer();
}

void LCD_TIC149::lcd_view_normal() const
{
    //Инвертированный вид - символы белым (фон чёрный)
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00000110);      // D = 1, E = 0; normal

    mI2CBus->send_buffer();
}

void LCD_TIC149::lcd_rotate_0() const
{
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00001110);      // MX = 1, MY = 1;
    mI2CBus->send_buffer();
}
void LCD_TIC149::lcd_rotate_180() const
{
    mI2CBus->write_to_buffer(0b00000000);
    mI2CBus->write_to_buffer(0x01);            // на основн стр
    mI2CBus->write_to_buffer(0b00001110);      // на стр 110
    mI2CBus->write_to_buffer(0b00001000);      // MX = 0, MY = 0;
    mI2CBus->send_buffer();
}

void LCD_TIC149::set_lcd_contrast(const unsigned char lcd_k) const
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

void LCD_TIC149::clear_lcd_hardware() const
{
//очистка ОЗУ драйвера
    mI2CBus->write_to_buffer(0b00000000);         // control byte

    mI2CBus->write_to_buffer(0x01);               // на основн стр
    mI2CBus->write_to_buffer(0b01000000);         // Y = 0;
    mI2CBus->write_to_buffer(0b10000000);         // X = 0;

    mI2CBus->send_buffer();

    mI2CBus->write_to_buffer(0b01000000);         // control byte

    for(int font_qwe = 0; font_qwe < width_LCD*height_LCD/8; font_qwe++)
    {
        mI2CBus->write_to_buffer(0x00);
    }
    mI2CBus->send_buffer();
}

void LCD_TIC149::push_buffer_to_lcd_screen() const
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

    unsigned char* buffer=mI2CBus->get_currect_ptr_buffer();
    unsigned short reserve_count=width_LCD*height_LCD/8;
    mI2CBus->reserve(reserve_count);

//Производиться упаковка по 8 строк в одну строку массива - т.к. экран двухцветный
//Кажеться нужно переворачивать
    unsigned char count_rows=0;
    for (std::vector<Row_LCD*>::const_iterator it = rows_LCD.begin() ; it != rows_LCD.end(); ++it)
    {
        (*it)->encoder_row(buffer,count_rows);
        if ((count_rows++)>7)
        {
            count_rows=0;
            buffer+=width_LCD;
        }
    }

    //const unsigned char* pic = screen_buffer;
    /*for(int font_qwe = 0; font_qwe < 1064; font_qwe++)
    {
        //Требуется преобразование к виду 1 бит на 1 пиксель - 8 строк нужно объединить в один массив
        //mI2CBus->write_to_buffer(*pic++);
    }*/
    mI2CBus->send_buffer();
}
