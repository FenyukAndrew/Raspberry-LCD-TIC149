//Чтобы русские символы нормально отображались нужно, чтобы была кодировка файл с текстом Windows-1251 - т.е. main.cc
//Заполнение экрана данными происходит построчно (1 строка в высоту 8 пикселей) слева направо сверху вниз

//Для вывода на экран используется библиотека символов, закодированных с помощью Windows-1251
//Решение проблемы: 

class I2CBus;

/*class ProtoView
{
public:
	//virtual void action() = 0;
};*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

#include "EasyBMP/EasyBMP.h"

constexpr unsigned char DEFAULT_COLOR=1;

//Класс, содержащий одну строку
class Row_LCD
{
public:
	Row_LCD(const ushort _columns) : columns_in_row(_columns)
	{
		data=new unsigned char[_columns];
	};
	~Row_LCD() 
	{
		delete[] data;
		//printf("Destructor row_LCD\n");
	};
	void write(const ushort column,const unsigned char value) 
	{
		if (column<columns_in_row)//Проверить - сколько надо строк - может быть <=
		{
			data[column]=value;
		}
	};
	unsigned char read(const ushort column) const 
	{
		if (column<columns_in_row)
		{
			return data[column];
		}
		else
		{
			return 0;
		}
	};
	unsigned char* get_row() const
	{
		return data;
	}
	void clear_row()
	{
		for(ushort i=0;i<columns_in_row;i++)
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
	void point(const ushort _column,const unsigned char color=DEFAULT_COLOR)
	{
		if (_column>=columns_in_row) return;//Нет такой колонки
		data[_column]=color;
	}
	void debug_output_console();
    void save_to_bmp(const ushort y,BMP& AnImage);

private:
	const ushort columns_in_row;//Лишние 2 байта для каждой строки
	unsigned char* data;
};

class View_LCD;

//Универсальная структура данных для всех шрифтов
struct sSymbol 
{
		ushort width;
        const unsigned char* b;          // Data

//Символ рисует сам себя
        void DrawTo(const ushort x,const ushort y,View_LCD& m_View_LCD,const unsigned char value_PART_COUNT,const unsigned char color=DEFAULT_COLOR) const;
};
//Символ должен уметь рисовать себя
//Также можно будет вывод символа сделать по точным координатам - т.е. реализовать смещение и ИЛИ

class Font
{
public:
  Font(const unsigned char PART_COUNT) : value_PART_COUNT(PART_COUNT)
  {};

  ushort get_width_symbol(const unsigned char code_symbol) const 
  {
  	return p_Font[code_symbol].width;
  };
  void DrawTo(const ushort x,const ushort y,View_LCD& m_View_LCD,const unsigned char code_symbol,const unsigned char color=DEFAULT_COLOR) const //Рисование символа
  {
  	 p_Font[code_symbol].DrawTo(x,y,m_View_LCD,value_PART_COUNT,color);
  };

//private:
  sSymbol p_Font[256];
  //Может лучше массив указателей? тогда и инициализацию проще сделать
private:
  const unsigned char value_PART_COUNT;
};


//        columns (x)
//        0123456 .. 133
//        1
//rows(y) 2
//		 ..
//		 64

//Содержит все строки
class View_LCD// : public ProtoView
{
public:
	View_LCD(const ushort _columns,const ushort _rows);
	~View_LCD();

	constexpr static ushort MAX_WIDTH_LCD=0;//Для того, чтобы по умолчанию не вводить данные - можно сделать #FFFF

	void clear_lcd();

	void point(const ushort _column,const ushort _height,const unsigned char _color=DEFAULT_COLOR);
	void write_byte_to_buffer(const ushort _column,const ushort _rows,const unsigned char _data);//Для вывода теста

//Лучше словарь, в котором производить регистрцию загружаемых шрифтов
//Фабрики - может в этом случае поможет?

//x считается в пикелях
//y в строках (каждая строка 8 бит (пикселей)
	void print_lcd_8(const ushort x,const ushort y,const std::string& str1, ushort width=MAX_WIDTH_LCD, const unsigned char color=DEFAULT_COLOR);
	void print_lcd_16(const ushort x,const ushort y,const std::string& str1, ushort width=MAX_WIDTH_LCD, const unsigned char color=DEFAULT_COLOR);
	void print_lcd_24(const ushort x,const ushort y,const std::string& str1, ushort width=MAX_WIDTH_LCD, const unsigned char color=DEFAULT_COLOR);
	void print_lcd_32(const ushort x,const ushort y,const std::string& str1, ushort width=MAX_WIDTH_LCD, const unsigned char color=DEFAULT_COLOR);
//Вывод на экран строки определённой ширины, если больше - то не выводятся

	enum class e_font_height : char {size_8, size_16, size_24, size_32};
	void print_lcd(const e_font_height _font_height,const ushort x,const ushort y,const std::string& strIn, ushort width=MAX_WIDTH_LCD, const unsigned char _color=DEFAULT_COLOR);

    void drawTo(const ushort x,const ushort y,View_LCD& m_View_LCD,const unsigned char color=DEFAULT_COLOR);//Отображение сам себя в другой видовой экран

    void debug_output_console();
    void save_to_bmp(BMP& AnImage);

	static std::string iconv_recode(const std::string& from, const std::string& to, std::string text);
	static std::string recodeUTF8toCP1251(const std::string& text);

private:
	const ushort rows_in_screen;//Высота экрана
	const ushort columns_in_row;//Ширина экрана
	//Row_LCD** rows_LCD;//Указатель на массив указателей
	std::vector<Row_LCD*> rows_LCD;//Лучше использовать std::list

	void print_lcd_XX(const ushort x,const ushort y,const std::string& strIn, ushort width, const unsigned char color,const Font& m_Font);
};




class LCD_TIC149 : public View_LCD
{
public:
	LCD_TIC149(I2CBus* m_I2CBus,const unsigned char contrast);
	~LCD_TIC149();

	const static ushort width_LCD=133;
	const static ushort height_LCD=64;
	
	void init_lcd(unsigned char contrast);
	void lcd_view_invert();	
	void lcd_view_normal();
	void lcd_rotate_0();
	void lcd_rotate_180();
	void set_lcd_contrast(const unsigned char lcd_k);
	void clear_lcd_hardware();//Очищает только экран, а не массив
	void lcd_screen_old();
	
	void lcd_screen_buffer();//Работает с новым экранным буфером
	void lcd_screen_buffer_old();
	void write_lcd_screen_buffer_old(const ushort pos, const unsigned char value) {screen_buffer[pos]=value;};
	
//x считается в пикелях
//y в строках (каждая строка 8 бит (пикселей)
	void print_lcd_8_old(char x,char y,const std::string& str1, ushort width=width_LCD, unsigned char INVERT=0);
	void print_lcd_16_old(char x,char y,const std::string& str1, ushort width=width_LCD, unsigned char INVERT=0);
	void print_lcd_24_old(char x,char y,const std::string& str1, ushort width=width_LCD, unsigned char INVERT=0);
	void print_lcd_32_old(char x,char y,const std::string& str1, ushort width=width_LCD, unsigned char INVERT=0);
//Вывод на экран строки определённой ширины, если больше - то не выводятся

private:
	I2CBus* mI2CBus;//По идее лучше использовать ссылку - как правильно её инициализировать?

	const static unsigned char screen_logo[1064];
	
	constexpr static ushort size_screen_buffer=1064;
	unsigned char screen_buffer[size_screen_buffer];

};
