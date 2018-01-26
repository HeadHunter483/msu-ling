import os
import re
import sys
import codecs

path = os.path.dirname(os.path.abspath(__file__))

conll=[]
pr=[]
par_mas=[]
word_root=""

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



def work_padej():# падеж кроме случая соответствующий предлог+неверный падеж и движения снизу-сверх на 3-м этапе
    #его высота неверный падеж у его/ для союза .. и угол

    
    j=root

    while(j<len(conll)):
        if conll[j][3]=="PR":
            break

        if "PARTCP" in word_root and conll[j][3]=="S":
            str4=find_padej(j)

            if str4!="gen" and str4!="dat" and str4!="ins":
                conll[j][4]=conll[j][4].replace(str4,"acc")
                conll[j][5]=conll[j][4]
                break
        
        if "больше" in word_root or "меньше" in word_root   and conll[j][3]=="S":
            str4=find_padej(j)
            conll[j][4]=conll[j][4].replace(str4,"gen")
            conll[j][5]=conll[j][4]
            break

        
        j=j+1


    for i in range(len(conll)):
        if conll[i][3]=="PR":
            break
        
        if conll[i][3]=="S" and "dat" in conll[i][4]:  
            j=i-1
            while(j>=0):
                if conll[j][7]=="root":
                   conll[i][6]=str(root)
                   conll[i][7]="1-kompl"
                   break
                
                j=j-1

        if conll[i][3]=="S" and "ins" in conll[i][4]:
            j=i
            while(j>=0):
                if (j==root):
                    conll[i][6]=str(root)
                    conll[i][7]="1-kompl"
                    break
                
                j=j-1

   
    

    

    
f=open("tmp.txt","r",encoding="utf-8")
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
verb_flag=0

for i in range(len(conll)):
    if conll[i][3]=="V":
        conll[i][6]="0"
        conll[i][7]="root"
        word_root=conll[i]
        root=i+1
        flag=1


    if flag==0 and conll[i][3]=="PARTCP" or conll[i][1]=="больше" or conll[i][1]=="меньше" or '-' in conll[i][3] or "brev" in conll[i][4]:
        j=i
        while(j<len(conll)):
            if conll[j][3]=="V":
                conll[j][6]="0"
                conll[j][7]="root"
                word_root=conll[i]
                root=j+1
                flag=1
                break
                
            j=j+1

        if (flag==0):
            conll[i][6]="0"
            conll[i][7]="root"
            word_root=conll[i]
            root=i+1
            flag=1
           
    
			
print(flag)		
for i in range(len(conll)):
    if flag==0:
        line="0	есть	быть	V	V ipf praes sg indic - 3p	V ipf praes sg indic - 3p	0	false_root	_	_\n"
        word_root=line
        conll.insert(0,line.split('	'))
        root=1
        break

for i in range(len(conll)):       
    if conll[i-1][1]!="," and conll[i][6]=="_" and conll[i][3]=="PARTCP":
        str4=find_padej(i)
        j=i
        while(j<len(conll)):
            if conll[j][3]=="S" and str4 in conll[i][4]:
                conll[i][6]=str(j+1)
                conll[i][7]="opred"
                break
            j=j+1


for i in range(len(conll)):
    if "PARTCP" in word_root and conll[i][3]=="S" and "nom" in conll[i][4]:
            str4=find_padej(i)
            conll[i][4]=conll[i][4].replace(str4,"acc")
            conll[i][5]=conll[i][4]
            break

str4=""
for i in range(len(conll)):  
    if conll[i][3]=="S" and "acc" in conll[i][4]:
        str4=find_padej(i)
        if (conll[i][1]=="в"):
            break
            
            
        j=i
        while(j<len(conll)):
                if conll[j][7]=="root" and conll[j][3]=="A" and "brev" in conll[j][4]:
                    print('1')
                    conll[i][4]=conll[i][4].replace("acc","nom")
                    conll[i][5]=conll[i][4]
                    break
                

                j=j+1

for i in range(len(conll)):
    if conll[i][7]=="root" and conll[i][3]=="A" and "brev" in conll[i][4]:
        j=i
    
        while(j<len(conll)):
            if conll[j][3]=="S" and not "dat" in conll[j][4]:
                str4=find_padej(j)
                conll[j][4]=conll[j][4].replace(str4,"dat")
                conll[j][5]=conll[j][4]
                break
                

            j=j+1


        
