import os
import re
import sys
import codecs
import subprocess

path = os.path.dirname(os.path.abspath(__file__))

conll=[]
pr=[]
par_mas=[]
rules=[] # правила из файла


def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))

subprocess_cmd('python morph_corrector.py ') 

f=open(path+"\\tmp\\tmp.txt","r",encoding="utf-8")
f2=open(path+"\\tmp\\predlogipadeji.txt","r",encoding="utf-8")
f3=open(path+"\\tmp\\rules.txt","r",encoding="utf-8")

for line in f2: #считали данные предлогов в массив pr
    row=line.split('	')
    pr.append(row)

f2.close()

for line in f3: #считали правила в массив rules
    row=line.split('	')
    rules.append(row)

f3.close()

def pr_synt(predlog):
    padej=''
    par_mas.clear()
    for i in range(len(pr)): # если встретили предлог то в par_mas кладем 3 значения
        if predlog in pr[i][0]:
            par_mas.append(pr[i][1]) #первый падеж
            par_mas.append(pr[i][2]) #второй падеж
            par_mas.append(pr[i][3]) #куда присоединить
            
    return par_mas

def find_padej(index): #ищем падеж
    arr1=['nom','gen','dat','acc','ins','loc']
    s=''
    i=0
    for i in range(6):
        if arr1[i] in conll[index][4]:
            s=arr1[i]
            break
        
    return s

try:
    # "Голова" разбивает предложение на части и каждую часть будем разбирать.Пишут в tmp
    for line in f:
        row=line.split('	')
        conll.append(row)

    f.close()


    i=0
    root=0
    flag=0

    #если предложение сложное то там уже могут быть размечены корни
    for i in range(len(conll)):
        if conll[i][7]=="root":
            conll[i][6]="0"
            root=i+1
            break


    if len(conll)==2: # Докажите ,
        conll[0][7]="root"
        conll[0][6]="0"
        flag=1

#Длина предложения 3 Если известно ,
    if len(conll)==3 and conll[0][3]=="CONJ" and conll[2][1]==",":
        conll[0][7]="podch-soyuzn"
        conll[0][6]="2"
        conll[1][7]="r"
        conll[1][6]="0"
        conll[2][7]="punc"
        conll[2][6]="2"   
        flag=1
    

    if (flag==0):
#root первые 5 строчки. Пока 2 строки готовы
        for i in range(len(conll)):
        #if oborot_flag==1:
            #break
    
            for j in range(3):
                if rules[j][4]=="no_pr":
                    if conll[i][3]=="PR":
                        break
	    
                if rules[j][1]=="V":
                    if conll[i][3]==rules[j][1]:
                        if conll[i-1][3]=="V":#быть начали и т.д
                            conll[i][6]=str(i)
                            conll[i][7]="1-kompl"
                            break
                        else:
                            conll[i][6]=rules[j][9]
                            conll[i][7]=rules[j][10]
                            root=i+1
                            break

                if rules[j][1]=="PARTCP" or rules[j][1]=="A":
                    if conll[i][3]==rules[j][1] and rules[j][2] in conll[i][4]:
                        conll[i][6]=rules[j][9]
                        conll[i][7]=rules[j][10]
                        root=i+1
                        break

        flag_tire=0
        for i in range(len(conll)):
            if conll[i][7]=="root":
                flag_tire=0
                break
        
        if flag_tire==0:
            for i in range(len(conll)):
                if (conll[i][1]=="-"):
                    if conll[i-1][3]=="NONLEX":
                        conll[i-1][6]=str(i)
                        conll[i-1][7]="predik"
                    
                    conll[i][7]="root"
                    conll[i][6]="0"
                    root=i+1
                    break

        for i in range(len(conll)):
            if conll[i][1]=="необходимо" and conll[i+1][3]=="S": #умрет от голода необходимо общение
                conll[i][6]=str(i)
                conll[i][7]="razyasnit"

#подлежащее
        for i in range(len(conll)):
            for j in range(5,6):
                if conll[i][3]==rules[j][1] and rules[j][2] in conll[i][4]:
                    if (rules[j][9]=="root"):
                        rules[j][9]=str(root)

                    conll[i][6]=rules[j][9]
                    conll[i][7]=rules[j][10]
	 
#дополнение в вин падеже
        for i in range(len(conll)):
            for j in range(6,9):
                if conll[i][3]==rules[j][1] and rules[j][2] in conll[i][4]:
                    if (rules[j][4]=="no_pr" and conll[i][3]=="PR"):
                        break

                    if (rules[j][9]=="root"):
                        rules[j][9]=str(root)

                #if rules[j][2]=="acc":#научились выращивать что-то. присоединить к выращивать а не научились
                        #k=j
                        #while(k>0):
                            #if conll[k][3]=="V" and "inf" in conll[k][4]:
                                #rules[j][9]=str(k+1)
                                #break
                            
                            #k=k-1

                    conll[i][6]=rules[j][9]
                    conll[i][7]=rules[j][10]
		 
      
