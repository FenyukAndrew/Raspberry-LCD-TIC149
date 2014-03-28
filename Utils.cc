#include <iconv.h>

#include "Utils.h"

std::string Utils::iconv_recode(const std::string& from, const std::string& to, std::string text)
{
    iconv_t cnv = iconv_open(to.c_str(), from.c_str());
    if (cnv == (iconv_t) - 1)
    {
        iconv_close(cnv);
        return "";
    }
    char *outbuf;
    if ((outbuf = (char *) malloc(text.length()*2 + 1)) == NULL)
    {
        iconv_close(cnv);
        return "";
    }
    char *ip = (char *) text.c_str(), *op = outbuf;
    size_t icount = text.length(), ocount = text.length()*2;

    if (iconv(cnv, &ip, &icount, &op, &ocount) != (size_t) - 1)
    {
        outbuf[text.length()*2 - ocount] = '\0';
        text = outbuf;// после вызова iconv адреса разные. Результат работы iconv находится в outbuf, а op указывает на другой не коректный адрес
    }
    else
    {
        text = "";
    }

    free(outbuf);
    iconv_close(cnv);
    return text;
}

std::string Utils::recodeUTF8toCP1251(const std::string& text)
{
    return iconv_recode("UTF-8","CP1251",text);
}