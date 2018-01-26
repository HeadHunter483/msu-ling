#!/usr/bin/python
# -*- coding: utf-8 -*-
 
import os
import sys
import chardet
import io
 
def converter(filePath):
    if any([filePath.endswith(extension) for extension in '.srt,.ass,.txt'.split(',')]):
        with open(filePath, "rb") as F:
            text = F.read()
            enc = chardet.detect(text).get("encoding")
            if enc and enc.lower() != "utf-8":
                try:
                    text = text.decode(enc)
                    text = text.encode("utf-8")
                    with open(filePath, "wb") as f:
                        f.write(text)
                        print (u"%s сконвертирован в utf8." % filePath)
                except:
                    print (u"Ошибка в имени файла: название содержит русские символы.")
            else :
                print (u"Файл %s находится в кодировке %s и не требует конвертирования." % (filePath, enc))
            print ('-'*40)
 
if __name__ == '__main__':
	param_name = sys.argv[1]
	param_value = sys.argv[2]
	if(param_value=="utf8"):
		if len(sys.argv[1:]) == 0:
			try:
				path = raw_input(u"Input path or file:")
			except KeyboardInterrupt:
				print (u"Ввод отменён.")
				sys.exit(0)
		else:
			path = sys.argv[1:][0]
		if os.path.isdir(path):
			last = path[-1]
			if last.endswith('/'):
				path = path
			else :
				path = path+"/"
			print (path)
			for (path, dirs, files) in os.walk(path):
				for file in files:
					filePath = path+file
					filePath = filePath.decode("utf-8")
					converter(filePath)
		elif os.path.isfile(path) :
			converter(path)
	if(param_value=="ansi"):
		with io.open(param_name, mode="r", encoding="utf8") as fd:
			content1 = fd.read()
		with io.open("temp8.txt", mode="w", encoding="cp1251",errors="ignore") as fd:
			fd.write(content1)
		myfile = open(param_name,"w", encoding="cp1251")
		myfile.truncate()
		myfile.close()
		with io.open("temp8.txt", mode="r", encoding="cp1251") as fd:
			content2 = fd.read()
		with io.open(param_name, mode="w", encoding="cp1251",errors="ignore") as fd:
			fd.write(content2)
		os.remove("temp8.txt")
		print(param_name+" сконвертирован в ansi.")
		fd.close()
sys.exit(0)
	
