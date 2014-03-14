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

//Класс, содержащий одну строку
class row_LCD
{
public:
	row_LCD(const int _columns) : columns_in_row(_columns)
	{
		data=new unsigned char[_columns];
	};
	~row_LCD() {delete[] data;printf("Destructor row_LCD\n");};
	void write(const int column,const unsigned char value) 
	{
		if (column<columns_in_row)
			data[column]=value;
	};
	unsigned char read(const int column) const 
	{
		if (column<columns_in_row)
			return data[column];
		else
			return 0;
	};
	unsigned char* get_row() const
	{
		return data;
	}
	void point(const int _column,const unsigned char _height,const unsigned char color=1)
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
	}
private:
	const int columns_in_row;//Лишние 4 байта для каждой строки
	unsigned char* data;
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
	View_LCD(const int _columns,const int _rows);//height_LCD/8
	~View_LCD();


	void point(const int _column,const int _height,const unsigned char _color=1);

//Сюда перенести рисование текста	

	static std::string iconv_recode(const std::string& from, const std::string& to, std::string text);
	static std::string recodeUTF8toCP1251(const std::string& text);

private:
	const int rows_in_screen;
	row_LCD** rows_LCD;//Указатель на массив указателей
};




class LCD_TIC149 : public View_LCD
{
public:
	LCD_TIC149(I2CBus* m_I2CBus,unsigned char contrast);
	~LCD_TIC149();

	const static int width_LCD=133;
	const static int height_LCD=64;
	
	void init_lcd(unsigned char contrast);
	void lcd_view_invert();	
	void lcd_view_normal();
	void lcd_rotate_0();
	void lcd_rotate_180();
	void set_lcd_contrast(unsigned char lcd_k);
	void clear_lcd_old();//Очищает только экран, а не массив
	void lcd_screen_old();
	
	void lcd_screen_buffer_old();
	void write_lcd_screen_buffer_old(int pos, unsigned char value) {screen_buffer[pos]=value;};
	
//x считается в пикелях
//y в строках (каждая строка 8 бит (пикселей)
	void print_lcd_8_old(char x,char y,const std::string& str1, int width=width_LCD, unsigned char INVERT=0);
	void print_lcd_16_old(char x,char y,const std::string& str1, int width=width_LCD, unsigned char INVERT=0);
	void print_lcd_24_old(char x,char y,const std::string& str1, int width=width_LCD, unsigned char INVERT=0);
	void print_lcd_32_old(char x,char y,const std::string& str1, int width=width_LCD, unsigned char INVERT=0);
//Вывод на экран строки определённой ширины, если больше - то не выводятся

private:
	I2CBus* mI2CBus;//По идее лучше использовать ссылку - как правильно её инициализировать?

	const static unsigned char screen_logo[1064];
	
	const static int size_screen_buffer=1064;
	unsigned char screen_buffer[size_screen_buffer];

};
