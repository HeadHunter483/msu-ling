import os
import re
import sys
import codecs

path = os.path.dirname(os.path.abspath(__file__))

conll=[]
pr=[]
par_mas=[]
word_root=""
word_root_mas=[]

one_time=0


def find_padej(index):
    arr1=['nom','gen','dat','acc','ins','loc']
    s=''
    i=0
    for i in range(6):
        if arr1[i] in conll[index][4]:
            s=arr1[i]
            break
        
    return s

def find_num(index):
    arr1=['pl','sg']
    s=''
    i=0
    for i in range(2):
        if arr1[i] in conll[index][4] and not "plen" in conll[index][4]:
            s=arr1[i]
            break
        
    return s

def pr_synt(predlog):
    padej=''
    par_mas.clear()
    for i in range(len(pr)):
        if predlog in pr[i][0]:
            par_mas.append(pr[i][1])
            par_mas.append(pr[i][2])
            par_mas.append(pr[i][3])
            
            
    return par_mas
    

def func_4(index_1,index_2,parent,morph_parametr,type_of_link):
    if morph_parametr in conll[index_2][4]:
        conll[index_2][4]=conll[index_2][4].replace(morph_parametr,par_mas[index_1])
        conll[index_2][5]=conll[index_2][4]


def find_noun(start,finish,padej,type1):
    j=start

    while(j<finish):
        if (conll[j-1][3]!="PR" and conll[j-1][3]!="S" and conll[j][3]=="S"):
            
            str4=find_padej(j)
            str3=find_num(root-1)

            if str3=="pl":
                str5="sg"

            if str3=="sg":
                str5="pl"
            
            if not str3 in conll[j][4]:
                conll[j][4]=conll[j][4].replace(str5,str3)

            if (type1==0):#partcp
                if (j<root-1):
                    conll[j][4]=conll[j][4].replace(str4,"acc")
                    conll[j][5]=conll[j][4]
                else:
                    if str4!="ins":
                        conll[j][4]=conll[j][4].replace(str4,"acc")
                        conll[j][5]=conll[j][4]
                        break
                
                
            if (type1==2): #v
                if (j<root-1  and not "imper" in conll[root-1][4]):
                    if str4!="gen":
                        conll[j][4]=conll[j][4].replace(str4,"nom")
                        conll[j][5]=conll[j][4]
                else:
                    if (j>root-1 and "imper" in conll[root-1][4]):
                        conll[j][4]=conll[j][4].replace(str4,"acc")
                        conll[j][5]=conll[j][4]
                        break

                    if (j>root-1 and not "imper" in conll[root-1][4]):
                        if (str4=="acc" and not "1p" in conll[root-1][4]):
                            conll[j][4]=conll[j][4].replace(str4,"nom")
                            conll[j][5]=conll[j][4]
                            break
                    
                        #if conll[root-1][1]!="есть":
                            #conll[j][4]=conll[j][4].replace(str4,"acc")
                            #conll[j][5]=conll[j][4]
                            #break
                    

            if (type1==1): #a
                if (j<root-1):
                    conll[j][4]=conll[j][4].replace(str4,"nom")
                    conll[j][5]=conll[j][4]
                    break
                else:
                    conll[j][4]=conll[j][4].replace(str4,"dat")
                    conll[j][5]=conll[j][4]
                    break
                    
            
           
        
        j=j+1

    
#########################################################
  
f=open("tmp.txt","r",encoding="utf-8")
f2=open("predlogipadeji.txt","r",encoding="utf-8")


for line in f2:
    row=line.split('	')
    pr.append(row)

f2.close()


for line in f:
    row=line.split('	')
    conll.append(row)

f.close()

############## Пытаемся поделить предложение на простые #######################

root=0
flag=0

for i in range(len(conll)):
    if conll[i][3]=="V":
        root=root+1
        
    if ( (conll[i][3]=="PARTCP" or conll[i][3]=="A") and "brev" in conll[i][4]) or '-' in conll[i][3]: 
        root=root+1

a=root