#квазиагент род
        for i in range(len(conll)):
            for j in range(9,10):
                if conll[i][3]==rules[j][1] and rules[j][2] in conll[i][4]:
                    if (rules[j][4]=="no_pr" and conll[i][3]=="PR"):
                        break

                    if (rules[j][9]=="prev"):
                        k=i-1
                        while(k>=0):
                            if conll[k][3]==rules[j][5]:
                                conll[i][6]=str(k+1)
                                conll[i][7]=rules[j][10]
                                break
		    
                            k=k-1

    	    
#nonlex
        for i in range(len(conll)):
            for j in range(10,11):
                if conll[i][3]==rules[j][1]:
                    if (rules[j][9]=="prev"):
                        k=i
                        while(k>=0):
                            if conll[k][3]==rules[j][5]: 
                                conll[i][6]=str(k+1)
                                conll[i][7]=rules[j][10]
                                break
		    
                            k=k-1

#num
        for i in range(len(conll)):
            for j in range(11,12):
                if conll[i][3]==rules[j][1]:
                    if (rules[j][9]=="bf_root"):
                        if (i==root):
                            conll[i][6]=str(root)
                            conll[i][7]=rules[j][10]
                            break
		    
                        k=i
                        while(k>=0):
                            if (rules[j][4]=="no_pr" and conll[k][3]=="PR"):
                                break
  
                            if conll[k][3]==rules[j][5] and (rules[j][7] in conll[k][4]): 
                                conll[i][6]=str(k+1)
                                conll[i][7]=rules[j][10]
                                break
		    
                            k=k-1

                        if conll[i][6]=="_":
                            k=i
                            while(k<len(conll)):
                                if conll[k][3]==rules[j][5] and "gen" in conll[k][4] or "nom" in conll[k][4]: 
                                    conll[i][6]=str(k+1)
                                    conll[i][7]=rules[j][10]
                                    break
		    
                                k=k+1

#pr            
        for i in range(len(conll)):
            for j in range(12,14):
                 if conll[i][3]==rules[j][1]:
                     pr_synt(conll[i][1])
	      
                     if (rules[j][9]=="root" and rules[j][2] in par_mas[2]):
                         conll[i][6]=str(root)
                         conll[i][7]=rules[j][10]

	     
                     if (rules[j][9]=="prev" and rules[j][2] in par_mas[2]):
                         k=i
                         while(k>=0):
                             if conll[k][3]==rules[j][5]:
                                 conll[i][6]=str(k+1)
                                 conll[i][7]=rules[j][10]
                                 break

                             k=k-1

        for i in range(len(conll)):
            if conll[i][3]=="PR":
                 pr_synt(conll[i][1])

                 if "p" in par_mas[2]:
                     if conll[i-1][3]=="A" or conll[i-1][3]=="PARTCP":
                         conll[i][6]=str(i)
                         conll[i][7]="atrib"
		 
                     if conll[i-1][3]!="A" and conll[i-1][3]!="PARTCP":
                         conll[i][6]=str(root)
                         conll[i][7]="obst"
		 
	     
                 j=i
                 while(j<len(conll)):
                     str4=find_padej(j)
                     if (conll[j][3]=="S") and  (str4==par_mas[0] or str4==par_mas[1]):
                         conll[j][6]=str(i+1)
                         conll[j][7]="predl"
                         break
		 
                     j=j+1


#idiom and part
        for i in range(len(conll)):
            for j in range(14,15):
                 if (conll[i][3]==rules[j][1] and  rules[j][9]=="next"):
                     conll[i][6]=str(i+2)
                     conll[i][7]=rules[j][10]
	     
            for j in range(15,16):
                 if (conll[i][3]==rules[j][1] and  rules[j][9]=="root"):
                     conll[i][6]=str(root)
                     conll[i][7]=rules[j][10]
	     	    

#opred
        for i in range(len(conll)):
            for j in range(16,18):
                 if conll[i][3]==rules[j][1] and rules[j][2] in conll[i][4]:
                     if (rules[j][9]=="next"):
                         str4=find_padej(i)
                         k=i
                         while(k<len(conll)):
                             if conll[k][3]==rules[j][5] and str4 in conll[k][4]:
                                 conll[i][6]=str(k+1)
                                 conll[i][7]=rules[j][10]
                                 break

                             k=k+1                    

#adv
        for i in range(len(conll)):
            for j in range(18,19):
                 if conll[i][3]==rules[j][1]:
                     if (rules[j][9]=="next"):
                         if conll[i+1][3]==rules[j][5]:
                             conll[i][6]=str(i+2)
                             conll[i][7]=rules[j][10]
                         else:
                             conll[i][6]=str(root)
                             conll[i][7]=rules[j][10]

#, conj and .
        for i in range(len(conll)):
            for j in range(18,22):
                if (conll[i][3]==rules[j][1] and rules[j][9]=="prev"):
                    conll[i][6]=str(i)
                    conll[i][7]=rules[j][10]

