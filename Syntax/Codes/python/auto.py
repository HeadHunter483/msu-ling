import re
import codecs
import subprocess
import os
import sys

path = os.path.dirname(os.path.abspath(__file__))

mas=[]

text = ''.join(open('in.txt',encoding="utf-8").readlines())
word=text.split()

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))

def splitter():
    i=0
    j=0
    str1=''
    while (i<len(word)):
        if word[i]!='.':
            str1=str1+' '+word[i]     
        if word[i]=='.'or word[i]=='?' or word[i]=='!':
            str1=str1+' .'
            mas.append(str1)
            str1=''
    
        i=i+1

    while (j<len(mas)):
        if "" in mas[j][0]:
            mas[j]=mas[j][:0]+mas[j][1:]
      
        j=j+1

subprocess_cmd('python clean.py auto 1')
splitter()
i=0

for i in range(len(mas)):
    print(mas[i])
    os.chdir(path)
    f=codecs.open("in.txt","w","utf-8")
    f.write(mas[i])
    f.close()
    subprocess_cmd('python num.py')
    subprocess_cmd('python converter.py in.txt utf8') 
    subprocess_cmd('python morph.py ')
    os.chdir(path + '/bin/Malt')
    subprocess_cmd('java -jar maltparser-1.8.1.jar -c PTM -i ' + path +'\\' + '\\tmp' + '\\' + '\\in_raw.conll -o out.txt -m parse')
    subprocess_cmd('python lowercase.py') 
  
    print('6.Исправление ошибок:')
    subprocess_cmd('python converter.py out.txt ansi') 
    subprocess_cmd('Errors.exe auto')
    subprocess_cmd('python converter.py final.txt utf8') 

    print('7.Визуализация синтаксического дерева: '+str(i+1))
    subprocess_cmd('python '+path+'\\'+'\\graph.py '+str(i+1))
		
    if((i+1) == len(mas)):  # если это последняя итерация
        os.chdir(path)   # переходим в родительскую директорию
        subprocess_cmd("copy " + path + "\\bin\\Malt\\temp2.txt " + path+" /Y") # копируем файл-результат в исходную папку
        os.rename('temp2.txt','out.txt') # переименовываем
        subprocess_cmd("copy " + path + "\\bin\\Malt\\semantic.txt " + path+" /Y") # копируем файл-результат в исходную папку
        os.chdir(path)   # переходим в родительскую директорию
        o=0
        for o in range(len(mas)):
            f=codecs.open("in.txt","w","utf-8")
            f.write(mas[o])
            f.close()

        print('Finished !')


