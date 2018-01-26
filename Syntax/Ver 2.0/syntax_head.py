import os
import re
import sys
import codecs
import subprocess

path = os.path.dirname(os.path.abspath(__file__))

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))

conll=[]
tmp=[]

def find_padej(index):
    arr1=['nom','gen','dat','acc','ins','loc']
    s=''
    i=0
    for i in range(6):
        if arr1[i] in conll[index][4]:
            s=arr1[i]
            break
        
    return s

def func_1(start,limit,type_):
   j=start
   while(j<limit):
        tmp.append(conll[j])
        j=j+1

   f=open("tmp.txt","w",encoding="utf-8")
   i=0        
   for i in range(len(tmp)):
       f.write(str(i+1)+'	'+tmp[i][1]+'	'+tmp[i][2]+'	'+tmp[i][3]+'	'+tmp[i][4]+'	'+tmp[i][5]+'	'+tmp[i][6]+'	'+tmp[i][7]+'	'+tmp[i][8]+'	'+tmp[i][9])

   f.close()
   tmp.clear()
   subprocess_cmd('python syntax_hand.py ')

   if type_==0:
       conll.clear()
       f=open("tmp2.txt","r",encoding="utf-8")

       for line in f:
           row=line.split('	')
           conll.append(row)

       f.close()

  

def func_2(word,type_):    
    i=0
    size=0
    
    for i in range(len(conll)):
        if conll[i][1]==word:
            func_1(0,i,1)

            if (type2==2):
                conll[i][7]="root"
            
            func_1(i,len(conll),1)
            size=len(conll)-i
            break
            
        
    conll.clear()
    

    f=open("tmp2.txt","r",encoding="utf-8")

    for line in f:
        row=line.split('	')
        conll.append(row)

    f.close()



    
    for i in range(len(conll)-1):
        if int(conll[i][0])>int(conll[i+1][0]):
            index=0
            index=i
            j=index
            
            while(j<len(conll)): #длина ?
                conll[j][0]=str(int(conll[j][0])+index)
                conll[j][6]=str(int(conll[j][6])+index+1)
                j=j+1

    root=0
    for i in range(len(conll)):
        if (type==1):
            if conll[i][7]=="root":
                root=i+1
                break

    for i in range(len(conll)):
        if conll[i][1]==word:
            if (type2==1):
                conll[i][6]=str(root)
                conll[i][7]="podch_soyuzn"

                j=i+1
                while(j<len(conll)):
                    if conll[j][7]=="root":
                        conll[j][6]=str(i+1)
                        conll[j][7]="unkn"

                    j=j+1

            if (type2==2):
                j=i
                str4=find_padej(i+1)

                while(j>=0):
                    if conll[j][3]=="S" and str4 in conll[j][4]:
                       conll[i][6]=str(j+1)
                       conll[i][7]="opred"
                       break

                    j=j-1
                   
            
        if conll[i][1]==",":
            conll[i][6]=str(i)
            conll[i][7]="punc"

            
   


def func_3(index,word):
    buff=[]
    str4=""
    leng=0
    word_before=""

    j=index
    while(j>=0):
        if conll[j][3]=="S":
            word_before=conll[j][1]
            break
        
        j=j-1
        

    index_stop=0    
    j=index+1
    flag=0
    while(flag!=1):
        if "," in conll[j][1]:
            flag=flag+1

        tmp.append(conll[j])
        j=j+1
        index_stop=j


    i=0
    while(i<=index):
        buff.append(conll[i])
        i=i+1

    
    i=index_stop
    while(i<len(conll)):
        buff.append(conll[i])
        i=i+1

    str4=conll[len(buff)]
    
    i=0

    f=open("tmp.txt","w",encoding="utf-8")

    for i in range(len(buff)):
        f.write(str(i+1)+'	'+buff[i][1]+'	'+buff[i][2]+'	'+buff[i][3]+'	'+buff[i][4]+'	'+buff[i][5]+'	'+buff[i][6]+'	'+buff[i][7]+'	'+buff[i][8]+'	'+buff[i][9])

    f.close()
    leng=len(buff)
    buff.clear()
    subprocess_cmd('python syntax_hand.py')
    
    i=0
    f=open("tmp.txt","w",encoding="utf-8")
           
    for i in range(len(tmp)):
       f.write(str(i+1)+'	'+tmp[i][1]+'	'+tmp[i][2]+'	'+tmp[i][3]+'	'+tmp[i][4]+'	'+tmp[i][5]+'	'+tmp[i][6]+'	'+tmp[i][7]+'	'+tmp[i][8]+'	'+tmp[i][9])
    f.close()

    
    tmp.clear()
    subprocess_cmd('python syntax_hand.py')
    
    f=open("tmp2.txt","r",encoding="utf-8")
    conll.clear()

    for line in f:
        row=line.split('	')
        conll.append(row)

    f.close()
    
    remember=0
    
    flag7=0    
    for i in range(len(conll)):
        if  flag7==0 and conll[i][1]==",":
            conll[i][6]=str(index)
            remember=i
            flag7=1
            
        if (remember!=i and conll[i][1]==","):
            conll[i][6]=str(i-1)
            

        if (conll[i][1]=="."):
            conll[i][6]=str(i)

    
    for i in range(len(conll)):
        if conll[i][1]==".":
            str4=conll[i]
            conll.insert(len(conll),str4)
            del conll[i]
           
        
    for i in range(len(conll)-1):
        if int(conll[i][0])>int(conll[i+1][0]):
            ind=leng-1
            j=leng
            while(j<len(conll)): #длина ?
                if conll[j][1]!="," and conll[j][1]!=".":
                    if int(conll[j][6])==0:
                        conll[j][6]="1"
              
                    conll[j][0]=str(int(conll[j][0])+ind)
                    conll[j][6]=str(int(conll[j][6])+ind)
                    
                j=j+1
                
            break

    index2=0
    for i in range(len(conll)):
        if (conll[i][1]==word):
           index2=i
           j=i
           while(j>=0):
               if conll[i][2]=="который":
                   if conll[j][3]=="S":
                       conll[i][6]=str(j+1)
                       conll[i][7]="opred"
                       break

               str4=find_padej(j)

               if conll[j][3]=="S" and str4 in conll[i][4]:
                   conll[i][6]=str(j+1)
                   conll[i][7]="opred"
                   break
                
               j=j-1


    
    j=index2
    
    while(j<len(conll)):
        if conll[j][7]=="root":
            i=j
            while(i>=0):
                if (conll[i][1]==word_before):
                    conll[j][6]=str(i+1)
                    conll[j][7]="opred"
                    break
                i=i-1


            break
           
        j=j+1

   
    
           
        
    


    

