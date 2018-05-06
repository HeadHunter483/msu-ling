import pymorphy2
import os
import re
import sys
import codecs
import string

path = os.path.dirname(os.path.abspath(__file__))

repldict = {
'NOUN':'S',#сущ
'ADJF':'A',#прил полн
'ADJS':'A',#прил крат
'VERB':'V',#глагол личная форма
'INFN':'V',#глагол инф
'PRTF':'PARTCP',#полное причастие
'PRTS':'PARTCP',#краткое причастие
'NUMR':'NUM',#числительное
'NUMB':'NUM',#3.2
'PREP':'PR',#предлог
'NPRO':'SPRO',#местоимение
'CONJ':'CONJ',#союз
'COMP':'ADV',#компаратив
'ADVB':'ADV',#наречие
'PRED':'ADV',#предикатив
'INTJ':'INTJ',#междометие
'GRND':'V',#деепричастие
'PNCT':'SENT',#знаки препинания
'PRCL':'PART',#частица
}

repldict2={
',':' ',
'NOUN':'S',
'NUMR':'num',
'ADJF':'plen',#полное
'PRTF':'plen',
'ADJS':'brev',#крат
'PRTS':'brev',
'INFN':'inf',
'GRND':'ger',#деепричастие
'Apro':'apro',#местоимение-прилагательное
'VERB':'-',
'Anph':'-',#анафорическое

#род
'masc':'m',#мужской
'femn':'f',#женский
'neut':'n',#средний

#падежи
'nomn':'nom',#именительный
'gent':'gen',#родительный
'datv':'dat',#дательный
'accs':'acc',#винительный
'ablt':'ins',#творительный
'loct':'loc',#предложный
'gen2':'gen',
'acc2':'acc',
'loc2':'loc',

'Subx':'-',#возможна субстантивация
'NPRO':'-',
'Qual':'-',#качественное
'Poss':'-',#притяжательное
'tran':'-',#переходный
'intr':'-',#непереходный
'Impe':'-',#безличный
'PRED':'-',
'Anum':'-',#порядковое числит
'ADVB':'-',
'COMP':'-',

#одушевленность
'inan':'inan', #неодушевл
'anim':'anim',#одушевлен

#число
'sing':'sg',#ед.ч.
'plur':'pl',#мн.ч.
'Sgtm':'sg',#только единственное число

#залог
'actv':'act',#действительный
'pssv':'pass',#страдательный

#лицо
'1per':'1p',
'2per':'2p',
'3per':'3p',

#время
'pres':'praes',#настоящее
'past':'praet', #прошедшее
'futr':'fut', #будущее

#наклонение
'indc':'indic',
'impr':'imper',
'imper2':'imper',

#вид
'perf':'pf',#cов
'impf':'ipf',#несов
}


words=[]
conll=[]
problems_word=[]
measure=[]

def isnumberwithlatin(s):
    return s.translate(string.punctuation).isalnum()

def replfunc(match):
    return repldict[match.group(0)]

def replfunc2(match):
    return repldict2[match.group(0)]

def replfunc3(match):
    return repldict3[match.group(0)]


def give_index(word):
    index=0
        
    if (word=="равно"):
        index=2

    if (word=="стороной"):
        index=1

    if (word=="боковыми"):
        index=1
        
    if (word=="вокруг"):
        index=1

    if (word=="касательная" or word=="секущая"):
        index=1

    if (word=="окружности"):
        index=1

    if (word=="заключенная" or word=="заключенный" or word=="заключенными"
        or word=="прилежащими" or word=="прилежащий" or word=="прилежащая"):
        index=1

    if (word=="нее"):
        index=2

    
    
    return index


def s_morph(string):
    str5=""
    mas=[]
    mas2=[]
    word = string.split()

    for current_word in word:
        mas.append(current_word.lower())

    while(len(mas2)!=4):
        mas2.append("-")

    for s in mas:
        if (s=='f' or s=='m' or s=='n'): #род
            mas2[0]=s
        if (s=='inan' or s=='anim'): #одуш
            mas2[1]=s
        if (s=='nom' or s=='gen' or s=='dat' or s=='acc' or s=='ins' or s=='loc'):
            mas2[2]=s
        if (s=='sg' or s=='pl'): #число
            mas2[3]=s
        
    i=0
    for i in range(len(mas2)):
        str5=str5+' '+mas2[i]
        
    return str5  
  
    
