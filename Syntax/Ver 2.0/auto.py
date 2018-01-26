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

print('\n    Синтаксический анализатор математических текстов ver 1.2\n')
print('\n  Этап I. Удаление результатов прошлого запуска\n ')
subprocess_cmd('python clean.py auto 1')
splitter()
i=0

for i in range(len(mas)):
    print('\nВ данный момент разбирается следующее предложение: '+mas[i])
    os.chdir(path)
    f=codecs.open("in.txt","w","utf-8")
    f.write(mas[i])
    f.close()
    #print('\n  Этап II. Преобразование входного файла к нужной кодировке:\n ')
    subprocess_cmd('python converter.py in.txt utf8')
    #print('\n  Этап III. Преобразование чисел написанных прописью в числовой формат:\n ')
    subprocess_cmd('python num.py')
    print('\n  Этап IV. Морфологический анализ:\n ')
    subprocess_cmd('python morph.py ')
    os.chdir(path+'\\tmp')
    subprocess_cmd('python morph_corrector.py ')
    os.chdir(path)
    #print('\n  Этап V. Этап работы с неделимыми выражениями:\n ')
    subprocess_cmd('python skleika.py ')
    #print('\n  Этап VI. Синтаксический анализ :\n ')
    subprocess_cmd('python syntax_head.py ')
    #print('\n  Этап VII. Визуализация синтаксического графа: (Отключено)\n ')

os.chdir(path)   # переходим в родительскую директорию
i=0
f=codecs.open("in.txt","w","utf-8")
for i in range(len(mas)):
    f.write(mas[i]+' ')

f.close()    
print('\n  Работа программы завершена успешно !')


