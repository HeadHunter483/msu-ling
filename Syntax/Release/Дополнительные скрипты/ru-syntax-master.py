import os
import fileinput
import io
import sys
import subprocess
import codecs
import re


path = os.path.dirname(os.path.abspath(__file__))
mas=[]
little_conll=[]
aver_conll=[]
conll=[]

repldict = {
'1-несобст-компл':'1-nesobst-kompl',
'2-несобст-компл':'2-nesobst-kompl',
'3-несобст-компл':'3-nesobst-kompl',
'4-несобст-компл':'4-nesobst-kompl',
'5-несобст-компл':'5-nesobst-kompl',
'неакт-компл':'neakt-kompl',
'инф-союзн':'inf-soyuzn',
'сравн-союзн':'sravn-soyuzn',
'сент-предик':'sent-predic',
'адр-присв':'adr-prisv',
'оп-опред':'op-opred',
'аппрокс-пор¤дк':'approks-poryad',
'релят':'relyat',
'композ':'kompoz',
'ном-аппоз':'nom-appoz',
'об-аппоз':'ob-appoz',
'аппрокс-колич':'approks-kolich',
'распред':'raspred',
'колич-огран':'kolich-ogran',
'аддит':'addit',
'длительн':'dlitel',
'дистанц':'distance',
'1-компл':'1-kompl', 
'2-компл':'2-kompl',
'3-компл':'3-kompl',
'4-компл':'4-kompl',
'5-компл':'5-kompl',
'суб-обст':'sub-obst',
'несобст-агент':'nesobst-agent',
'дат-субъект':'dat-subj',
'компл-аппоз':'kompl-appoz',
'соотнос':'sootnos',
'обст':'obst',
'пасс-анал':'pass-anal',
'нум-аппоз':'num-appoz',
'агент':'agent',
'опред':'opred',
'количест':'kolichest',
'электив':'electiv',
'атриб':'atrib',
'предл':'predl',
'аппрокс-колич':'approks-kolich',
'предик':'predic',
'огранич':'ogranich',
'вводн':'vvodn',
'изъясн':'izyasn',
'разъяснит':'razyasnit',
'примыкат':'primikat',
'уточн':'utochn',
'аналит':'analit',
'кратн':'kratn',
'соотнос':'sootnos',
'эксплет':'eksplet',
'пролепт':'prolept',
'вспом':'vspom',
'аппоз':'appoz',
'сочин':'sochin',
'соч-союзн':'soch-soyuzn',
'сент-соч':'sent-soch',
'квазиагент':'kvaziagent',
'подч-союзн':'podch-soyuzn',
'ROOT':'root',
}

def replfunc(match):
	return repldict[match.group(0)]

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))

def splitter():
    i=0
    j=0
    str1=''
    while (i<len(word)):
        if word[i]!='.' and word[i]!='?' and word[i]!='!':
            str1=str1+' '+word[i]     
        if word[i]=='.'or word[i]=='?' or word[i]=='!':
            str1=str1+' '+word[i]+' '
            mas.append(str1)
            str1=''
    
        i=i+1

    while (j<len(mas)):
        if "" in mas[j][0]:
            mas[j]=mas[j][:0]+mas[j][1:]
      
        j=j+1

        return len(mas)

def postanalyze(aver_conll,little_conll):
    punc=[]
    
    for i in range(len(little_conll)):
        little_conll[i]=little_conll[i].split('\t')

    ############## Выходные типы связей меняем на латинские эквиваленты ########
        
    for i in range(len(little_conll)-1):
        str4=little_conll[i][7]
        regex = re.compile('|'.join(re.escape(x) for x in repldict))
        little_conll[i][7]=regex.sub(replfunc,str4)

    ############################################################################

    for i in range(len(little_conll)-1):
        if "см" in little_conll[i][1] or "градус" in little_conll[i][1]:
                
                little_conll[i-1][1]=little_conll[i-1][1]+little_conll[i][1]
                little_conll[i-1][6]=little_conll[i][6]
                little_conll[i-1][7]=little_conll[i-1][7]
                little_conll[i][1]="del"
                
                j=i+1
                while(j<len(little_conll)-1):        
                        if (int(little_conll[j][6])>int(i)):
                            little_conll[j][6]=str(int(little_conll[j][6])-1)
                    
                        j=j+1

        if little_conll[i][1]=="/":
                little_conll[i][1]=little_conll[i-1][1]+little_conll[i][1]+little_conll[i+1][1]
                little_conll[i][3]="NUM"
                little_conll[i-1][1]="del"
                little_conll[i+1][1]="del"
                
                j=i+1
                while(j<len(little_conll)-1):        
                        if (int(little_conll[j][6])>int(i)):
                            little_conll[j][6]=str(int(little_conll[j][6])-1)
                    
                        j=j+1
                        
        if ("PUNC" in little_conll[i][7] and (little_conll[i][1]!="-" or little_conll[i][1]!="=")):
            punc.append(little_conll[i])
            
            j=i+1
            while(j<len(little_conll)-1):        
                if (int(little_conll[j][6])>int(i)):
                    little_conll[j][6]=str(int(little_conll[j][6])-1)
                    
                j=j+1

                     
        else:
            if little_conll[i][1]!="см" and little_conll[i][2]!="градус" and little_conll[i+1][1]!="/" and little_conll[i][1]!="del":
                    aver_conll.append(little_conll[i])

        if (str(little_conll[i]).strip()):
            continue
        
    aver_conll=aver_conll+punc
    return aver_conll


def maltwork():
    for i in range(len(mas)):
        f=codecs.open(path+"\\in.txt","w","utf-8")
        f.write(mas[i])
        f.close()

        subprocess_cmd('python ' + path + '\\ru-syntax.py in.txt')
        os.chdir(path + '/bin/Malt')
        subprocess_cmd('java -jar maltparser-1.8.1.jar -c PTM -i ' + path +'\\' + '\\tmp' + '\\' + '\\in_raw.conll -o out.txt -m parse')

        f=open(path+"\\bin\\Malt\\out.txt","r",encoding="utf8")
   
        for line in f:
            if line=="":
                continue
            little_conll.append(line)

        f.close()

        conll.append(postanalyze(aver_conll,little_conll))
        aver_conll.clear()
        little_conll.clear()


text = ''.join(open('my_malt_in.txt',encoding="utf-8").readlines())
word=text.split()
a=splitter()
maltwork()

big_conll=[]

fout=open(path+"\\syntax_malt.txt","w",encoding="utf8")

for i in range(a):
        for s in conll[i]:
            big_conll.append(s)


k=0
for i in range(len(big_conll)):
    if (k>int(big_conll[i][0])):
        if (big_conll[i][1]!="," and big_conll[i][1]!="-"):
            k=0
            fout.write('\n')
        
    fout.write(str(k+1)+'	'+big_conll[i][1]+'	'+big_conll[i][3]+'	'+big_conll[i][6]+'	'+big_conll[i][7]+'\n')
    k=k+1
fout.close()
