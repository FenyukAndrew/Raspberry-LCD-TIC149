#include <unistd.h>

#include <fcntl.h>

#include "I2CBus.h"
#include "LCD_TIC149.h"
#include "Weather.h"

#include "EasyBMP/EasyBMP.h"

#include "LCD_TIC149_Fast.h"

#include "common_header.h"

#include "Lib_Fonts.h"

//Чтобы включился в выходной файл
#include "description_font.h"
#include "Arial_8.h"
#include "Tahoma_16.h"
#include "Tahoma_24.h"
#include "Tahoma_32.h"

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

    //std::cerr<<"Standart Error"<<std::endl;

    Weather m_Weather("http://export.yandex.ru/weather-ng/forecasts/34929.xml");
    //std::cout << m_Weather.get_weather("temperature") << std::endl;
    //std::cout << m_Weather.get_weather("weather_type") << std::endl;

    std::string weather_str="T="+m_Weather.get_weather("temperature")+" "+m_Weather.get_weather("weather_type");

    BMP AnImage;
    //AnImage.SetSize(133,64);
    AnImage.SetBitDepth(1);

    View_LCD* m_View_LCD=new View_LCD(133,64);

    std::string full_weather_str="00:00:00 "+weather_str;//"00:00:00 Это очень длинная строка. This string is very long"
    unsigned short w1=Lib_Fonts::Instance().get_width(Fnt::h32,full_weather_str);
    View_LCD* m_View1=new View_LCD(w1,32);
    m_View1->print_lcd(Fnt::h32,0,0,full_weather_str);
    m_View1->line(0,31,w1-5,31);

    std::string s3=" Валек красавец в натуре. Валек красавец в натуре.";
    unsigned short w3=Lib_Fonts::Instance().get_width(Fnt::h16,s3);
    View_LCD* m_View3=new View_LCD(w3,17);
    m_View3->print_lcd(Fnt::h16,0,0,s3);
    //m_View1->line(0,0,890,30);

    /*m_View_LCD->point(0,0);
    m_View_LCD->line(0,0,132,63);
    m_View_LCD->rectangle(0,0,132,63);//Т.к. нумерация с 0
    m_View_LCD->circle(64,32,32);

    //m_View_LCD->print_lcd(Font_height::h8,0,0,"1234567890");
    m_View_LCD->print_lcd(Fnt::h8,0,0,"1234567890",true);
    m_View_LCD->print_lcd(Fnt::h16,0,8,"VALEK красавец",true);
    m_View_LCD->print_lcd(Fnt::h24,0,24,"1DFG",true);

    m_View_LCD->save_to_bmp(AnImage);
    AnImage.WriteToFile("Output.bmp");
    //m_View_LCD->debug_output_console();
    m_View_LCD->debug_output_console_compact();*/

    char buffer[50];
    char last_sec=-1;

//Возможно из-за этого проблемы с отображением на Rasp
    signed char sign_step=1;
    signed char step=0;

    for(int i=0; i<1100; i++)
    {

        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );

        if (last_sec!=now->tm_sec)//Вызывать нужно 1 раз в секунду
        {
            sprintf(buffer, "%02d:%02d:%02d",now->tm_hour,now->tm_min,now->tm_sec);
            m_View1->print_lcd(Fnt::h32,0,0,buffer);
            last_sec=now->tm_sec;
        }
        step+=sign_step;
        if (step>19) sign_step=-1;
        if (step<0) sign_step=1;

        m_View1->drawTo(150-i,0,*m_View_LCD);
        m_View3->drawTo(i-550,30+step,*m_View_LCD);//Нужна функция вычисляющая ширину строки
        m_View_LCD->debug_output_console_compact();
        std::cout << "\x1B[32F";//Возврат на 32 строки вверх
        //sleep(1);
        usleep(25000);
    }

    /*char buffer[50];
    for(int i=0;i<15;i++)
    {
    	time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );

        sprintf(buffer, "%02d:%02d:%02d",now->tm_hour,now->tm_min,now->tm_sec);
    	m_View_LCD->print_lcd(32,0,0,buffer);
        m_View_LCD->debug_output_console_compact();
        std::cout << "\x1B[32F";//Возврат на 32 строки вверх
    	sleep(1);
    }*/

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

    /*char buffer[50];
    for(int i=0;i<15;i++)
    {
    	time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );

        sprintf(buffer, "%02d:%02d:%02d",now->tm_hour,now->tm_min,now->tm_sec);
    	mLCD_TIC149->print_lcd_32_old(0,0,buffer);
    	mLCD_TIC149->lcd_screen_buffer_old();
    	sleep(1);
    }*/

    delete(mLCD_TIC149);
}
