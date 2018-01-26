# -*- coding: cp1251 -*-

with open('out.txt',encoding="utf8") as fin, open('output.txt','w',encoding="utf8") as fout:
    for line in fin:
        fout.write(line.lower())

	
	
	
	
