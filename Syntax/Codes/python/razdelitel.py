import re

num_words=[]
words=[] # храним массив слов
remember='a'


dict=['проведенная','проведенных','которого','образованного','который','проведена','вписанной','построенная','содержащий','проведенную']
dict2=['служит','равны']


def insertChar(mystring, position, chartoinsert ):
    longi = len(mystring)
    mystring   =  mystring[:position] + chartoinsert + mystring[position:] 
    return mystring   


def split_line(text):
    #text=text.replace(',', ' , ')
    words = text.split()
	
    for current_word in words:
        num_words.append(current_word.lower())
        num_words.append(" ")
		
def return_index(str):
	i=0
	value=0
	for i in range(len(num_words)):
		if "если" in num_words[i]:
			value=i
			break;
		i=i+1
	return value	
	
	
def insert_in_position(text,str,opred):
	split_line(text)
	i=return_index(str)
	
	j=0
	length=0;
	while (j<=i):
		length=length+len(num_words[i])
		print(length)
		j=j+1
	
	print(length)
	text=insertChar(text,length+3,opred)
	print(text)
	
words_re = re.compile("|".join(dict))
words_re2 = re.compile("|".join(dict2))		


main_list=[]

flag1=0

def russianTokenizer(text):
	#insert_in_position(text,"енных","opred");
	#insert_in_position(text,"еcли","opred");
	words = text.split(',')
	#remember=""
	#remember_1=""
	
	for st in words:
		num_words.append(st)
		#if "если" in st:
			#remember=st;
			
	j=0
	mem=0
	h=0
	
	num_words.append(' ')
	num_words.append(' ')
	num_words.append(' ')
	num_words.append(' ')
	num_words.append(' ')
	num_words.append(' ')
	num_words.append(' ')
	num_words.append(' ')
	#num_words.append(' ')
	#num_words.append(' ')
	
	flag1=0
	while(j<len(num_words)):
	
		#print(num_words[0].count(' '))
	
		if len(num_words[j])==1:
			num_words[j]=num_words[j-1]+', '+num_words[j]
			num_words.pop(j-1)
				
		if "Дан квадрат" in num_words[j]:
			num_words[j]=num_words[j].replace("Дан квадрат","")
			flag1=1
			
		if "Дан треугольник" in num_words[j]:
			num_words[j]=num_words[j].replace("Дан треугольник","")
			flag1=2

		if "Дана окружность" in num_words[j]:
			num_words[j]=num_words[j].replace("Дана окружность","")
			flag1=3
			
		if flag1==1 and "которого" in num_words[j]:
			num_words[j]=num_words[j].replace("которого","квадрата")
			
		if flag1==2 and "которого" in num_words[j]:
			num_words[j]=num_words[j].replace("которого","треугольника")

		if flag1==3 and "которой" in num_words[j]:
			num_words[j]=num_words[j].replace("которой","окружности")
			
		if "если" in num_words[j]:
			num_words[j]=num_words[j].replace("если","condition (если)")
			
		if words_re.search(num_words[j]): #опора на ключевые слова
			num_words[j]='opred '+num_words[j]
			
		if "Докажите" in num_words[j]:
			num_words[j]=num_words[j]+', '+num_words[j+1]
			num_words.pop(j+1)
		
		if "в отношении" in num_words[j]:
			num_words[j-1]=num_words[j-1]+' '+num_words[j]
			num_words.pop(j)
		
		if "считая от" in num_words[j]:
			num_words[j-1]=num_words[j-1]+', '+num_words[j]
			num_words.pop(j)
			
		
		if  "opred"	in num_words[j]:
			if "вписанной" in num_words[j]:
				num_words[j]=num_words[j].replace("вписанной","(окружности) вписанной")
				
			num_words[j-1]=num_words[j-1]+' '+num_words[j+1]
			num_words.pop(j+1)
			
		if " а " in num_words[j]:
			#print('here')
			num_words[j]=num_words[j].replace(" а ","conj (a) ")
		
		if "-" in num_words[j]:
			match=words_re2.search(num_words[j-1])
			num_words[j]=num_words[j].replace('-',match.group(0))

		if "параллельная основанию" in num_words[j]:
			num_words[0]=num_words[0]+' параллельная основанию '
			num_words.pop(j)
			
		j=j+1
	
	
	j=0
	
	while(j<len(num_words)):
		num_words[j]=num_words[j]+'.'
		
		j=j+1
	
	print(num_words)
	#continue;
		#else :
			#remember_1=remember_1+" "+st;
			
	
	#print(num_words[2])
	#for st in words:
	#print(remember_1)
	

#text = "Определить острые углы прямоугольного треугольника,если медиана,проведенная к его гипотенузе,делит прямой угол в отношении 1:2."
#text="Дан треугольник, две вершины которого лежат на окружности радиуса R, две другие на касательной к этой окружности."
#text="Общая хорда двух окружностей служит для одной из них стороной вписанного квадрата,а для другой - стороной правильного вписанного шестиугольника."
#text="Найти расстояние между центрами окружностей,если радиус меньшей из них равен r."
#text="Из внешней точки к окружности проведены секущая длиной 12 см и касательная,длина которой составляет 2/3 внутреннего отрезка секущей."
#text="Длины параллельных сторон трапеции равны 25 и 4 см, а длины непараллельных сторон - 20 и 13 см."
#text="Найти площадь треугольника, образованного общими внешними касательными к этим окружностям."
#text="На сторонах квадрата вне его построены правильные треугольники, и их вершины последовательно соединены."
#text="В ромб,который делится своей диагональю на два равносторонних треугольника,вписана окружность радиуса 2."
#text="Найдите углы равнобедренного треугольника, вписанного в окружность, боковая сторона которого стягивает дугу, градусная мера которой 38 градусов."
#text="К окружности , вписанной в равнобедренный треугольник с основанием 12 см и высотой 8 см , проведена касательная,параллельная основанию."
#text="В ромб ABCD и в треугольник ABC,содержащий его большую диагональ,вписаны окружности."
text="Найдите основание равнобедренного треугольника, если центр вписанной в него окружности делит высоту, проведенную к основанию , в отношении 12:5,считая от вершины,а боковая сторона равна 60 см."
print(text)	
text=text.replace(".",'')
tokens = russianTokenizer(text)



#for i in range(len(words)):
	#print(str(words[i]))
		
#print(tokens)
