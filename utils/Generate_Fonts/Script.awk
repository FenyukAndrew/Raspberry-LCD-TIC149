function repairDelimiter(input_str, delimiter)
{
	z=split (DataStr[j], pathStr, ", ");
	#print "Z=" z
	if (z==2)
	{
		return pathStr[1] ", " pathStr[2]
	}
	else
	{
		i_space=index(pathStr[1]," ")
		firstPart=substr(pathStr[1],0,i_space-1) #Чтобы пробел не вставлялся 2 раза
		secondPart=substr(pathStr[1],i_space,length(pathStr[1]))
		return firstPart","secondPart
	}
}

function makeLastRow(input_str)
{
	z=split (input_str, pathStr, ", ");
	#print "Z=" z
	if (z==2)
		print pathStr[1] "  " pathStr[2]
	else
	print pathStr[1]
}

BEGIN {
	X=0
	NumberStr=0
	#print FILENAME Все равно ничего не печатает!
	#не выводит ни одного входного параметра, только имя входного файла
	#for ( i = 1; i < ARGC; i++ )
	#{
    #	printf "ARGV=%s ", ARGV[i]
	#}
	#printf "\n"
}

#Шаблон поиска и начало операции разделения
/{{/ {
	gsub(/{/,"")
	gsub(/}/,"")
	i=split ($0, M, ",");
	NumberPasses=M[1]/8 #Число проходов
	#print NumberPasses "-" M[2]
	X=X+1
	MASSIV[X]=M[2]
	for (j=1; j<=M[2]; j++)
	{
		getline
		#вначале все строки помещаются в массив, чтобы затем их вывести в правильном порядке
		NumberStr=NumberStr+1
		DataStr[NumberStr]=$0
		#print $0
		#чтение следующей строки
		#есть проблема, если будут взяты все байты, то в конце не будет запятой
	}
}

END {
	print "unsigned char " FILENAME "[]={"
	#Первый байт указывает число проходов в символе <= ЗАДАТЬ ВРУЧНУЮ
	printf "%s,\n",NumberPasses;
	#Вначале выводиться массив количества строк в символе (число байт=число строк*число проходов)
	for (j=1; j<=256; j++)
	{
		printf MASSIV[j] ",";
		if ((j%16)==0)
			printf "\n"
	}
	#вывод данных
	for (j=1; j<NumberStr; j++)
	{
		#Если в строке только 2 разделителя "," - то добавление в конец ","
		print repairDelimiter(DataStr[j], ",")
	}
	#Последняя строка не должна содержать ","
	makeLastRow(DataStr[NumberStr])
	print "};"
}
