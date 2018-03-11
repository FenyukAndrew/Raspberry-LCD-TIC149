#!/usr/bin/env python
import sys
import re

#-*- coding: utf-8 -*-
if len(sys.argv)<3:
    print 'Need input and output filename'
    sys.exit(1)
f = open(sys.argv[1],'r')
MASSIV = []
DataStr = []
numberPasses=0
numberRowData=0
for line in f.readlines():
    if line.find("{{") > -1:
        #print [ x for x in re.split( r'{{([0-9]+),([ ]+)([0-9]+)},{', line )]
        result=re.split( r'{{([0-9]+),([ ]+)([0-9]+)},{', line )
        numberPasses=int(result[1])/8
        numberRowData=int(result[3]) #Number of rows, which need write to output file
        MASSIV.append(numberRowData)
    else:
        if (numberRowData>0):
            DataStr.append(line)
            numberRowData=numberRowData-1
fout = open(sys.argv[2], 'w')
fout.write('unsigned char []={\n')
fout.write(format(numberPasses,'d')+',\n')
Z=0
for key in MASSIV:
    fout.write(format(key,'d')+',')
    Z=Z+1
    if Z % 16 ==0:
        fout.write('\n')
numberRows=len(DataStr)
for value in DataStr[:(numberRows-2)]:
    r1=value.split(', ')
    if len(r1)==2:
        fout.write(value)
    else:
		i_space=index(pathStr[1]," ")
		firstPart=substr(pathStr[1],0,i_space-1) #Чтобы пробел не вставлялся 2 раза
		secondPart=substr(pathStr[1],i_space,length(pathStr[1]))
		return firstPart","secondPart
#Last row - delete ','        
str=DataStr[numberRows-1]    
fout.write('};\n')
