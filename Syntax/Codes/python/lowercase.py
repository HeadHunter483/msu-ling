# -*- coding: cp1251 -*-
import os
import io

with open('out.txt',encoding="utf8") as fin, open('temp7.txt','w',encoding="utf8") as fout:
    for line in fin:
        fout.write(line.lower())
		
with io.open("temp7.txt", mode="r", encoding="cp1251") as fd:
	content2 = fd.read()
	with io.open('out.txt', mode="w", encoding="cp1251",errors="ignore") as fd:
		fd.write(content2)
		
os.remove("temp7.txt")
	
	
	
