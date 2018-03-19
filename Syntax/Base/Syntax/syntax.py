import os
import re
import sys
import codecs

path = os.path.dirname(os.path.abspath(__file__))

conll=[]
pr=[]
par_mas=[]

one_time=0


def func_1(index,parent,part_of_speach_1,morph_parametr_1,part_of_speach_2,type_of_link,mode):
    if (mode==1):
        if part_of_speach_1!="NONLEX":
             if part_of_speach_1 in conll[index][3]: 
                conll[i][6]=str(parent)
                conll[i][7]=type_of_link
        if part_of_speach_1=="NONLEX":
            if (conll[index-1][3]!="CONJ" and conll[index-1][3]!="PUNC") and part_of_speach_1 in conll[index][3]:
                conll[i][6]=str(parent)
                conll[i][7]=type_of_link


    if (mode==2):
        if conll[index][3]==part_of_speach_1 or conll[index][3]==part_of_speach_2:
            conll[index][6]=str(parent)
            conll[index][7]=type_of_link
                    
    if (mode==3):
        if conll[index][3]==part_of_speach_1  and morph_parametr_1 in conll[index][4]:
            conll[index][6]=str(parent)
            conll[index][7]=type_of_link
        

def func_2(index,part_of_speach_1,part_of_speach_2,type_of_link,mode):
    if (mode==1):
        j=0
        if conll[index][3]==part_of_speach_1:
            j=index
            while (j<len(conll)):
                if conll[j][3]==part_of_speach_2:
                    conll[i][6]=str(j+1)
                    conll[i][7]=type_of_link
                    break
            
                j=j+1

        
                
    if (mode==2):
        j=index
        while (j>=0):
            if conll[j][3]==part_of_speach_1:
               conll[i][6]=str(j+1)
               conll[i][7]=type_of_link
               break

            j=j-1


def find_padej(index):
    arr1=['nom','gen','dat','acc','ins','loc']
    s=''
    i=0
    for i in range(6):
        if arr1[i] in conll[index][4]:
            s=arr1[i]
            break
        
    return s
    

                

def func_3(index,part_of_speach_1,morph_parametr,part_of_speach_2,type_of_link,mode):
    if conll[i][3]==part_of_speach_1 and morph_parametr in conll[i][4]:
            if (mode==1):
                j=i
                s=""
                s2=""
                while (j<len(conll)):
                    if conll[j][3]==part_of_speach_2:
                        s=find_padej(index+1)
                        s2=find_padej(index)
                        
                        if (s!=s2):
                            conll[index][4]=conll[index][4].replace(s2,s)
                            conll[index][5]=conll[index][4]
                        
                        conll[i][6]=str(j+1)
                        conll[i][7]=type_of_link
                        break
                
                    j=j+1

            if (mode==2):
                j=i
                while (j>=0):
                    if conll[j][3]==part_of_speach_2:
                        conll[i][6]=str(j+1)
                        conll[i][7]=type_of_link
                        break
                
                    j=j-1


                

def func_4(index_1,index_2,parent,morph_parametr,type_of_link):
    if morph_parametr in conll[index_2][4]:
        conll[index_2][4]=conll[index_2][4].replace(morph_parametr,par_mas[index_1])
        conll[index_2][5]=conll[index_2][4]
        conll[index_2][6]=str(parent)
        conll[index_2][7]=type_of_link
        one_time=1

os.chdir(path+'\\tmp')  
f=open("in_raw.conll","r",encoding="utf-8")
f2=open("predlogipadeji.txt","r",encoding="utf-8")


for line in f2:
    row=line.split('	')
    pr.append(row)

f2.close()


def pr_synt(predlog):
    padej=''
    par_mas.clear()
    for i in range(len(pr)):
        if predlog in pr[i][0]:
            par_mas.append(pr[i][1])
            par_mas.append(pr[i][2])
            par_mas.append(pr[i][3])
            
            
    return par_mas



for line in f:
    row=line.split('	')
    conll.append(row)

f.close()

i=0
root=0



