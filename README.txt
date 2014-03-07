Проект для работы с экраном на C++

Может есть смысл представить экран не в виде массива - а в виде строк, объединенных в список (через список указателей). Не потребуется тупое умножение. Кодирование символов по-любому не менять - т.к. это стандрт, генерируемый программой.

class View в конструктор передается размер шрифта и текст, экран
в класс экрана - т.к. он содержит шрифты (вынести в отдельный класс?), функция для определения длины строки в определенном шрифте.
Можно экран и класс View (вид) объединить как наследование от общего класса для рисования (который содержит шрифты)
View тоже можно проецировать на View - эта функция может смещать влево и вправо - вызывать в общем цикле через usleep(40) - 25 кадров в секунду

//Конфигурация дисплея - построчно (1 строка в высоту 8 пикселей) слева направо сверху вниз
unsigned char* row_screen[height_LCD/8];
class row_LCD
{
row_LCD(int column) {new data(column);};
~row_LCD(int column) {delete data(column);};
public:
write(int column);
read(int column) const;
const* get_row(int i) const;//Посмотреть итераторы
private:
unsigned char* data;
}

abstuct class ProtoView
class View : public ProtoView
{
row_LCD* rows[height_LCD/8];
}
Может сделать массив классов row_lcd - кажется будут проблемы, если будет наследование - сделать isolated?



***** СТАРОЕ

Компиляция одного файла
gcc main.cc -o main

С использованием std заработало только при
g++ main.cc -o main

Периодически возникает ошибка
classA.h:1:1: error: unknown type name ‘class’
classA.h:2:1: error: expected ‘=’, ‘,’, ‘;’, ‘asm’ or ‘__attribute__’ before ‘{’ token
РЕШЕНИЕ: повторная перекомпиляция
Что-то часто возникают подобные ошибки - нужен ReBuild All?

Запуск sudo ./main


const int* - слева от * - данные
int* const - справа от * - константный указатель
http://books.google.ru/books?id=uPvvxRYj-NIC&pg=PA380&lpg=PA380&dq=C%2B%2B+const+%D1%81%D0%BB%D0%B5%D0%B2%D0%B0+%D0%BE%D1%82+*&source=bl&ots=K5mme-V8bM&sig=tPk_0Vzqx5yivPZRbK9VykQnijY&hl=ru&sa=X&ei=V2n8UsHLF_H44QTw0ICoAQ&ved=0CC8Q6AEwAQ#v=onepage&q=C%2B%2B%20const%20%D1%81%D0%BB%D0%B5%D0%B2%D0%B0%20%D0%BE%D1%82%20*&f=false


//Страуструп - прочитано 5-5.2
Читать 5.3 - Но перед этим нужно прочитать главу 3 - т.к. код построен на программе калькулятор стр. 70


Основной проект:
компиляция sudo make
запуск sudo ./prog


