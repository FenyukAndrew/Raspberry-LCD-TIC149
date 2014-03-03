//Чтобы русские символы нормально отображались нужно, чтобы была кодировка файл с текстом Windows-1251 - т.е. main.cc
//Заполнение экрана данными происходит построчно (1 строка в высоту 8 пикселей) слева направо сверху вниз

class I2CBus;

class LCD_TIC149
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
	void clear_lcd();
	void lcd_screen();
	
	void lcd_screen_buffer();
	void write_lcd_screen_buffer(int pos, unsigned char value) {screen_buffer[pos]=value;};
	
//x считается в пикелях
//y в строках (каждая строка 8 бит (пикселей)
	void print_lcd_8(char x,char y,char* str1, int width=width_LCD, unsigned char INVERT=0);
	void print_lcd_16(char x,char y,char* str1, int width=width_LCD, unsigned char INVERT=0);
	void print_lcd_24(char x,char y,char* str1, int width=width_LCD, unsigned char INVERT=0);
	void print_lcd_32(char x,char y,char* str1, int width=width_LCD, unsigned char INVERT=0);
//Вывод на экран строки определённой ширины, если больше - то не выводятся


	
private:
	I2CBus* mI2CBus;//По идее лучше использовать ссылку - как правильно её инициализировать?

	const static unsigned char screen_logo[1064];
	
	const static int size_screen_buffer=1064;
	unsigned char screen_buffer[size_screen_buffer];

};