oborot_flag=0
#for i in range(len(conll)):
    #if "," in conll[i][1] and conll[i+1][1]!="а" and conll[i+1][1]!="что" and conll[i+1][1]!="если" and conll[i+1][2]!="который":
        #if conll[i-1][3]==conll[i+1][3]:
            #continue
        
        #if oborot_flag==1:
            #oborot_flag=0
            #continue
    
        #if conll[i+1][3]=="PARTCP" and "plen" in conll[i+1][4]:
            #oborot_flag=1
            #continue
        
        #conll[i][1]=conll[i][1].replace(",",".")
        #conll[i][2]="."
        #conll[i][3]="SENT"
        #conll[i][4]="SENT"
        #conll[i][5]="SENT"
        #conll[i+1][1]=conll[i+1][1].capitalize()
        #root=root-1

amount=0
for i in range(len(conll)):
    if (conll[i][1]=="."):
        amount=amount+1

if (amount==1 and a==0 and conll[0][3]!="PARTCP"):
    line="0	есть	быть	V	V ipf praes sg indic - 3p	V ipf praes sg indic - 3p	_	_	_	_\n"
    conll.insert(0,line.split('	'))


#############################  Теперь ищем 1 корень. Чтобы опираться на него при исправлении ошибок ##############################################


flag=0
root=0

for i in range(len(conll)):
    if conll[i][3]=="V": # это может быть глагол
        word_root=conll[i]
        word_root_mas.append(word_root)
        root=i+1
        flag=1

    if flag==0 and ((conll[i][3]=="PARTCP" or conll[i][3]=="A") and "brev" in conll[i][4]) or '-' in conll[i][3]:
        word_root=conll[i] #  а также тире краткое причастие или прилагательное , больше и меньше
        word_root_mas.append(word_root)
        root=i+1
        flag=1

    if "." in conll[i][1]:
        flag=0
           

######################################################

index=[]
index.append(0)


for i in range(len(conll)): # если корень причастие то подлежащего нет
    if "." in conll[i][1]:
        index.append(i)

for j in range(len(word_root_mas)):
    if "PARTCP" in word_root_mas[j]:
            find_noun(0,len(conll),"acc",0)
    if "A" in word_root_mas[j]:
            find_noun(0,len(conll),"nom",1)
    if "V" in word_root_mas[j]:
            find_noun(0,len(conll),"nom",2)

analog_root=0
analog_start=0
for i in range(len(conll)):
    if (conll[i][1]=="если"):
        j=i
        analog_start=i
        while(j<len(conll)):
            if (conll[j][3]=="A" and "brev" in conll[j][4]):
                analog_root=j
            j=j+1

if (analog_start>1):
    for i in range(analog_start,len(conll)):
        if (conll[i][3]=="S" and conll[i-1][3]!="PR" and "acc" in conll[i][4]):
            str4=find_padej(i)
            conll[i][4]=conll[i][4].replace(str4,"nom")
            conll[i][5]=conll[i][5].replace(str4,"nom")
            break

#дополнительно проверяем

for i in range(len(conll)):
    if (conll[i][3]=="A" or conll[i][3]=="PARTCP") and "brev" in conll[i][4]:
        j=i
        while(j<len(conll)):
            if conll[j][3]=="PR":
                break
            
            if conll[j][3]=="S":
                str3=find_num(i)
                str4=find_padej(j)

                if str3=="pl":
                    str5="sg"

                if str3=="sg":
                    str5="pl"
            
                if not str3 in conll[j][4]:
                    conll[j][4]=conll[j][4].replace(str5,str3)

                if conll[i][3]=="A" and conll[j-1][3]!="CONJ":
                    conll[j][4]=conll[j][4].replace(str4,"dat")
                    
                if conll[i][3]=="A" and conll[j-1][3]=="CONJ":
                    conll[j][4]=conll[j][4].replace(str4,"nom")
                    
                if conll[i][3]=="PARTCP":
                    if str4!="ins":
                        conll[j][4]=conll[j][4].replace(str4,"acc")
                    
                conll[j][5]=conll[j][4]
                break
            
            j=j+1

    if (conll[i][3]=="A" or conll[i][3]=="PARTCP") and not "brev" in conll[i][4]:     
        j=i
        while(j<len(conll)):
            if conll[j-1][3]!="PR" and conll[j][3]=="S":
                str3=find_num(i)
                str4=find_padej(i)
                
                if str3=="pl":
                    str5="sg"

                if str3=="sg":
                    str5="pl"
            
                if not str3 in conll[j][4]:
                    conll[i][4]=conll[i][4].replace(str5,str3)

                if not str4 in conll[j][4]:
                    str2=find_padej(j)
                    conll[i][4]=conll[i][4].replace(str4,str2)
                    
                conll[i][5]=conll[i][4]
                break
            
            j=j+1
    

