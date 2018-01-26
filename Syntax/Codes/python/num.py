import os
import re
import codecs

path = os.path.dirname(os.path.abspath(__file__))

repldict = {
    'с половиной':'0.5',
    'половина':'0.5',
    'четверть':'0.25',
    'с четвертью':'0.25',
    'треть':'1/3',
    'одиннадцать': '11',
    'одиннадцати':'11',
    'одиннадцатых':'11',
    'двенадцать': '12',
    'двенадцати':'12',
    'двенадцатых':'12',
    'тринадцать': '13',
    'тринадцати':'13',
    'тринадцатых':'13',
    'четырнадцать': '14',
    'четырнадцати':'14',
    'четырнадцатых':'14',
    'пятнадцать': '15',
    'пятнадцати':'15',
    'пятнадцатых':'15',
    'шестнадцать': '16',
    'шестнадцати':'16',
    'шестнадцатых':'16',
    'семнадцать': '17',
    'семнадцати':'17',
    'семнадцатых':'17',
    'восемнадцать': '18',
    'восемнадцати':'18',
    'восемнадцатых':'18',
    'девятнадцать': '19',
    'девятнадцати':'19',
    'девятнадцатых':'19',
    'двадцать': '20',
    'двадцати':'20',
    'двадцатых':'20',
    'тридцать': '30',
    'тридцати':'30',
    'тридцатых':'30',
    'сороковых':'40',
    'сорок': '40',
    'сорока':'40',
    'пятьдесят': '50',
    'пятидесяти':'50',
    'пятидесятых':'50',
    'шестидесяти':'60',
    'шестьдесят': '60',
    'шестидесятых':'60',
    'семьдесят': '70',
    'семидесяти':'70',
    'семидесятых':'70',
    'восемьдесят': '80',
    'восемидесяти':'80',
    'восемидесятых':'80',
    'девяносто': '90',
    'девяноста':'100',
    'девяностых':'90',
    'одна тысяча': '1000',
    'тысячи':'1000',
    'тысяча': '1000',
    'тысяче':'1000',
    'сто ': '100',
    'ста':'100',
    'один': '1',
    'первых':'1',
    'одна': '1',
    'двести': '200',
    'двухсотых':'200',
    'двухсот':'200',
    'двумстам':'200',
    'двумястами':'200',
    'триста':'300',
    'трехсотых':'300',
    'трёхсот':'300',
    'трёмстам':'300',
    'тремястами':'300',
    'четыреста':'400',
    'четырехсотым':'400',
    'четырёхсот':'400',
    'четырёмстам':'400',
    'четырьмястами':'400',
    'пятьсот':'500',
    'пятисот':'500',
    'пятистам':'500',
    'пятьюстами':'500',
    'шестьсот':'600',
    'шестисот':'600',
    'шестистам':'600',
    'шестьюстами':'600',
    'семьсот':'700',
    'семисот':'700',
    'семистам':'700',
    'семьюстами':'700',
    'восемьсот':'800',
    'восемисот':'800',
    'восемьюстами':'800',
    'девьтьсот':'900',
    'девятисот':'900',
    'девятьюстами':'900',
    'две': '2',
    'двум':'2',
    'двух':'2',
    'два': '2',
    'две':'2',
    'три': '3',
    'трем':'3',
    'четыре': '4',
    'четвёртых':'4',
    'пять': '5',
    'пяти':'5',
    'пятых':'5',
    'шесть': '6',
    'шестых':'6',
    'семь': '7',
    'седьмых':'7',
    'восемь': '8',
    'восьмых':'8',
    'девять': '9',
    'девятых':'9',
    'десять': '10',
    'нуль': '0',
    'ноль': '0',
    'миллион':'1000000',
    'миллиард':'1000000000',
}

repldict2={
    'сто ':'сто  ',
    ', ':' , ',
    ',':' , ',
    '.':' . ',
    'кв.':'квадратных',
}

words = []

def replfunc(match):
    return repldict[match.group(0)]

def replfunc2(match):
    return repldict2[match.group(0)]

def split_line(text):
    word = text.split()

    for current_word in word:
        words.append(current_word.lower())