for i in range(len(conll)):
    if conll[i][7]=="root" and "inf" in conll[i][4]:
        j=i
    
        while(j<len(conll)):
            if conll[j][3]=="S" and not "acc" in conll[j][4]:
                str4=find_padej(j)
                conll[j][4]=conll[j][4].replace(str4,"acc")
                conll[j][5]=conll[j][4]
                break
                

            j=j+1
            
    if conll[i][3]=="S" and "acc" in conll[i][4] or "nom" in conll[i][4]:
        j=i+1
        while(j<len(conll)):
            if conll[j][3]=="S":
                str4=find_padej(j)
                if str4!="gen":
                    conll[j][4]=conll[j][4].replace(str4,"gen")
                    conll[j][5]=conll[j][4]
                    break
            j=j+1


if conll[0][7]=="false_root":
    j=0
    while(j<len(conll)):
        if (conll[j][3]=="S"):
            str4=find_padej(j)
            conll[j][4]=conll[j][4].replace(str4,"acc")
            conll[j][5]=conll[j][4]
            conll[j][6]=str(1)
            conll[j][7]="1-kompl"

        j=j+1
        
                   

            


######## Основополагающие
for i in range(len(conll)):
        func_1(i,i,"NONLEX","-","-","atrib",1) #чисто часть речи
        func_1(i,i,"CONJ","-","-","sochin",1)
        func_1(i,i,",","-","SENT","punc",2)
        func_3(i,"A","plen","S","opred",1) #вниз

        j=0
        if conll[i][3]=="NUM":
            j=i
            while (j<len(conll)):
                print(conll[j][1])
                if conll[j][3]=="S" and "gen" in conll[j][4] or "acc" in  conll[j][4]:
                    conll[i][6]=str(j+1)
                    conll[i][7]="kolichest"
                    break
            
                j=j+1

        


        if conll[i][3]=="PART":
            conll[i][6]=str(root)
            conll[i][7]="unkn_link_for_part"

        if conll[i][3]=="PR": #для c наоборот
            pr_synt(conll[i][1])
    
            if "root" in par_mas[2]:
                conll[i][6]=str(root)
                conll[i][7]="obst"
                
            if "p" in par_mas[2]:
                if conll[i-1][3]=="PARTCP" or conll[i-1][2]=="прилежащий" or conll[i-1][2]=="равный":
                    conll[i][6]=str(i)
                    conll[i][7]="obst"
                else:
                    conll[i][6]=str(root)
                    conll[i][7]="predl"

            if conll[i][1]=="к":
                l=i
                while(l>=0):
                    if conll[l][3]=="S" and "gen" in conll[l][4]:
                        if conll[l-1][3]=="S" and "gen" in conll[l-1][4]:
                            conll[i][6]=str(l)
                        else:
                            conll[i][6]=str(l+1)
                            
                            
                    l=l-1
                
            if "s" in par_mas[2]:
                k=i
                while(k>=0):
                    if conll[k][3]=="S":
                        conll[i][6]=str(k+1)
                        conll[i][7]="atrib"
                        break
                    
                    k=k-1

                
            j=i
            index=0
            while(j<len(conll)):
                if conll[j][3]=="S" or conll[j][3]=="SPRO":
                  if not par_mas[0] in conll[j][4] and not par_mas[1] in conll[j][4]:
                        func_4(0,j,i+1,"nom","predl")
                        func_4(0,j,i+1,"gen","predl")
                        func_4(0,j,i+1,"dat","predl")
                        func_4(0,j,i+1,"acc","predl")
                        func_4(0,j,i+1,"ins","predl")
                        func_4(0,j,i+1,"loc","predl")
                        
                  if par_mas[0] in conll[j][4] or par_mas[1] in conll[j][4]:
                      conll[j][6]=str(i+1)
                      conll[j][7]="predl"
                      break
                
                j=j+1
               

######### Второй этап

work_padej()

