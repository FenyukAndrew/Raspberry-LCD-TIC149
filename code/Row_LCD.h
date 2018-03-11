#include "common_header.h"

class BMP;

//Класс, содержащий одну строку
class Row_LCD
{
public:
    Row_LCD(const unsigned short _columns) : columns_in_row(_columns)
    {
        data=new unsigned char[_columns];
        clear_row();//Очистка массива - т.к. там будут случайные значения
    };
    ~Row_LCD()
    {
        delete[] data;
        //printf("Destructor row_LCD\n");
    };
    void write(const unsigned short column,const unsigned char value)
    {
        if (column<columns_in_row)//Проверить - сколько надо строк - может быть <=
        {
            data[column]=value;
        }
    };
    unsigned char get_color(const short x) const
    {
        if ((x>=0) && (x<columns_in_row))
        {
            return data[x];
        }
        else
        {
            return 0;
        }
    };
    const unsigned char* get_row() const
    {
        return data;
    }
    void encoder_row(unsigned char* buffer,const unsigned char count_rows) const;

    void clear_row()
    {
        for(unsigned short i=0; i<columns_in_row; i++)
        {
            data[i]=0;
        }
    }
    /*void point(const ushort _column,const unsigned char _height,const unsigned char color=DEFAULT_COLOR)
    {
    	if (_column>=columns_in_row) return;//Нет такой колонки
    	if (_height>7) return;
    	if (color!=0)
    	{
    		data[_column]|=(1<<_height);//Может оказаться (7-_height)
    	}
    	else
    	{
    		data[_column]&=~(1<<_height);//Может оказаться (7-_height)
    	}
    }*/
    void point(const short x,const unsigned char color=DEFAULT_COLOR)
    {
        if ((x<0) || (x>=columns_in_row)) return;//Нет такой колонки
        data[x]=color;
    }
    void debug_output_console();
    void save_to_bmp(const unsigned short y,BMP& AnImage);

private:
    const unsigned short columns_in_row;//Лишние 2 байта для каждой строки
    unsigned char* data;
};
