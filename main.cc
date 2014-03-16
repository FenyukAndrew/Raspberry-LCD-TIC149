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


 

int main(int argc, char *argv[])
{

	Weather m_Weather("http://export.yandex.ru/weather-ng/forecasts/34929.xml");
    std::cout << m_Weather.get_weather("temperature") << std::endl;
    std::cout << m_Weather.get_weather("weather_type") << std::endl;

    View_LCD* m_View_LCD=new View_LCD(64/8,133);
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