######### Правила насчет которых можно быть уверенным. Что может быть корнем
flag=0

for i in range(len(conll)):   
    if conll[i][3]=="V" or conll[i][1]=="больше" or conll[i][1]=="меньше" or conll[i][3]=="PARTCP" or '-' in conll[i][3] or "brev" in conll[i][4]:
        conll[i][6]="0"
        conll[i][7]="root"
        root=i+1
        flag=1

for i in range(len(conll)):
    if flag==0 and conll[i][3]!="V" and conll[i][3]!="PARTCP" and not "brev" in conll[i][4]:
        line="0	есть	быть	V	V ipf praes sg indic - 3p	V ipf praes sg indic - 3p	0	false_root	_	_\n"
        conll.insert(0,line.split('	'))
        root=1
        break


######## Основополагающие
for i in range(len(conll)):
        func_1(i,i,"NONLEX","-","-","atrib",1) #чисто часть речи
        func_1(i,i,"CONJ","-","-","unkn_link_for_conj",1)
        func_1(i,i,",","-","SENT","punc",2)
        func_2(i,"NUM","S","num",1) #часть речи и поиск
        func_3(i,"A","plen","S","opred",1) #вниз


        if conll[i][3]=="PART":
            conll[i][6]=str(root)
            conll[i][7]="unkn_link_for_part"
        
        if conll[i][3]=="PR": #для c наоборот
            pr_synt(conll[i][1])
    
            if "root" in par_mas[2]:
                conll[i][6]=str(root)
                conll[i][7]="predl"
            else :
                conll[i][6]=str(i)
                conll[i][7]="atrib"

                
            j=i
            index=0
            while(j<len(conll)):
                if conll[j][3]=="S" or conll[j][3]=="SPRO":
                  if not par_mas[0] in conll[j][4]:
                        func_4(0,j,i+1,"nom","predl")
                        func_4(0,j,i+1,"gen","predl")
                        func_4(0,j,i+1,"dat","predl")
                        func_4(0,j,i+1,"acc","predl")
                        func_4(0,j,i+1,"ins","predl")
                        func_4(0,j,i+1,"loc","predl")
                        
                  if par_mas[0] in conll[j][4]:
                      conll[j][6]=str(i+1)
                      conll[j][7]="predl"
                      
                  break
                
                j=j+1
               

######### Второй этап 

for i in range(len(conll)):
    if conll[i][3]=="SPRO" and not "nom" in conll[i][4]:
        j=i
        while(j<len(conll)):
            if conll[j][3]=="S":
                conll[i][6]=str(j+1)
                conll[i][7]="opred"
                break
            
            j=j+1
    
    if conll[i][3]=="CONJ" and conll[i+1][3]!="NUM":
        conll[i+1][6]="_"
        conll[i+1][7]="_"

    if conll[i][3]=="S" and "dat" in conll[i][4]:  
        j=i-1
        while(j>=0):
            if conll[j][7]=="root":
               conll[i][6]=str(root)
               conll[i][7]="1-kompl"
                
            j=j-1


    if conll[i][1]=="больше":
        j=i
        while(j<len(conll)):
            if conll[j][3]=="S" and "gen" in conll[j][4]:
                conll[j][6]=str(root)
                conll[j][7]="1-kompl"
                break
            
            if conll[j][3]=="A" and "gen" in conll[j][4]:
                conll[j][6]=str(root)
                conll[j][7]="1-kompl"
                break

            
            j=j+1
        
    if conll[i][6]=="_" and conll[i][7]=="_":
        func_1(i,root,"S","acc","-","1-kompl",3)
        #func_1(i,i,"S","ins","-","1-kompl",3)
             
        if conll[i][3]=="S" and "gen" in conll[i][4] and conll[i][6]=="_":
            j=i-1
            while(j>=0):
                if conll[j][3]=="S" and "nom" or "acc" in conll[j][4]:
                    conll[i][6]=str(j+1)
                    conll[i][7]="kvaziagent"
                    break
                
                j=j-1

        
        #func_3(i,"A","plen","S","opred",2) #вниз


