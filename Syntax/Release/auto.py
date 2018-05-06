import re
import codecs
import subprocess
import os
import sys
import time
from utilites.clean import cleaner
from utilites.converter import convert

path = os.path.dirname(os.path.abspath(__file__))

mas=[]

#convert("in.txt","utf8")
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


###############################################

print('\n    Синтаксический анализатор математических текстов ver 1.3\n')
print('Date: '+time.strftime("%d/%m/%Y/%H:%M:%S"))
cleaner()
splitter()

value=0
success=1

for i in range(len(mas)):
    if "Text" in mas[i]:
        f2=open("semantic.txt","a",encoding="utf-8")
        f2.write('Text\n')
        f2.close()
        continue
    
    for l in range(len(mas[i])):
        if "ё" in mas[i]:
            mas[i]=str(mas[i]).replace("ё","е")
              
    f=open(path+"\\tmp\\sentences.txt","w",encoding="utf-8")
    f.write(mas[i])
    f.close()

    ###------   Морфология ------###
    
    subprocess_cmd('python morph_all.py sentences.txt morph.txt')
    
    f=open(path+"\\tmp\\morph.txt","r",encoding="utf-8")

    for line in f:
        if "error" in line:
            success=0
            f=open("syntax.txt","w",encoding="utf-8")
            f.write('error\n')
            f.close()
            break

    ### ---------- Синтаксис ------------- ###
    if success==1:
        subprocess_cmd('python syntax_all.py morph.txt syntax.txt'+' '+str(i+1))
        f=open("syntax.txt","r",encoding="utf-8")
        for line in f:
             if "error" in line:
                success=0
                f=open("syntax.txt","w",encoding="utf-8")
                f.write('error\n')
                f.close()
                break
            
        #subprocess_cmd('python graph_m.py graph'+str(i+1)+'.txt '+str(i+1))

if success==1: 
    f2=open("semantic.txt","a",encoding="utf-8")
    f2.write('End\n')
    f2.close()
        
    ############################################################33

    os.chdir(path)   # переходим в родительскую директорию
    f=open(path+"\\tmp\\sentences.txt","w",encoding="utf-8")

    for i in range(len(mas)):
        f.write(mas[i]+' ')

    f.close()

    ##################### Semantic #########################################

    conll=[]
    f=open("semantic.txt","r",encoding="utf-8")

    for line in f:
        row=line.split('	')
        conll.append(row)

    f.close()

    for i in range(len(conll)-1):
        if conll[i]==['\n'] or conll[i]==['\r\n'] or conll[i]==['Text\n'] or conll[i]==['End\n']:
            continue
                    
        if conll[i][0]==conll[i][5]:
            success=0
            print('\nОшибка в синтаксическом дереве \n')
            print(conll[i-1])
            print(conll[i])
            print(conll[i+1])
            print('\n')
            break
        
       

if success==1:
    f=open("semantic.txt","w",encoding="utf-8")

    ######## Считаем кол-во задач #######

    count_task=0
    count_sent=0
    start=0 
    finish=0

    for i in range(len(conll)):
    #### смотрим на количество указателей --> столько задач ###
        if conll[i]==['Text\n']: 
            count_task=count_task+1
        
    f.write(str(count_task)+'\n')### пищем кол-во задач в файл

    for i in range(len(conll)):
        if conll[i]==['\n'] or conll[i]==['\r\n'] : ## пропуск хода
            continue
        
        if i!=0 and (conll[i]==['Text\n'] or conll[i]==['End\n']): ### ищем в таблице соответствующие слова ##
            finish=i
            for k in range(start,finish):
                if conll[k]==['\n'] or conll[k]==['\r\n'] or conll[k]==['Text\n']:
                    continue
        
                if conll[k][3]=="SENT": ## посчитали кол-во предложений в данном блоке
                    count_sent=count_sent+1

            f.write(str(count_sent)+'\n') ## записали
        
            for m in range(start,finish):
                if conll[m]==['\n'] or conll[m]==['\r\n'] or conll[m]==['Text\n']:
                    continue
            
                if conll[m][3]=="SENT": ### записываем кол-во токенов
                    f.write(str(conll[m][0])+'\n')
                
            for l in range(start,finish):
                if conll[l]==['\n'] or conll[l]==['\r\n']:
                    f.write('\n')
                    continue
                
                if conll[l]==['Text\n']:
                    continue

                ### пишем предложения ###
                f.write(conll[l][0]+'\t'+conll[l][1]+'\t'+conll[l][2]+'\t'+conll[l][3]+'\t'+conll[l][4]+'\t'+conll[l][5]+'\t'+conll[l][6])

            if conll[i]==['End\n']: ## Закончили ! ##
                break

            start=finish 
            count_sent=0
    
    f.close()

    ##################################################################################

if success==1:
    print('\n  Работа программы завершена успешно !')
if success==0:
    print('\n Работа программы завершена неудачно ! ')
    


