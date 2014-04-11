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
    Weather m_Weather("http://export.yandex.ru/weather-ng/forecasts/34929.xml");

    std::string weather_str="T="+m_Weather.get_weather("temperature")+" "+m_Weather.get_weather("weather_type");

    //Для тестового вывода в консоль
    //View_LCD* m_View_LCD=new View_LCD(133,64);

    I2CBus mI2CBus("/dev/i2c-1",0x3c);
    LCD_TIC149* m_View_LCD=new LCD_TIC149(&mI2CBus,61);
    //m_View_LCD->lcd_view_invert();
    m_View_LCD->clear_lcd_hardware();

    std::string full_weather_str="00:00:00 "+weather_str+" ";//"00:00:00 Это очень длинная строка. This string is very long"
    unsigned short w1=Lib_Fonts::Instance().get_width(Fnt::h32,full_weather_str);
    View_LCD* m_View1=new View_LCD(w1,32);
    m_View1->print_lcd(Fnt::h32,0,0,full_weather_str);
    m_View1->line(0,31,w1-5,31);

    std::string s3=" Валек красавец в натуре. Валек красавец в натуре.";
    unsigned short w3=Lib_Fonts::Instance().get_width(Fnt::h16,s3);
    View_LCD* m_View3=new View_LCD(w3,17);
    m_View3->print_lcd(Fnt::h16,0,0,s3);

    char buffer[50];
    char last_sec=-1;

    signed char sign_step=1;
    signed char step=0;

    for(int i=0; i<1100; i=i+4)
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

        //m_View_LCD->debug_output_console_compact4();
        m_View_LCD->push_buffer_to_lcd_screen();

        std::cout << "\x1B[32F";//Возврат на 32 строки вверх
        //sleep(1);
        usleep(50000);//25000, 10000 видны артефакты - экран слишком медленный
    }

    delete m_View_LCD;
    delete m_View3;
}
