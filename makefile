# в имени второй цели указаны два файла и для этой же цели не указана команда компиляции, кроме того, нигде явно не указана зависимость объектных файлов от "*.c"-файлов. 
#Дело в том, что команда make имеет предопределенные правила для получения файлов с определенными суффиксами. Так, для цели - объектного файла (суффикс ".o") 
#при обнаружении соответствующего файла с суффиксом ".c", будет вызван компилятор "сс -с" с указанием в параметрах этого ".c"-файла и всех файлов - зависимостей. 
#Более того, в этом случае явно не указанные ".c"-файлы make самостоятельно внесет в список зависимостей и будет реагировать их изменение так же, как и для явно указанных зависимостей.
OBJS = main.o I2CBus.o sSymbol.o Font.o Row_LCD.o View_LCD.o LCD_TIC149.o LCD_TIC149_Fast.o Weather.o EasyBMP.o Utils.o
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

sSymbol.o: sSymbol.cc sSymbol.h
	g++ -c -std=c++11 sSymbol.cc sSymbol.h
Font.o: Font.cc Font.h
	g++ -c -std=c++11 Font.cc Font.h
Row_LCD.o: Row_LCD.cc Row_LCD.h
	g++ -c -std=c++11 Row_LCD.cc Row_LCD.h
View_LCD.o: View_LCD.cc View_LCD.h
	g++ -c -std=c++11 View_LCD.cc View_LCD.h

LCD_TIC149.o: LCD_TIC149.cc LCD_TIC149.h description_font.h
	g++ -c -std=c++11 LCD_TIC149.cc LCD_TIC149.h description_font.h
LCD_TIC149_Fast.o: LCD_TIC149_Fast.cc LCD_TIC149_Fast.h
	g++ -c -std=c++11 LCD_TIC149_Fast.cc LCD_TIC149_Fast.h
Weather.o: Weather.cc Weather.h	
	g++ -c -std=c++11 Weather.cc Weather.h
EasyBMP.o: EasyBMP/EasyBMP.cc EasyBMP/EasyBMP*.h
	g++ $(CFLAGS) -c -std=c++11 EasyBMP/EasyBMP.cc EasyBMP/EasyBMP*.h
Utils.o:  Utils.cc	
	g++ -c -std=c++11 Utils.cc Utils.h
#$(ODIR)/%.o: %.cc 
#    gcc -c $(INC) -o $@ $< $(CFLAGS) 
# т.е. можно просто описать компиляцию всех файлов - без лишних проблем

clean:
	rm -f prog $(OBJS)
	rm -f *.h.gch *.*~ *~
	rm -f EasyBMP/*.h.gch EasyBMP/*.*~ EasyBMP/*~
	

