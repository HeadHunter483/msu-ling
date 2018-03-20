import re
import codecs
import subprocess
import os
import sys
import time

path = os.path.dirname(os.path.abspath(__file__))

mas=[]
options=[]

text = ''.join(open('sentences.txt',encoding="utf-8").readlines())
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


#### Опция
f=open("config.txt","r",encoding="utf8")
i=0
for line in f:
        line=line.rstrip()
        row=line.split('	')
        options.append(row)
        i=i+1
        if (i>2): break
f.close()

test=0
graph=0
show=0

for i in range(len(options)):
    if options[i][0]=="graph" and options[i][1]=="1":
        graph=1
    if (graph==0 and options[i][0]=="test" and options[i][1]=="1"):
        test=1
        graph=0
    if options[i][0]=="show" and options[i][1]=="1":
        show=1

###############################################

if (show==1):
    print('\n    Синтаксический анализатор математических текстов ver 1.2\n')
    print('Date: '+time.strftime("%d/%m/%Y/%H:%M:%S"))
    print('\n  Этап I. Удаление результатов прошлого запуска\n ')

if (test==0):
    subprocess_cmd('python clean.py auto '+str(show))
    
if (test==1):
    subprocess_cmd('python clean.py test '+str(show))
    
splitter()

for i in range(len(mas)):
    if (show==1):
        print('\nВ данный момент разбирается следующее предложение: '+mas[i])

    os.chdir(path)
    f=codecs.open("sentences.txt","w","utf-8")
    f.write(mas[i])
    f.close()
    
    if (show==1):
        print('\n  Этап II. Преобразование входного файла к нужной кодировке:\n ')

    subprocess_cmd('python converter.py sentences.txt utf8')

    if (show==1):
        print('\n  Этап III. Преобразование чисел написанных прописью в числовой формат:\n ')

    #subprocess_cmd('python num.py')

    if (show==1):
        print('\n  Этап IV. Морфологический анализ:\n ')
        
    subprocess_cmd('python morph.py sentences.txt morph.txt')
    #subprocess_cmd('pause ')
    #os.chdir(path+'\\tmp')
    #subprocess_cmd('python morph_corrector.py ')
    #os.chdir(path)

    if (show==1):
        print('\n  Этап V. Этап работы с неделимыми выражениями:\n ')

    subprocess_cmd('python skleika.py ')
    
    if (show==1):
        print('\n  Этап VI. Синтаксический анализ :\n ')

    subprocess_cmd('python syntax_head.py morph.txt syntax.txt')

    if (show==1 and graph==1):
        print('\n  Этап VII. Визуализация синтаксического графа:\n ')

    if (graph==1):
        subprocess_cmd('python graph.py tree'+str(i+1))
        

os.chdir(path)   # переходим в родительскую директорию
i=0
f=codecs.open("sentences.txt","w","utf-8")
for i in range(len(mas)):
    f.write(mas[i]+' ')

f.close()

if (test==1):
    subprocess_cmd('python tester.py ')

print('\n  Работа программы завершена успешно !')


