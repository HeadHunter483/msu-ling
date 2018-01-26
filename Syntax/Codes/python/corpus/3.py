import xml.etree.cElementTree as ET
import os
import re
import subprocess
import sys

# На вход подается xml-файл синтагруса.Затем данные оттуда преобразуются к виду таблицы.
# После этого столбец с морфологией меняется на эквивалентный написанный на транслите
# Морфология преобразуется к нужному нам формату
# Вытаскивается часть речи
# Данные пишутся в 2 файла - таблицу и отдельно предложения
# Данные таблицы разбиваются на маленькие файлы
# Конвертация в utf8 не обязательно


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
'разъ¤снит':'razyasnit',
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
}

def replfunc(match):
	return repldict[match.group(0)]
	
def replfunc2(match):
	return repldict2[match.group(0)]

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))
	
symbols = (u"абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ",
           u"abvgdeejzijklmnoprstufhzcss_y_euaABVGDEEJZIJKLMNOPRSTUFHZCSS_Y_EUA")

tr = {ord(a):ord(b) for a, b in zip(*symbols)}

if __name__ == '__main__':
	mode = sys.argv[1]
	file_name=sys.argv[2]

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

os.remove('test.txt')
fout1= open('text1.txt', 'w')
fout2= open('text2.txt', 'w')

###################
punc=''

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
	
	k=0
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
	else :
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
			#fout1.write(word.lower()+'	'+feat.lower()+'	'+dom.lower()+'	'+link.lower()+'\n')
		
	last=now
	punc=''
	
fout1.write('\n')	
fout1.write('#######'+'\n')	
	
for i in range(len(words)):	
	fout2.write(str(words[i]))
	
fout1.close()
fout2.close()



##############   Делаем необходимые замены слов #############

if mode=='test' or mode=='corp':
	regex = re.compile('|'.join(re.escape(x) for x in repldict))
	with open('text1.txt',encoding="cp1251") as fin, open('final3.txt','w',encoding="utf8") as fout:
		for line in fin:
			fout.write(regex.sub(replfunc,line))
		
	regex2 = re.compile('|'.join(re.escape(y) for y in repldict2))
	with open('final3.txt',encoding="utf8") as fin, open('final5.txt','w',encoding="utf8") as fout:
		for line in fin:
			fout.write(regex2.sub(replfunc2,line))
       
	os.remove('final3.txt')		
	os.remove('text1.txt')
	os.rename('final5.txt','text1.txt')

	regex = re.compile('|'.join(re.escape(x) for x in repldict))
	with open('text2.txt',encoding="cp1251") as fin, open('final4.txt','w',encoding="utf8") as fout:
		for line in fin:
			fout.write(regex.sub(replfunc,line))
	
	os.remove('text2.txt')
	os.rename('final4.txt','test.txt')

#####################################################################

#########  Разбиваем один файл на несколько тестовых ###############

a='.'
b='?'
c='!'
i=0
p1=0
p2=0
p3=0

with open("text1.txt", "r") as f:
    buff = []
    i = 1
    for line in f:
        if line.strip(): 
           buff.append(line)
        if line.strip() == '':
           output = open('test_%d.txt' % i,'w')
           output.write(''.join(buff))		   
           output.close()
           file=open('test_%d.txt' % i,'r')
           text=file.read()
		   
           if not '!' in text:
              p1=1
           else:
              p2=0
              p3=0
			          
           if not '?' in text:
               p2=1
           else:
               p1=0
               p3=0
                          
           if not '.' in text:
               p3=1
           else:
               p1=0
               p2=0      
           
           if (p1==1 and p2==1 and p3==1):
              file.close()
              file2=open('test_%d.txt' % i,'r')
              l=file2.read()
              count=l.splitlines()
              file2.close()
              file2=open('test_%d.txt' % i,'a')
              file2.write(str(len(count)+1)+'	'+a+'	'+a+'	'+'SENT'+'	'+'SENT'+'	'+'_'+'	'+str(len(count))+'	'+'punc'+'\n')
              #file2.write(a+'	'+'SENT'+'	'+str(len(count))+'	'+'punc'+'\n')
              file2.close()
              p1=0
              p2=0
              p3=0
			  
           i+=1
           buff = []			  
 

################################################
		   
#for k in range(i):
	#subprocess_cmd('python converter.py test_corpus_'+str(k+1)+'.txt utf8')
	
if mode=='corp':
	subprocess_cmd('python 4.py')
	k=1
	while(k<i):
		os.remove('test_'+str(k)+'.txt')
		k=k+1
	
