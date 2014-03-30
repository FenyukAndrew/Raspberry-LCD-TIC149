//Сделан автогенератором CodeBlocks
#ifndef LIB_FONTS_H
#define LIB_FONTS_H

#include <map>
#include "common_header.h"

//#include "Singleton.h"
class Font;

//Т.к. шрифты загружаются один раз, лучше использовать Майэрса - по завершению программы автоматически удалиться
class Lib_Fonts
{
    public:
        ~Lib_Fonts();
        static Lib_Fonts& Instance()
        {
            static Lib_Fonts m_Lib_Fonts;
            return m_Lib_Fonts;
        };
    Font* get_Font(Fnt e_Fnt);

    unsigned short get_width(Fnt e_Fnt,const std::string& str);

    private:
        Lib_Fonts();
        Lib_Fonts(const Lib_Fonts& other);
        Lib_Fonts& operator=(const Lib_Fonts& other);

    std::map<Fnt,Font*>* m_Fonts;
};

//Лучше не подсчет ссылок, а улучшенный или Маерса- тогда можно будет вызывать в любом месте
/*class Lib_Fonts : public Singleton<Lib_Fonts>
{
    public:
        ~Lib_Fonts();
        Lib_Fonts(const Lib_Fonts& other);
        Lib_Fonts& operator=(const Lib_Fonts& other);
    protected:
        Lib_Fonts();
        friend class Singleton<Lib_Fonts>;
    private:

    std::map<Fnt,Font*> m_Fonts;
};*/

#endif // LIB_FONTS_H
