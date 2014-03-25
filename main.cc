#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctime>

#include "I2CBus.h"
#include "LCD_TIC149.h"
#include "Weather.h"

#include "EasyBMP/EasyBMP.h"

int main(int argc, char *argv[])
{
	/*std::string str;
	//for(short i=32;i<255;i++) str+=i;
	str="РўРµСЃС‚РѕРІР°СЏ СЃС‚СЂРѕРєР°!Тестовая строка!";
	std::cout << str << std::endl;
    //Все символы <32 выводить пробел?

	std::string str4=View_LCD::recodeUTF8toCP1251(str);
	std::cout << str4 << std::endl;

	std::string str5=View_LCD::iconv_recode("CP1251","UTF-8",str);;
	std::cout << str5 << std::endl;*/

	/*for(int a=0;a<32;a++)
	{
	for(int y=0;y<133/2;y++)
	{
		std::cout << "▀ ";
		//std::cout << "█ ";//▌▐";//y%10;
		//std::cout << "▌";//▌▐";//y%10;
	}
	std::cout << std::endl;
	}*/
	//Возврат к началу экрана
	//CSI n F 	CPL – Cursor Previous Line 	Перемещает курсор в начало n-ой (по умолчанию 1-ой) строки сверху относительно текущей.
	//std::cout << "\x1B[32F" << "*Currect position*" << std::endl;
	//std::cout << "\x1b[3m"; Цвет не прошел

	Weather m_Weather("http://export.yandex.ru/weather-ng/forecasts/34929.xml");
    std::cout << m_Weather.get_weather("temperature") << std::endl;
    std::cout << m_Weather.get_weather("weather_type") << std::endl;

	BMP AnImage;
	//AnImage.SetSize(133,64);
	AnImage.SetBitDepth(24);

    View_LCD* m_View_LCD=new View_LCD(133,64);

    //Очистка экрана
    for(ushort x=0;x<133;x++)
    {
    	for(ushort y=0;y<64;y++)
    	{
    		m_View_LCD->point(x,y,0);
    	}
    }

    for(ushort y=0;y<64;y++)
    {
    	m_View_LCD->point(y,y);
    }
    //m_View_LCD->print_lcd_8(0,0,"1234567890");
    m_View_LCD->print_lcd_16(0,0,"1as");
    //m_View_LCD->print_lcd_24(0,0,"1DFG");

    m_View_LCD->save_to_bmp(AnImage);
    //m_View_LCD->debug_output_console();
	AnImage.WriteToFile("Output.bmp");	
    delete m_View_LCD;

	I2CBus mI2CBus("/dev/i2c-1",0x3c);
	LCD_TIC149* mLCD_TIC149=new LCD_TIC149(&mI2CBus,61);
	
	//mLCD_TIC149->lcd_view_invert();
	mLCD_TIC149->clear_lcd_hardware();
	//mLCD_TIC149->lcd_screen_old();
	
	//mLCD_TIC149->lcd_rotate_180();
	
	/*mLCD_TIC149->print_lcd_24_old(0,0,"VALEK красавец");
	mLCD_TIC149->print_lcd_16_old(0,3,"VALEK красавец");
	mLCD_TIC149->print_lcd_8_old(0,5,"VALEK красавец");
	mLCD_TIC149->print_lcd_32_old(0,6,"012345");*/

	//mLCD_TIC149->print_lcd_32_old(0,0,"0123456");
	//mLCD_TIC149->print_lcd_32_old(0,4,"7890123");

	char buffer[50];
	for(int i=0;i<15;i++)
	{
		time_t t = time(0);   // get time now
	    struct tm * now = localtime( & t );		

	    sprintf(buffer, "%02d:%02d:%02d",now->tm_hour,now->tm_min,now->tm_sec);
		mLCD_TIC149->print_lcd_32_old(0,0,buffer);
		mLCD_TIC149->lcd_screen_buffer_old();
		sleep(1);
	}
			
	delete(mLCD_TIC149);
}