#spro
        for i in range(len(conll)):
            for j in range(22,23):
                if (rules[j][4]=="no_bf_pr" and conll[i-1][3]!="PR" and conll[i][3]==rules[j][1] and rules[j][2] in conll[i][4]):
                    k=i+1
                    while(k<len(conll)):
                        if conll[k][3]=="S":
                            conll[i][6]=str(k+1)
                            conll[i][7]=rules[j][10]
                            break

                        k=k+1

### общие правила
        for i in range(len(conll)):
            if conll[i][3]=="CONJ":
                if (conll[i][1]=="так"):
                    conll[i][6]=str(root)
	
                j=i+1
                while(j<len(conll)):
                    if conll[i-1][3]==conll[j][3]: #ищем одинаковые части речи
                        conll[j][6]=conll[i-1][6]
                        conll[j][7]=conll[i-1][7]
                        break
		    
                    j=j+1

        for i in range(len(conll)):
            if conll[i][6]=="_" and conll[i][3]=="NUM":
                conll[i][6]=str(root)
                conll[i][7]="kolich"

        index=0
        for i in range(len(conll)): #если .. то
            if conll[i][1]=="то":
               index=i
               j=i
               while(j>=0):
                   if conll[j][1]=="если":
                       conll[index][6]=str(j+1)
                       conll[index][7]="diff_conj"
                       break
               
                   j=j-1

    
        if (index>0):
            for i in range(index,len(conll)):
                if conll[i][1]=="-":
                    conll[i][6]=str(i)
                    conll[i][7]="unkn_root"

                    if (conll[i-1][7]=="predik"):
                        conll[i-1][6]=str(index+1)
                    if (conll[i+1][3]=="S"):
                        conll[i+1][6]=str(i+1)

                    break

#for i in range(len(conll)):
    #if (conll[i-1][3]!="S" and conll[i][3]=="NONLEX"):
	#print("here")
	#if (conll[i+1][7]=="root"):
	    #conll[i][6]=str(i+2)
	    #conll[i][7]="predik"

	#if (conll[i-1][7]=="root"):
	    #conll[i][6]=str(i)
	    #conll[i][7]="1-kompl"

        count=0
        for i in range(len(conll)):
            if (conll[i][7]=="root"):
                count=count+1

        for i in range(len(conll)):
            if (count>1):
                if (conll[i][3]=="CONJ"):
                    j=i;
                    while(j>=0):
                        if (conll[j][7]=="root"):
                            conll[i][6]=str(j+1)
                            break
		
                        j=j-1


        index=0
        for i in range(len(conll)):
            if (conll[i][1]=="что"):
                index=i
                break

            for i in range(len(conll)):
                if conll[i][7]=="root" and index!=0:
                    conll[i][6]=str(index+1)

    ### после всего
        for i in range(len(conll)):
            if conll[i][3]=="CONJ" and conll[i][6]=="0":
                conll[i][6]=str(root)

            if conll[i][3]=="A" and conll[i][6]=="_":
                str4=find_padej(i)
                j=i
                while(j>0):
                    if conll[j][3]=="S" and str4 in conll[j][4]:
                        conll[i][6]=str(j+1)
                        conll[i][7]="opred"
                        break
                    
                    j=j-1

            if conll[i][1]=="тогда" and conll[i+1][1]=="и" and conll[i+2][1]=="только" and conll[i+3][1]=="тогда":
                conll[i][6]=str(root)
                conll[i+1][6]=str(i+1)
                conll[i+2][6]=str(i+2)
                conll[i+3][6]=str(i+3)
            
                conll[i+1][3]="CONJ"
                conll[i+2][3]="CONJ"
                conll[i+3][3]="CONJ"


        for i in range(len(conll)):
            if "следовательно" in conll[i][1] and root==0:
                j=i
                while(j<len(conll)):
                    if conll[j][3]=="S" and "nom" in conll[j][4]:
                        root=j+1
                        break
                    j=j+1
                
                conll[i][6]=str(root)
                conll[i+1][6]=str(i+1)
                conll[i][7]="vvodn"

            if conll[i][3]=="SPRO" and conll[i][6]=="_":
                str4=find_padej(i)
                j=i
                while(j<len(conll)):
                    if (conll[j][3]=="S"):
                        conll[i][6]=str(j+1)
                        conll[i][7]="opred"
                        break

                    j=j+1

            if conll[i][1]=="такие" and conll[i+1][1]=="как":
                j=i
                while(j>0):
                    if (conll[j][3]=="S"):
                        conll[i][6]=str(j+1)
                        break
                    j=j-1
                if (conll[i+2][3]=="S"):
                    conll[i+2][6]=str(i+2)
                    conll[i+2][7]="sravn-soyuzn"

except:
    f=codecs.open(path+"\\tmp\\tmp2.txt","a","utf-8")
    f.write("error")
    f.close()
else:
    f=codecs.open(path+"\\tmp\\tmp2.txt","a","utf-8")
    for i in range(len(conll)):
        #print(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][5]+'	'+conll[i][6]+'	'+conll[i][7]+'	'+conll[i][8]+'	'+conll[i][9])
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][5]+'	'+conll[i][6]+'	'+conll[i][7]+'	'+conll[i][8]+'	'+conll[i][9])

    f.close()                    
                                 