for i in range(len(conll)):            
    if conll[i][3]=="S":
        j=i+1
        while(j<len(conll)):
            if  conll[j][3]!="S" and (conll[j][3]!="A" or conll[j][3]!="PARTCP") and not "brev" in conll[j][4]:
                break
            
            if conll[j][3]=="S":
                str4=find_padej(j)
                if str4!="gen" and str4!="ins" and str4!="acc" and str4!="nom":
                    conll[j][4]=conll[j][4].replace(str4,"gen")
                    conll[j][5]=conll[j][4]
                    break
            j=j+1


for i in range(len(conll)):
        #func_3(i,"A","plen","S","opred",1) #вниз

        if conll[i][3]=="PR": #для c наоборот
            pr_synt(conll[i][1])
                
            j=i
            index=0
            while(j<len(conll)):
                if conll[j][3]=="S":
                    str4=find_padej(j)
                    conll[j][4]=conll[j][4].replace(str4,par_mas[0])
                    conll[j][5]=conll[j][4]    
                    break
                    
                
                j=j+1


for i in range(len(conll)):
    if ((conll[i][3]=="A" or conll[i][3]=="PARTCP") and not "brev" in conll[i][4]):
        j=i
        while(j<len(conll)):
            if (conll[j][3]=="PR"):
                break
            
            if (conll[j][3]=="S"):
                str4=find_padej(j)
                
                if not str4 in conll[i][4]:
                    str2=find_padej(i)
                    conll[i][4]=conll[i][4].replace(str2,str4)

                conll[i][5]=conll[i][4]
                break
            
            j=j+1
        

for i in range(len(conll)):
    if conll[i][3]=="SPRO":
        j=i
        while(j<len(conll)):
            if conll[j][3]=="S" and "nom" in conll[j][4] or "acc" in conll[j][4]:
                str4=find_padej(i)
                conll[i][4]=conll[i][4].replace(str4,"gen")
                conll[i][5]=conll[i][4]
                break


            j=j+1

    if conll[0][1]!="есть":
        if conll[i-1][3]!="PR" and conll[i][3]=="NUM" and conll[i+1][3]=="S":
            str4=find_padej(i+1)
            conll[i+1][4]=conll[i+1][4].replace(str4,"gen")
            conll[i+1][5]=conll[i+1][4]
    #if conll[0][1]=="есть":
         #if conll[i-1][3]!="PR" and conll[i][3]=="NUM" and conll[i+1][3]=="S":
            #str4=find_padej(i+1)
            #conll[i+1][4]=conll[i+1][4].replace(str4,"nom")
            #conll[i+1][5]=conll[i+1][4]

    if (conll[i][1]=="1 из"):
        str4=find_padej(i+1)
        conll[i+1][4]=conll[i+1][4].replace(str4,"gen")
        conll[i+1][5]=conll[i+1][4]

for i in range(len(conll)):
    if conll[i][3]=="V" and conll[i+1][3]=="V" and "inf" in conll[i+1][4]:
        j=i+1
        while(j<len(conll)):
            if (conll[j][3]=="S"):
                str4=find_padej(j)
                conll[j][4]=conll[j][4].replace(str4,"acc")

                if conll[j-1][3]=="A":
                    str4=find_padej(j-1)
                    conll[j-1][4]=conll[j-1][4].replace(str4,"acc")
                break

            j=j+1


for i in range(len(conll)):
    if conll[i-1][3]=="V" and "inf" in conll[i-1][4] and conll[i][3]=="S":
        str4=find_padej(i)
        conll[i][4]=conll[i][4].replace(str4,"acc")
        break
    

#if conll[0][1]=="есть":
    #for i in range(1,len(conll)):
        #if conll[i][2]=="сторона" and not "gen" in conll[i][4]:
            #str3=find_num(i)
            #conll[i][4]=conll[i][4].replace(str3,"pl")
            #conll[i][5]=conll[i][4]
                
        
###############################################################

    
        
f=codecs.open("tmp.txt","w","utf-8")
i=0
for i in range(len(conll)):
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][5]+'	'+conll[i][6]+'	'+conll[i][7]+'	'+conll[i][8]+'	'+conll[i][9])

f.close()