#### промежуточный этап
str5=""
s=""
s1=""
s2=""
s3=""
flag_adj=0
for i in range(len(conll)):
    if (conll[i][3]=="CONJ" or conll[i][3]=="PUNC"):
        j=i+1
        while(j<len(conll)):
            if (conll[j][3]!="A" and conll[j][3]!="S"):
                break
            if (conll[j][3]=="S"):
                if (conll[j-1][3]=="A"):
                    flag_adj=1
                
                if conll[i-1][3]==conll[j][3]:
                    s=find_padej(i-1)
                    s2=find_padej(j)
               
                if s!=s2:
                    conll[j][4]=conll[j][4].replace(s2,s)
                    conll[j][5]=conll[j][4]

                    if (flag_adj==1):
                        conll[j-1][4]=conll[j-1][4].replace(s2,s)
                        conll[j-1][5]=conll[j-1][4]
                        flag_adj=0
        
                break

            j=j+1


        
                

##### третий этап

#для нескольких подлежащих отдельный случай

j=len(conll)-1
str4=""
s4=""

while(j>=0):
     if conll[j][3]=="S" and conll[j][6]=="_" and conll[j][7]=="_":
        i=j-1
        while(i>=0):
            if (conll[i][3]=="S"):
                str4=find_padej(i)

                if str4 in conll[j][4]:
                    conll[j][6]=conll[i][6]
                    conll[j][7]=conll[i][7]  
                    break
                    
                
            i=i-1

     j=j-1


            
str4=""                
for i in range(len(conll)):
    if (conll[i][1]=="1 из"):
        str4=find_padej(i+1)
        conll[i+1][4]=conll[i+1][4].replace(str4,"gen")
        conll[i+1][5]=conll[i+1][4]
        conll[i+1][6]=str(i+1)
        conll[i+1][7]="predl"

       
    func_1(i,root,"S","nom","-","predik",3)
    if (conll[i][6]=="_" and conll[i][7]=="_"):
        func_1(i,root,"ADV","-","-","unkn",1)

        if (conll[i][3]=="NUM"):
            j=i
            flag=0
            while(j>=0):
                if (conll[j][3]=="S" and ("ins" in conll[j][4] or "gen" in conll[j][4])):
                    conll[i][6]=str(j+1)
                    conll[i][7]="num"
                    flag=1
                    break
                j=j-1

            if (flag==0):
                conll[i][6]=str(root)
                conll[i][7]="num"
                

        if (conll[i][3]=="IDIOM"):
            conll[i][6]=str(root)
            conll[i][7]="unkn_link_for_idiom"

        
        if (conll[i][3]=="A"):
            str4=find_padej(i)
            
            j=i+1
            while(j<len(conll)):
                if conll[j][3]=="S" and str4 in conll[j][4]:
                    conll[i][6]=str(j+1)
                    conll[i][7]="opred"
                    break

                j=j+1

      

        if conll[i][3]=="S" and "ins" in conll[i][4]:
            j=i
            while(j>=0):
                if (j==root):
                    conll[i][6]=str(root)
                    conll[i][7]="1-kompl"
                    break
                j=j-1

                
          
    if (conll[i][3]=="NONLEX" and (conll[i-1][3]=="CONJ" or conll[i-1][3]==",")):
        j=i-1
        while(j>=0):       
            if conll[j][3]=="NONLEX":
                conll[i][6]=conll[j][6]
                conll[i][7]=conll[j][7]
                break

            j=j-1

        

os.chdir(path)  
f=codecs.open("out.txt","a","utf-8")
i=0
for i in range(len(conll)):
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][5]+'	'+conll[i][6]+'	'+conll[i][7]+'	'+conll[i][8]+'	'+conll[i][9])

f.write('\n')
f.close()


f=codecs.open("semantic.txt","a","utf-8")
i=0
for i in range(len(conll)):
        print(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][6]+'	'+conll[i][7]+'\n')
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][6]+'	'+conll[i][7]+'\n')

f.write('\n')
f.close()