def v_morph(string):
    str5=""
    word = string.split()
    mas=[]
    mas2=[]

    for current_word in word:
        mas.append(current_word.lower())

    while(len(mas2)!=6):
        mas2.append("-")

    for s in mas:
        if (s=='pf' or s=='ipf'): #вид
            mas2[0]=s
        if (s=='praes' or s=='praet' or s=='fut'): #время
            mas2[1]=s
        if (s=='sg' or s=='pl'): #число
            mas2[2]=s
        if (s=='indic' or s=='inf' or s=='imper'): #наклонение
            mas2[3]=s
        if (s=='1p' or s=='2p' or s=='3p'): #лицо
            mas2[5]=s

    i=0
    for i in range(len(mas2)):
        str5=str5+' '+mas2[i]
        
    return str5


def partcp_morph(string):
    str5=""
    word = string.split()
    mas=[]
    mas2=[]

    for current_word in word:
        mas.append(current_word.lower())

    while(len(mas2)!=8):
        mas2.append("-")

    for s in mas:
        if (s=='pf' or s=='ipf'): #вид
            mas2[0]=s
        if (s=='praes' or s=='praet' or s=='fut'): #время
            mas2[1]=s
        if (s=='nom' or s=='gen' or s=='dat' or s=='acc' or s=='ins' or s=='loc'):
            mas2[2]=s
        if (s=='sg' or s=='pl'): #число
            mas2[3]=s
        if (s=='plen' or s=='brev'): #полное или краткое
            mas2[4]=s
        if (s=='f' or s=='m' or s=='n'): #род
            mas2[5]=s
        if (s=='act' or s=='pass'):
            mas2[6]=s

    for i in range(len(mas2)):
        str5=str5+' '+mas2[i]
        
    return str5

def adj_morph(string):
    str5=""
    word = string.split()
    mas=[]
    mas2=[]

    for current_word in word:
        mas.append(current_word.lower())

    while(len(mas2)!=6):
        mas2.append("-")

    for s in mas:
        if (s=='nom' or s=='gen' or s=='dat' or s=='acc' or s=='ins' or s=='loc'):
            mas2[1]=s
        if (s=='sg' or s=='pl'):
            mas2[2]=s
        if (s=='plen' or s=='brev'):
            mas2[3]=s
        if (s=='f' or s=='m' or s=='n'):
            mas2[4]=s
        if (s=='inan' or s=='anim'):
            mas2[5]=s

    for i in range(len(mas2)):
        str5=str5+' '+mas2[i]
        
    return str5    
    
def spro_morph(string):
    str5=""
    word = string.split()
    mas=[]
    mas2=[]

    for current_word in word:
        mas.append(current_word.lower())

    while(len(mas2)!=4):
        mas2.append("-")

    for s in mas:
        if (s=='sg' or s=='pl'):
            mas2[0]=s
        if (s=='1p' or s=='2p' or s=='3p'):
            mas2[1]=s
        if (s=='f' or s=='m' or s=='n'):
            mas2[2]=s
        if (s=='nom' or s=='gen' or s=='dat' or s=='acc' or s=='ins' or s=='loc'):
            mas2[3]=s

    for i in range(len(mas2)):
        str5=str5+' '+mas2[i]
        

    return str5    

problems_word=[]
measure=[]
f=open(path+"\\..\\tmp\\morph_iskl.txt","r",encoding="utf-8")

for line in f:
    row=line.split('	')
    problems_word.append(row)

f.close()

f=open(path+"\\..\\tmp\\measure.txt","r",encoding="utf-8")

for line in f:
    row=line.split('  ')
    measure.append(row)

f.close()

mrph=pymorphy2.MorphAnalyzer()

