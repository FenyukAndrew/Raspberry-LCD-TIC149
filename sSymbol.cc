#include "sSymbol.h"
#include "View_LCD.h"

        void sSymbol::DrawTo(const short x,const short y,View_LCD& m_View_LCD,const unsigned char value_PART_COUNT,const bool transparency,const unsigned char color) const
        {
        for(unsigned char i_lcd = 0; i_lcd < value_PART_COUNT; i_lcd++)//Делаем в несколько проходов
        {
                short offset_x_buffer=x;//133 точки - от 0 до 132 , но полусается начинаем с 1 - т.к. вначале идёт ++             
                for(int count = (width-1)*value_PART_COUNT; count >= 0; count-=value_PART_COUNT)
                {
                    //(rows_LCD[y+i_lcd])->write(offset_x_buffer,b[count+i_lcd]);
                    //m_View_LCD.write_byte_to_buffer(offset_x_buffer,y+i_lcd,b[count+i_lcd]);
                    for(unsigned char _bit=0;_bit<=7;_bit++)
                    {
                      unsigned char _c=((b[count+i_lcd]&(1<<_bit))!=0) ? DEFAULT_COLOR : 0;
                      if ((transparency) && (_c==0))
                      {//Если включена прозрачность
                      }
                      else
                      {
                        m_View_LCD.point(offset_x_buffer,y+i_lcd*8+7-_bit,_c);
                      }
                    }

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
