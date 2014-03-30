#ifndef HEADER_FONTS
#define HEADER_FONTS 1
//Общий для всех шрифтов, сгенерированных LCDicon
struct sHeader
{
    //const char ccChar;              // optional char - Номер символа
    const int cxPix;//Повторяется для всех символов
    const int cyPix;//Необходимое количество выводимых байт (т.е. 19-x)
};

template<int T>
struct sUnitN
{
    const sHeader sH;               // header
    const unsigned char b[T];          // Data - Верхняя строка все нечётные - нижняя все чётные
};
constexpr unsigned char PART_COUNT8=1;
constexpr unsigned char PART_COUNT16=2;
constexpr unsigned char PART_COUNT24=3;
constexpr unsigned char PART_COUNT32=4;
//Маскимальная длина символа высотой 24 - определять по программе - посмотреть количество строк с данными в символе (в одной строке бможет быть больше 1 байта)
constexpr unsigned int SYM_8_LONG=7;
constexpr unsigned int SYM_16_LONG=19;
constexpr unsigned int SYM_24_LONG=32;
constexpr unsigned int SYM_32_LONG=42;

//typedef constexpr static sUnitN<SYM_16_LONG*PART_COUNT16> sUnit16;
typedef sUnitN<SYM_8_LONG*PART_COUNT8> sUnit8;
typedef sUnitN<SYM_16_LONG*PART_COUNT16> sUnit16;
typedef sUnitN<SYM_24_LONG*PART_COUNT24> sUnit24;
typedef sUnitN<SYM_32_LONG*PART_COUNT32> sUnit32;

/*
//Нужно вставить в каждое место использования
extern sUnit8 Arial_8[256];
extern sUnit16 Tahoma_16[256];
extern sUnit24 Tahoma_24[256];
extern sUnit32 Tahoma_32[256];
*/

#endif
