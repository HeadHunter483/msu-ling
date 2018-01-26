import os
import re
import sys
import codecs

path = os.path.dirname(os.path.abspath(__file__))

conll=[]
pr=[]
par_mas=[]

rules=[]

f=open("tmp.txt","r",encoding="utf-8")
f2=open("predlogipadeji.txt","r",encoding="utf-8")
f3=open("rules.txt","r",encoding="utf-8")

for line in f2:
    row=line.split('	')
    pr.append(row)

f2.close()

for line in f3:
    row=line.split('	')
    rules.append(row)

f3.close()

def pr_synt(predlog):
    padej=''
    par_mas.clear()
    for i in range(len(pr)):
        if predlog in pr[i][0]:
            par_mas.append(pr[i][1])
            par_mas.append(pr[i][2])
            par_mas.append(pr[i][3])
            
            
    return par_mas

def find_padej(index):
    arr1=['nom','gen','dat','acc','ins','loc']
    s=''
    i=0
    for i in range(6):
        if arr1[i] in conll[index][4]:
            s=arr1[i]
            break
        
    return s
    

for line in f:
    row=line.split('	')
    conll.append(row)

f.close()

i=0
oborot_flag=0
root=0

for i in range(len(conll)):
    if conll[i][7]=="root":
        conll[i][6]="0"
        root=i+1
        oborot_flag=1
        break






#root первые 5 строчки. Пока 2 строки готовы
for i in range(len(conll)):
    if oborot_flag==1:
        break
    
    for j in range(3):
        if rules[j][4]=="no_pr":
            if conll[i][3]=="PR":
                break
            
        if rules[j][1]=="V":
            if conll[i][3]==rules[j][1]:
                conll[i][6]=rules[j][9]
                conll[i][7]=rules[j][10]
                root=i+1

        if rules[j][1]=="PARTCP" or rules[j][1]=="A":
            if conll[i][3]==rules[j][1] and rules[j][2] in conll[i][4]:
                conll[i][6]=rules[j][9]
                conll[i][7]=rules[j][10]
                root=i+1
            
        

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
             if (conll[j][3]=="S" or conll[j][3]=="SPRO") and  (str4==par_mas[0] or str4==par_mas[1]):
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
        #if conll[i-1][3]=="S":
            #str1=find_padej(i-1)
        j=i+1
        while(j<len(conll)):
            if conll[i-1][3]==conll[j][3]:
                conll[j][6]=conll[i-1][6]
                conll[j][7]=conll[i-1][7]
                break
                    
            j=j+1
                     

f=codecs.open("tmp2.txt","a","utf-8")
i=0
for i in range(len(conll)):
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][5]+'	'+conll[i][6]+'	'+conll[i][7]+'	'+conll[i][8]+'	'+conll[i][9])

f.close()                    
                                 


