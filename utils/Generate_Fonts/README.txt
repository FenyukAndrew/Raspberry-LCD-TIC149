Генерация шрифтов производиться с помощью программы LCDIcon
затем в целях уменьшения размера .h файл приводиться к компактному размеры с помощью perl - удаляются пустые строки

Запуск скрипта AWK
awk -f 'Script.awk' Arial_8_original.h >Arial_8.h
awk -f 'Script.awk' Tahoma_16_original.h >Tahoma_16.h
awk -f 'Script.awk' Tahoma_24_original.h >Tahoma_24.h
awk -f 'Script.awk' Tahoma_32_original.h >Tahoma_32.h

perl Script.pl

python Script.py Tahoma_16_original.h Tahoma_16_PY.h

Результат компиляции с оригинальными файлами от LCDIcon
231.3 KiB (236,871 bytes)

После переделывания под новую структуру шрифтов (убран быстрый вывод на экран)
108.3 KB (108,316 bytes)

///TODO: Сделать тоже самое на perl и python
///Сравнить результат на правильность

