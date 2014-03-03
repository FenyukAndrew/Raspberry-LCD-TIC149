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

int main()
{
	I2CBus mI2CBus("/dev/i2c-1",0x3c);
	LCD_TIC149* mLCD_TIC149=new LCD_TIC149(&mI2CBus,61);
	
	//mLCD_TIC149->lcd_view_invert();
	mLCD_TIC149->clear_lcd();
	//mLCD_TIC149->lcd_screen();
	
	//mLCD_TIC149->lcd_rotate_180();
	
	/*mLCD_TIC149->print_lcd_24(0,0,"VALEK ��������");
	mLCD_TIC149->print_lcd_16(0,3,"VALEK ��������");
	mLCD_TIC149->print_lcd_8(0,5,"VALEK ��������");
	mLCD_TIC149->print_lcd_32(0,6,"012345");*/

	//mLCD_TIC149->print_lcd_32(0,0,"0123456");
	//mLCD_TIC149->print_lcd_32(0,4,"7890123");

	char buffer[50];
	for(int i=0;i<15;i++)
	{
		time_t t = time(0);   // get time now
	    struct tm * now = localtime( & t );		

	    sprintf(buffer, "%02d:%02d:%02d",now->tm_hour,now->tm_min,now->tm_sec);
		mLCD_TIC149->print_lcd_32(0,0,buffer);
		mLCD_TIC149->lcd_screen_buffer();
		sleep(1);
	}
			
	delete(mLCD_TIC149);
}
