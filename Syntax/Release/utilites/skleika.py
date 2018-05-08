import os
import re

diction=[]
punc=[]

path = os.path.dirname(os.path.abspath(__file__))

f2=open(path+"\\..\\tmp\\dict.txt","r",encoding="utf-8")


for line in f2:
    row=line.split('	')
    diction.append(row)

f2.close()

def sep(l):
	return len(l.replace('=',' ').split())==0

def skleika(r):
    rows=r
    flag=0
    flag2=1
    del_index=0
    str4=""
    
    for i in range(len(rows)):
        if rows[i][3]=="MEAS" or "см" in rows[i][1]:
            if (rows[i-1][1].isdigit() or re.search(r'\b\d+([\.,]\d+)', rows[i-1][1])): 
                rows[i-1][1]=rows[i-1][1]+rows[i][1]
                rows[i-1][2]=rows[i-1][2]+rows[i][1]
                    
            
        
        for j in range(len(diction)):

            ############ Работа с идиомами #############

            if rows[i][1] in diction[j][0]:
                if rows[i+1][1] in diction[j][0]:
                    rows[i][1]=diction[j][0]
                    rows[i][2]=diction[j][0]
                    str4=diction[j][1]
                    str4=str4.replace('\n','')
                    rows[i][3]=str4
                    rows[i][4]=str4
                    rows[i][5]=str4
                   
                    k=0
                    while(k<=int(diction[j][2])):
                        rows[i+k+1][1]="del"
                        k=k+1

            ############# Если нашли токен вида sqrt(num) ##################

        if rows[i][1]=="sqrt":
            if rows[i+1][1]=="(":
                if rows[i+3][1]==")":
                    rows[i][1]=rows[i][1]+rows[i+1][1]+rows[i+2][1]+rows[i+3][1]
                    rows[i][2]=rows[i][1]
                if rows[i+3][1]!=")":
                    rows[i][1]=rows[i][1]+rows[i+1][1]+rows[i+2][1]+")"
                    rows[i][2]=rows[i][1]
                    
                
                rows[i][3]="NUM"             
                rows[i][4]="NUM - - -"
               
                k=0
                while(k<3):
                    if k==2 and rows[i+k+1]!=")":
                        break
                    rows[i+k+1][1]="del"
                    k=k+1

            ############# Число_1 / число_2 (например 1/2) ##################

        if rows[i][1].isdigit() and rows[i+1][1]=="/" and rows[i+2][1].isdigit():
            rows[i][1]=rows[i][1]+rows[i+1][1]+rows[i+2][1]
            rows[i][2]=rows[i][1]
           
            k=0
            while(k<2):
                rows[i+k+1][1]="del"
                k=k+1

            ############## Мера и число перед ним 3 см #####################

        #if rows[i][3]=="MEAS": #and (rows[i-1][1].isdigit() or re.search(r'\b\d+([\.,]\d+)', rows[i-1][1])):
               #print(rows[i-1][1])
               #rows[i-1][1]=rows[i-1][1]+rows[i][1]
               #rows[i-1][2]=rows[i-1][2]+rows[i][1]
               
        
              
        if "см" in rows[i][1]:
            rows[i][4]="NUM NUM - - -"
            rows[i][5]=rows[i][4]
            rows[i][3]="NUM"

        str2=''
        if rows[i][3]=="NUM" and rows[i+1][1]==":":
            j=i
            while(j<len(rows)):
                if (rows[j][3]!="NUM" and rows[j][1]!=":"):
                    break
                else:
                    str2=str2+rows[j][1]
                    rows[j][1]="del"
                j=j+1

            rows[i][1]=str2
            rows[i][2]=rows[i][1]

        
       

        #if ":" in rows[i][1] and ":" in rows[i+1][1]:
            #rows[i][1]=rows[i][1]+rows[i+1][1]
            #del rows[i+1]

    #if rows[i][3]=="MEAS" and rows[i-1][3]=="NUM":
        #if not "см" in rows[i-1][1] and "градуса" in rows[i-1][1] and "градусов" in rows[i-1][1]:
            #rows[i-1][1]=rows[i-1][1]+" "+str(rows[i][1])
            #rows[i-1][2]=rows[i-1][2]+" "+str(rows[i][1])

        #if rows[i][3]=="MEAS":
            #del rows[i]

            ############### Знаки пунктуации заменяем на обозначения в таблице ########

    
    for i in range(len(rows)):
        if (rows[i][1]=="." or rows[i][1]=="," or rows[i][1]=="?"):
            if (rows[i][1]==","):
                rows[i+1][8]="after_punc"
            else:
                rows[i-1][8]="before_punc"
                
            
            punc.append(rows[i])

    

    res=[]

    for i in range(len(rows)):
        if (rows[i][1]!="." and rows[i][1]!="," and rows[i][1]!="?" and rows[i][1]!="del" and rows[i][3]!="MEAS" and rows[i][1]!="см" and rows[i][2]!="градус"):
            res.append(rows[i])

    end_flag=0
    for i in range(len(res)):
        if res[i][8]=="before_punc":
            end_flag=1
            break

    if end_flag==0:
        res[len(res)-1][8]="before_punc"

    res=res+punc

    return res






