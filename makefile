# в имени второй цели указаны два файла и для этой же цели не указана команда компиляции, кроме того, нигде явно не указана зависимость объектных файлов от "*.c"-файлов. 
#Дело в том, что команда make имеет предопределенные правила для получения файлов с определенными суффиксами. Так, для цели - объектного файла (суффикс ".o") 
#при обнаружении соответствующего файла с суффиксом ".c", будет вызван компилятор "сс -с" с указанием в параметрах этого ".c"-файла и всех файлов - зависимостей. 
#Более того, в этом случае явно не указанные ".c"-файлы make самостоятельно внесет в список зависимостей и будет реагировать их изменение так же, как и для явно указанных зависимостей.
OBJS = main.o I2CBus.o LCD_TIC149.o Weather.o EasyBMP.o
#ODIR = obj
#OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))
#OBJS = _OBJS

#только для EasyBMP
CFLAGS = -O3 -pipe -fomit-frame-pointer -funroll-all-loops -s

prog: $(OBJS)
	g++ -o prog $(OBJS)  -lcurl -ltinyxml
main.o: main.cc 
	g++ -c -std=c++11 main.cc
I2CBus.o: I2CBus.cc I2CBus.h
	g++ -c -std=c++11 I2CBus.cc I2CBus.h
LCD_TIC149.o: LCD_TIC149.cc LCD_TIC149.h
	g++ -c -std=c++11 LCD_TIC149.cc LCD_TIC149.h
Weather.o: Weather.cc Weather.h	
	g++ -c -std=c++11 Weather.cc Weather.h
EasyBMP.o: EasyBMP/EasyBMP.cc EasyBMP/EasyBMP*.h
	g++ $(CFLAGS) -c -std=c++11 EasyBMP/EasyBMP.cc EasyBMP/EasyBMP*.h
#$(ODIR)/%.o: %.cc 
#    gcc -c $(INC) -o $@ $< $(CFLAGS) 
# т.е. можно просто описать компиляцию всех файлов - без лишних проблем

clean:
	rm -f prog $(OBJS)
	rm -f *.h.gch *.*~ *~
	rm -f EasyBMP/*.h.gch EasyBMP/*.*~ EasyBMP/*~
	

