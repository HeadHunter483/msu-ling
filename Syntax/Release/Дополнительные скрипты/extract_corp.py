import xml.etree.cElementTree as ET
import os
import re
import subprocess
import sys

path = os.path.dirname(os.path.abspath(__file__))

repldict = {
'deepr':'ger',
'sred':'n',
'jen':'f',
'muj':'m',
'nepros':'inpraes',
'nast':'praes',
'pros':'praet',
'_root':'root',
'im':'nom',
'rod':'gen',
'dat':'dat',
'vin':'acc',
'tvor':'ins',
'pr':'pr',
'ed':'sg',
'mn':'pl',
'inf':'inf',
'iz_av':'indic',
'pov':'imper',
'kr':'brev',
'poln':'plen',
'pritaj':'poss',
'prev':'supr',
'srav':'comp',
'nesov':'ipf',
'sov':'pf',
'od':'anim',
'neod':'inan',
' .':'. ',
' ,':', ',
' ?':'? ',
' !':'! ',
' -':' - ',
'3-l':'3p',
'2-l':'2p',
'1-l':'1p',
}

repldict2 = {
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
'обст ':'obst',
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
'разъ¤снит':'razyasnit',
'примыкат':'primikat',
'уточн':'utochn',
'аналит':'analit',
'кратн':'kratn',
'соотнос':'sootnos',
'эксплет':'eksplet',
'пролепт':'prolept',
'вспом ':'vspom',
'аппоз':'appoz',
'сочин':'sochin',
'сравнит':'sravnit',
'соч-союзн':'soch-soyuzn',
'сент-соч':'sent-soch',
'квазиагент':'kvaziagent',
'подч-союзн':'podch-soyuzn',
}

def replfunc(match):
	return repldict[match.group(0)]
	
def replfunc2(match):
	return repldict2[match.group(0)]

	
symbols = (u"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ",
           u"abvgdeejzijklmnoprstufhzcss_y_euaABVGDEEJZIJKLMNOPRSTUFHZCSS_Y_EUA")

length=0
s=" "
line=""

new_text=[]
error_list=[]
new_conll=[]
aver_error_list=[]
new_error_list=[]
new_conll_list=[]
text=[]
conll=[]



                                
def extr(mode,file_name,fout1):
    os.chdir(path+"\\Texts")
    tr = {ord(a):ord(b) for a, b in zip(*symbols)}
    print(file_name)

    tree = ET.ElementTree(file=file_name)
    tree.getroot()

    words=[] # храним массив слов
    punctat=[] #какой именно знак пунктуации
    index=[] #к чему отнесем знак пунктуации
    translated = []
    now=0
    last=0
    flag=0
    flag3=0
    punc=''

    try:
        os.remove(path+"\\Texts\\test.txt")
    except FileNotFoundError:
        0

    for node in tree.iter('W'):
        id = node.attrib.get('ID') #вытаскиваем из xml id
        now=int(id) #переводим id в целое число
        word=str(node.text) #слово
        dom=node.attrib.get('DOM') #родитель
        lemma=node.attrib.get('LEMMA') #начальная форма
        link = node.attrib.get('LINK') #тип связи
        feat=node.attrib.get('FEAT') #морфология
        parts_of_speach=feat.split() #из морфологии "вытаскиваем" часть речи


        if(now<last): #ставим переход на новую строку, если следующий токен не больше предыдущего
            flag=1
            words.append('.')
            words.append('\n')


        if (link==None): #если связи нет
            link=' '


        if(flag==1): #дописываем к разобранным данные в конце пунктуацию
            for k in range(len(punctat)):
                if(mode=="corp"):
                    fout1.write(str(last+k+1)+'	'+punctat[k]+'	'+punctat[k]+'	'+'SENT'+'	'+'SENT'+'	'+'_'+'	'+index[k]+'	'+'punc'+'\n')
                else:
                    fout1.write(str(last+k+1)+'	'+punctat[k]+'	'+'SENT'+'	'+index[k]+'	'+'punc'+'\n')

            punctat.clear() #чистим массив пунктуаций
            index.clear() # чистим массив индексов
            fout1.write('\n')
            flag=0

        if(lemma=='ОН' or lemma=='ОНО' or lemma=='ОНИ' or lemma=='ОНА' or lemma=='Я' or lemma=='ТЫ' or lemma=='ВЫ' or lemma=='МЫ' or lemma=='СЕБЯ'):
            parts_of_speach[0]='SPRO' #слова,имеющие соответствующие начальные формы воспринимаем как местоимения

        if 'ПРИЧ' in feat:
            parts_of_speach[0]='PARTCP'
            feat=feat.replace('ПРИЧ','')

        if (dom=='_root'): #если родитель указан не числом,а словом
            link='root'
            dom='0'

        if (node.text!=None): #если слово опущено,но оно подразумевается пишем его в скобказ
            words.append(node.text)
            words.append(' ')
        else:
            words.append('')
            word='('+lemma.lower()+')'
	
	##### работа с пунктуацией #####
        if (',' in node.tail):
            words.append(',')
            punctat.append(',')
            index.append(str(now))

        if ('.' in node.tail):
            punctat.append('.')
            index.append(str(now))

        if ('-' in node.tail):
            words.append('-')
            punctat.append('-')
            index.append(str(now))

        if ('?' in node.tail):
            words.append('?')
            punctat.append('?')
            index.append(str(now))

        if ('!' in node.tail):
            words.append('!')
            punctat.append('!')
            index.append(str(now))
		
	##################################

        feat=feat.translate(tr) #морфологию переделываем на транслит


        if mode=='test':
            if(punc!=',' and punc!='.' and punc!='?' and punc!='-' and punc!='!'):
                fout1.write(str(now).lower()+'	'+word.lower()+'	'+parts_of_speach[0]+'	'+dom.lower()+'	'+link.lower()+'\n')

        if mode=='corp':
            if(punc!=',' and punc!='.' and punc!='?' and punc!='-' and punc!='!'):
                fout1.write(str(now).lower()+'	'+word.lower()+'	'+lemma.lower()+'	'+parts_of_speach[0]+'	'+feat.lower()+'	'+feat.lower()+'	'+dom.lower()+'	'+link.lower()+'\n')

        last=now
        punc=''

     


