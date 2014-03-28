#include <string>
#include <vector>

#include "common_header.h"

class Font;
class BMP;
class Row_LCD;

//Рисование - указывается верхний левый угол текста
//        columns (x)
//        0123456 .. 133
//        1
//rows(y) 2
//		 ..
//		 64

//Возможные типы высот шрифтов
enum class Font_height : char {h8, h16, h24, h32};

//Содержит все строки
class View_LCD// : public ProtoView
{
public:
	View_LCD(const unsigned short _columns,const unsigned short _rows);
	~View_LCD();

	constexpr static unsigned short MAX_WIDTH_LCD=0;//Для того, чтобы по умолчанию не вводить данные - можно сделать #FFFF

	void clear_lcd();

	void point(const short x,const short y,const unsigned char _color=DEFAULT_COLOR);
	unsigned char get_color(const short x,const short y) const;
	//void write_byte_to_buffer(const ushort _column,const ushort _rows,const unsigned char _data);//Для вывода теста

//Лучше словарь, в котором производить регистрцию загружаемых шрифтов
//Фабрики - может в этом случае поможет?

//x считается в пикелях
//y в строках (каждая строка 8 бит (пикселей)
	/*void print_lcd_8(const short x,const short y,const std::string& str1, ushort width=MAX_WIDTH_LCD, const unsigned char color=DEFAULT_COLOR);
	void print_lcd_16(const short x,const short y,const std::string& str1, ushort width=MAX_WIDTH_LCD, const unsigned char color=DEFAULT_COLOR);
	void print_lcd_24(const short x,const short y,const std::string& str1, ushort width=MAX_WIDTH_LCD, const unsigned char color=DEFAULT_COLOR);
	void print_lcd_32(const short x,const short y,const std::string& str1, ushort width=MAX_WIDTH_LCD, const unsigned char color=DEFAULT_COLOR);*/
//Вывод на экран строки определённой ширины, если больше - то не выводятся

	void print_lcd(const Font_height _font_height,const short x,const short y,const std::string& strIn,const bool transparency=false, unsigned short width=MAX_WIDTH_LCD, const unsigned char _color=DEFAULT_COLOR);
	void print_lcd(const unsigned char _font_height,const short x,const short y,const std::string& strIn,const bool transparency=false, unsigned short width=MAX_WIDTH_LCD, const unsigned char _color=DEFAULT_COLOR);

	void line(short x1, short y1, const short x2, const short y2,const unsigned char color_code=DEFAULT_COLOR);
	void circle(const short x_center, const short y_center, const short radius, const unsigned char color_code=DEFAULT_COLOR);
	void rectangle(short x1, short y1, short x2, short y2, const unsigned char color_code=DEFAULT_COLOR);

    void drawTo(const short x,const short y,View_LCD& m_View_LCD,const bool transparency=false) const;//Отображение сам себя в другой видовой экран

    void debug_output_console() const;
    void debug_output_console_compact() const;//Если в высоту нечетное число строк, то последняя нечетная невыводиться не будет
    void save_to_bmp(BMP& AnImage) const;

private:
	const unsigned short rows_in_screen;//Высота экрана
	const unsigned short columns_in_row;//Ширина экрана
	//Row_LCD** rows_LCD;//Указатель на массив указателей
	std::vector<Row_LCD*> rows_LCD;//Лучше использовать std::list

	void print_lcd_XX(short x,const short y,const std::string& strIn,const bool transparency,ushort width,const unsigned char color,const Font& m_Font);
	void plot_circle(const int x, const int y, const short x_center, const short y_center, const unsigned char color_code);

Font* font_Arial_8;
Font* font_Tahoma_16;
Font* font_Tahoma_24;
Font* font_Tahoma_32;
};