for i in range(len(conll)):
    if conll[i][3]=="SPRO" and not "nom" in conll[i][4]:
        j=i
        while(j<len(conll)):
            if conll[i-1][3]=="PR":
                conll[i][6]=str(i)
                conll[i][7]="predl"
                break
            if conll[j][3]=="S":
                conll[i][6]=str(j+1)
                conll[i][7]="opred"
                break
            
            j=j+1
    
    if conll[i][3]=="CONJ" and conll[i+1][3]!="NUM":
        conll[i+1][6]="_"
        conll[i+1][7]="_"


    if conll[i-1][1]=="," and conll[i][3]=="PARTCP":
        j=i
        while(j<len(conll)):
            if conll[j][3]=="PR":
                break
            if conll[j][3]=="S":
                str4=find_padej(j)
                conll[j][4]=conll[j][4].replace(str4,"acc")
                conll[j][5]=conll[j][4]
                conll[j][6]=str(i+1)
                conll[j][7]="1-kompl"
                break

            j=j+1

        

    if conll[i][1]=="больше":
        j=i
        while(j<len(conll)):
            if conll[j][3]=="A" and "gen" in conll[j][4]:
                conll[j][6]=str(root)
                conll[j][7]="1-kompl"
                break

            
            j=j+1

    
    
    if conll[i][6]=="_" and conll[i][7]=="_":
        #work_padej(i)
        func_1(i,root,"S","acc","-","1-kompl",3)
        #func_1(i,i,"S","ins","-","1-kompl",3)
             
        
        
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
flag_conj=0
index_conj=0

while(j>=0):
     if conll[j][3]=="S" and conll[j][6]=="_" and conll[j][7]=="_":
        i=j-1
        print(conll[j][1])
        while(i>=0):
            #if (conll[i][3]=="CONJ"):
                #flag_conj=1
                #index_conj=i
                

            if (conll[i][3]=="S"):
                str4=find_padej(i)

                if str4 in conll[j][4]:
                    conll[j][6]=conll[i][6]
                    conll[j][7]=conll[i][7]
                    break
                    
                    #if (flag_conj==1):
                        #conll[index_conj]=str(j+1)

                    #break
                    
                
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
        if conll[i][3]=="PR":
            conll[i][6]=str(root)
            conll[i][7]="obst"    
        
        if conll[i][3]=="ADV" and conll[i+1][3]=="PARTCP":
            conll[i][6]=str(i+2)
            conll[i][7]="unkn"
        if conll[i][3]=="ADV" and conll[i+1][3]!="PARTCP":
            func_1(i,root,"ADV","-","-","unkn",1)

        if conll[i][3]=="S" and  (not conll[i-1][3]=="S" and "gen" in conll[i-1][4]) and conll[i][6]=="_":
            j=i
            str4=find_padej(i)
            while(j<len(conll)):
                if conll[j][3]=="PR":
                    break
                
                if conll[j][7]=="root" and conll[j][3]=="A" and "brev" in conll[j][4]:
                    print('3')
                    conll[i][4]=conll[i][4].replace(str4,"nom")
                    conll[i][5]=conll[i][4]
                    conll[i][6]=str(root)
                    conll[i][7]="predik"
                    break
                

                j=j+1
            

        if conll[i][3]=="S" and "gen" in conll[i][4] and conll[i][6]=="_":
            j=i-1
            while(j>=0):
                if conll[j][3]=="S" and "nom" or "acc" in conll[j][4]:
                    conll[i][6]=str(j+1)
                    conll[i][7]="kvaziagent"
                    break
                
                j=j-1

            

        if (conll[i][3]=="NUM"):
            j=i
            flag=0
            while(j>=0):
                if (conll[j][3]=="S" and "ins" in conll[j][4]):
                    conll[i][6]=str(j+1)
                    conll[i][7]="kolichest"
                    flag=1
                    break
                j=j-1

            if (flag==0):
                conll[i][6]=str(root)
                conll[i][7]="kolichest"
                

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

                
          
    if (conll[i][3]=="NONLEX" and (conll[i-1][3]=="CONJ" or conll[i-1][3]==",")):
        j=i-1
        while(j>=0):       
            if conll[j][3]=="NONLEX":
                conll[i][6]=conll[j][6]
                conll[i][7]=conll[j][7]
                break

            j=j-1


f=codecs.open("tmp2.txt","a","utf-8")
i=0
for i in range(len(conll)):
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][5]+'	'+conll[i][6]+'	'+conll[i][7]+'	'+conll[i][8]+'	'+conll[i][9])

f.close()


#f=codecs.open("semantic.txt","a","utf-8")
#i=0
#for i in range(len(conll)):
        #print(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][6]+'	'+conll[i][7]+'\n')
        #f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][6]+'	'+conll[i][7]+'\n')

#f.write('\n')
#f.close()