def morph(line, pr=False):
        if pr:
            print('parse sentence '+line)

        words=[]
        conll=[]

        def split_line(text):
            word = text.split()

            for current_word in word:
                words.append(current_word.lower())

        
        split_line(line)
        str1=''
        index=0
        flag=0

        for i in range(len(words)):
            for j in range(len(problems_word)):
                if (words[i]==problems_word[j][0]):
                    p=mrph.parse(words[i])[int(problems_word[j][1])]
                    break
                p=mrph.parse(words[i])[0]
       
            if (words[i]!='.' or words[i]!=',' or words[i]!='?' or words[i]!='!'):
                conll.append([str(i+1),str(words[i]),str(p.normal_form),str(p.tag.POS),str(p.tag),str(p.normalized.tag),'_','_','_','_'])

        
        str1=""
        str2=""
        flag6=0

        for i in range(len(conll)):
            flag6=0
            if 'LATN' in conll[i][4]:
                conll[i][3]="NONLEX"
                conll[i][4]="NONLEX"
                conll[i][5]="NONLEX"
            
            if 'PNCT' in conll[i][4]:
                if ',' or '-' in conll[i][1]:
                    conll[i][3]=conll[i][1]
                    conll[i][4]=conll[i][1]
                    conll[i][5]="PUNC"
                    flag6=1
                     
                if conll[i][1]=='.' or conll[i][1]=='!' or conll[i][1]=='?':
                    conll[i][3]="SENT"
                    conll[i][4]="SENT"
                    conll[i][5]="SENT"

            if 'UNKN' in conll[i][4]:
                conll[i][3]=conll[i][1]
                conll[i][4]=conll[i][1]
                conll[i][5]="PUNC"
                flag6=1

            str2=conll[i][3]
            regex = re.compile('|'.join(re.escape(x) for x in repldict))
            conll[i][3] = regex.sub(replfunc, str2)
    
            str3=conll[i][4]
            regex = re.compile('|'.join(re.escape(x) for x in repldict2)) 
            conll[i][4] = regex.sub(replfunc2, str3)


            if conll[i][1]=="меньше" or conll[i][1]=="больше":
                conll[i][3]="A"
                conll[i][4]="A - - sg brev - -"
        

            if conll[i][2].isdigit():
                conll[i][1]=conll[i][2]
                conll[i][3]="NUM"
                conll[i][4]=" - - -"
        
    
            if conll[i][3]=="S":
                conll[i][4]=s_morph(conll[i][4])


            for j in range(len(measure)):
                if conll[i][1]==measure[j][0]:
                    if pr:
                        print(conll[i][1])
                        print(measure[j][0])
		    
                    conll[i][3]='MEAS'
                    conll[i][4]=' '+str(measure[j][1])
                    break
           
            if conll[i][3]=="V":
                conll[i][4]=v_morph(conll[i][4])
            if conll[i][3]=="PARTCP":
                conll[i][4]=partcp_morph(conll[i][4])
            if conll[i][3]=="A":
                conll[i][4]=adj_morph(conll[i][4])
            if conll[i][3]=="SPRO":
                conll[i][4]=spro_morph(conll[i][4])
        
            if conll[i][3]=="NUM":
                conll[i][4]=" - - -"

            if "NUMB" in conll[i][4]:
                conll[i][3]="NUM"
                conll[i][4]=" - - -"

            if conll[i][3]=="ADV":
                conll[i][4]=" -"
    
            if conll[i][1].isdigit() or "/" in conll[i][1] or "sqrt" in conll[i][1]:
                conll[i][3]="NUM"
                conll[i][4]="NUM - - -"
     
            if conll[i][3]=="CONJ" or conll[i][3]=="PR" or conll[i][3]=="PART":
                conll[i][4]=conll[i][3]
                conll[i][5]=conll[i][3]
                flag6=1
    
            if (not conll[i][3] in conll[i][4]):
                conll[i][4]=conll[i][3]+''+conll[i][4]

            if (re.search('[a-zA-Z]',conll[i][1])):
                if re.search(r'\d+',conll[i][1]):
                    conll[i][3]="NONLEX"
                    conll[i][4]="NONLEX"
                    conll[i][5]="NONLEX"

            if ":" in conll[i][1] and re.search(r'\d+',conll[i][1]):
                conll[i][3]="NUM"
                conll[i][4]="NUM - - -"

            if flag6!=1:
                conll[i][5]=conll[i][4]

            flag6=0
            

########################################################
        result=[[str(i+1)]+conll[i][1:10] for i in range(len(conll))]
        return result




