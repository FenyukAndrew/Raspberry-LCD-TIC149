#include "Row_LCD.h"
#include "EasyBMP/EasyBMP.h"
#include <string>

//Упаковка строки - смещение на номер строки
void Row_LCD::encoder_row(unsigned char* buffer,const unsigned char count_rows) const
{
    for(unsigned short i=0; i<columns_in_row; i++)
    {
        unsigned char b=((data[i]!=0) ? 1 : 0)<<count_rows;
        if (count_rows==0)
        {//Если первая строка - перезапись
            *buffer=b;
        }
        else
        {
            *buffer|=b;
        }
        buffer++;
    }
}

void Row_LCD::debug_output_console()
{
    //Т.к. строка 8 пикселей в высоту, для печати используются 4 символа: ▀▄█
    const std::string symbols[4]= {" ","▄","▀","█"};
    for(unsigned short i=0; i<columns_in_row; i++)
    {
        unsigned char b=data[i];
        if (b!=0)
        {
            std::cout<<symbols[3];
        }
        else
        {
            std::cout<<symbols[0];
        }
    }
    std::cout<<std::endl;
}

void Row_LCD::save_to_bmp(const unsigned short y,BMP& AnImage)
{
    for(unsigned short x=0; x<columns_in_row; x++)
    {
        /*for(unsigned char i=0;i<=7;i++)
        {
          if ((data[x]&(1<<i))!=0)
          {*/
        if (data[x]!=0)
        {
            AnImage(x,y)->Red = 255;
            AnImage(x,y)->Green = 0;
            AnImage(x,y)->Blue = 0;
            AnImage(x,y)->Alpha = 0;
        }
    }
}