def post_extr():  
        regex = re.compile('|'.join(re.escape(x) for x in repldict))
        with open(path+'\\Texts\\text1.txt',encoding="cp1251") as fin, open(path+'\\Texts\\final3.txt','w',encoding="utf8") as fout:
                for line in fin:
                        fout.write(regex.sub(replfunc,line))
		
        regex2 = re.compile('|'.join(re.escape(y) for y in repldict2))
        with open(path+'\\Texts\\final3.txt',encoding="utf8") as fin, open(path+'\\Texts\\final5.txt','w',encoding="utf8") as fout:
                for line in fin:
                        fout.write(regex2.sub(replfunc2,line))
       
        os.remove(path+'\\Texts\\final3.txt')		
        os.remove(path+'\\Texts\\text1.txt')
        os.rename(path+'\\Texts\\final5.txt','text1.txt')


        f=open(path+"\\Texts\\text1.txt", 'r',encoding='utf8')

        conll=[]
        for line in f:
                line=line.split()
                conll.append(line)

        f.close()

        ###########################################

       
        count=1
        start=0
        flag_act=0

        punc=0
        for i in range(len(conll)):
                if conll[i]==[]:
                       continue
        
                if (count==1):
                        start=i

                if conll[i][1]==",":
                        punc=punc+1

                if (conll[i][1]=="!" or "-" in conll[i][1]):
                        conll[start][1]="error"

                if (punc>1):
                        conll[start][1]="error"
                        
                if conll[i][1]!="." and conll[i][1]!="?" and conll[i][1]!="!":
                        count=count+1
                else:
                        if (count>10 or count < 4):
                                conll[start][1]="error"
                                count=1
                                continue
                        else:
                                if conll[start][1]!="error":
                                        count=1
                                        conll[start][1]="good"

       
               

        f=open(path+"\\Texts\\spisok.txt", 'a',encoding='utf8')
        
        for i in range(len(conll)):
                  if conll[i]==[]:
                          continue

                  f.write(conll[i][1]+' ')

                  if conll[i][1]=="." or conll[i][1]=="!" or conll[i][1]=="?" :
                          f.write('\n')

                  
        f.close()
        

        
        ############################################################################################
                        
        
                        
        f=open(path+"\\Texts\\corp.txt", 'a',encoding='utf8')      

        for i in range(len(conll)):
                        if conll[i]==[]:
                          continue

                        #if conll[i][1]=="error":
                                #while(conll[i][3]!="SENT"):
                                        #continue

                        f.write(str(conll[i][0])+'\t'+str(conll[i][1])+'\t'+str(conll[i][2])+'\t'+str(conll[i][3])+'\t'+str(conll[i][4])+'\n')

                        if conll[i][1]=="." or conll[i][1]=="?" or conll[i][1]=="!" :
                                f.write('\n')
                

        f.close()


def zapusk(file_name):
        fout1= open(path+"\\Texts\\text1.txt", 'w')
        
        extr("test",file_name,fout1)

        fout1.close()
        

        post_extr()

        os.chdir(path+"\\Texts\\")	
        os.chdir(path)