os.chdir(path+'\\tmp')

f=open("in_raw.conll","r",encoding="utf-8")

for line in f:
    row=line.split('	')
    conll.append(row)

f.close()

type=0
conj=['а','если','что','вписанного']
conj_now=''
index=0
word=''

amount_of_zap=0


### ДЕЛАТЬ ПРОВЕРКУ НА ОДНОРОДНОСТЬ

for i in range(len(conll)):            
    if (conll[i][1]=="," and (conll[i+1][4]=="который" or conll[i+1][3]=="PARTCP")):
        j=i+1
        while(j<len(conll)):
            if "," in conll[j][1]:
                type=2
                index=i
                word=conll[i+1][1]
                break

            j=j+1

type2=0                
for i in range(len(conll)):
    for j in range(len(conj)):
        if (type==0 and conll[i-1][1]=="," and (conll[i][1]==conj[j] or conll[i][3]=="PARTCP")):
            type=1
            if (conll[i][1]==conj[j]):
                conj_now=conj[j]
                type2=1
            if (conll[i][3]=="PARTCP"):
                conj_now=conll[i][1]
                type2=2
            
print("Type: "+str(type))



find=0

#for i in range(len(conll)):
    #if "nom" in conll[i][4] and (conll[i][3]=="S" or conll[i][3]=="SPRO"):
       #find=find+0.5
                                 
    #if conll[i][3]=="V" or conll[i][3]=="PARTCP" or "brev" in conll[i][4]:
        #find=find+0.5

    #if find>=0.5 and conll[i+1][3]!="CONJ" and conll[i][2]!="который" and conll[i+1][3]!="PARTCP" and conll[i][1]=="," and conll[i][1]==".":
        #j=0
        #type=4
        #conll[i][1]=conll[i][1].replace(",",".")
        #conll[i][2]=conll[i][1]
        #conll[i][3]=conll[i][1]
        #conll[i][4]=conll[i][1]
        #conll[i][5]="PUNC"
        #find=0


#if (type==4):
    #os.remove("tmp.txt")
    #os.remove("tmp2.txt")

    #for i in range(len(conll)-1):
        #if conll[i][1]==".":
            #func_1(0,i+1,1)
            #func_1(i+1,len(conll),1)
            #break
        

        
if (type==1):
    os.remove("tmp.txt")
    os.remove("tmp2.txt")
    func_2(conj_now,type2)

if (type==0):
    os.remove("tmp.txt")
    os.remove("tmp2.txt")
    func_1(0,len(conll),0)


if (type==2):
    os.remove("tmp.txt")
    os.remove("tmp2.txt")
    func_3(index,word)

root=0
flag=0



#for i in range(len(conll)):
    #if "," in conll[i][1] and conll[i+1][1]!="а" and conll[i+1][1]!="что":
        #conll[i][1]=conll[i][1].replace(",",".")
        #conll[i+1][1]=conll[i+1][1].capitalize()
        #root=root-1


        
        











f=codecs.open(path+"\\out.txt","a","utf-8")
i=0
        
for i in range(len(conll)):
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][5]+'	'+conll[i][6]+'	'+conll[i][7]+'	'+conll[i][8]+'	'+conll[i][9])

f.write('\n')
f.close()

f=codecs.open(path+"\\semantic.txt","a","utf-8")
i=0
for i in range(len(conll)):
    #print(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][6]+'	'+conll[i][7]+'\n')
    f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][6]+'	'+conll[i][7]+'\n')

f.write('\n')
f.close()


print('finished')


    