def transform_1():
    sum = 0
    new_line = ''
    i=0
    already_was=0
   
    for i in range(len(words)):
        if (not words[i].isdigit()):
            new_line=new_line+' '+words[i]

        if (words[i].isdigit()):
            beforelast=words[i - 2]
            last=words[i - 1]
            next=words[i + 1]

            if (already_was==0):
                sum=sum+int(words[i])
            else:
                sum=sum+0
                already_was=0
                
            if (next=='1000'):
               sum=sum*1000
               already_was=1
           
            if (sum>0 and len(words[i])<=len(next)):
                new_line=new_line+' '+str(sum)
                sum=0
               
            if (sum >0 and not next.isdigit()):
                new_line=new_line+' '+str(sum)
                sum=0
           
           
    return new_line


def transform_2():
    new_line=''
    remember_about=''
    sum=0
    i=0
    for i in range(len(words)):
        if (words[i].isdigit()):
            last=words[i-1]
            next=words[i+1]
            beforelast=words[i - 2]

            if (next!='целых' and next!='сотых' and next!='десятых' and next!='тысячных'):
                new_line=new_line+' '+words[i]

            if (next.isdigit()):
                if (int(words[i])>int(next)):
                    sum=int(words[i])+int(next)
                    new_line=new_line.replace(words[i],'')
                    new_line=new_line+' '+str(sum)
                    remember_about=next
       
            if (words[i]==remember_about):
                new_line=new_line.replace(remember_about+' ','')

            if (last=='целых' and next=='сотых'):
                sum=int(beforelast)+int(words[i])*0.01
                new_line=new_line+' '+str(sum)

            if (last=='целых' and next=='десятых'):
                sum=int(beforelast)+int(words[i])*0.1
                new_line=new_line+' '+str(sum)

            if (last=='целых' and next=='тысячных'):
                sum=int(beforelast)+int(words[i])*0.001
                new_line=new_line+' '+str(sum)  
        
        else:
            if (words[i]!='сотых' and words[i]!='целых' and words[i]!='десятых' and words[i]!='тысячных'):
                new_line=new_line+' '+words[i]

    return new_line


def transform_3():
    new_line=''
    remember_about=''
    i=0
    sum=0
   
    if  "" in words[0]:
        new_line=words[0]
        del words[0]
        
    for i in range(len(words)):
        if (words[i-1].isdigit() and words[i].isdigit() and words[i]!='1000'):
            new_line=new_line+"/"+words[i]
            remember_about=words[i]

        if (words[i]=="0.5" or words[i]=="0.25"):
            last=words[i-1]
            new_line=new_line.replace(' '+last,'')

            if (words[i]=="0.5"):
                sum=int(last)+0.5
                new_line=new_line+' '+str(sum)
                remember_about="0.5"
                
            if (words[i]=="0.25"):
                sum=int(last)+0.25
                new_line=new_line+' '+str(sum)
                remember_about="0.25"

        if (words[i]!=remember_about):
            new_line=new_line+' '+words[i]

    return  new_line

os.chdir(path)
f=open("in.txt","r",encoding="utf-8") # открыли файл для чтения

#line="основание треугольника двадцати двух сторон треугольника равно сто  тысячи квадратов. "
#line="две диагонали равны соответcвенно три , три целых три десятых ."
#line="три основания треугольника равно сто  тридцать пять квадратных см . "
#line = "сто  три стороны треугольника равны семьдесят три целых четыре сотых , сто  двенадцать целых пятьдесят тысячных , сто  тридцать семь тысячи  сорок семь с половиной . "
#line = "три стороны треугольника равны тысяча девяносто четыре  целых сто три сотых . "

line=f.read()
print(line)
regex = re.compile('|'.join(re.escape(x) for x in repldict2)) #делаем замену слов из строки на значения из словаря
text = regex.sub(replfunc2, line)
print(text)

#for line in f: # разбили строку на токены и токены положили в массив
    #word=line.split('	')
    #words.append(line.split())

#print(words)
f.close()

regex = re.compile('|'.join(re.escape(x) for x in repldict)) #делаем замену слов из строки на значения из словаря
text2 = regex.sub(replfunc, text)
split_line(text2)

str1 = transform_1()
#print('first'+str1)
words.clear()
split_line(str1)


str2=transform_2()
#print('sec'+str2)
words.clear()
split_line(str2)

str3=transform_3()

f=codecs.open("in.txt","w","utf-8")
f.write(str3)
print(str3)
f.close()

# сто и тысяч с доп пробелом
#четыреста
