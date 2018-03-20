import os
import re
import sys
import codecs
import subprocess
import time

path = os.path.dirname(os.path.abspath(__file__))

if __name__ == "__main__":
    in_name=sys.argv[1]
    out_name=sys.argv[2]


type1=0
conj=['а','если','что','когда']
conll=[]
new_conll=[]
tmp=[]

conj_now=''
index=0
word=''
amount_of_zap=0
type2=0


def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))


def find_case(index): # в слово с указанным индексом ищем падеж
    arr_cases=['nom','gen','dat','acc','ins','loc']
    s=''
    for i in range(6):
        if arr_cases[i] in conll[index][4]:
            s=arr_cases[i]
            break
        
    return s

def write_to_file(name,arr,mode): #массив arr записать в файл name
     f=open(path+"\\tmp\\"+name,mode,encoding="utf-8")

     for i in range(len(arr)):
         f.write(str(i+1)+'	'+arr[i][1]+'	'+arr[i][2]+'	'+arr[i][3]+'	'+arr[i][4]+'	'+arr[i][5]+'	'+arr[i][6]+'	'+arr[i][7]+'	'+arr[i][8]+'	'+arr[i][9])

     f.close()
     name=''

def recount(start,finish,arr,type):
    index=0
    for i in range(1,len(arr)):
           if int(arr[i-1][0])>int(arr[i][0]):
               index=i
               break

    for i in range(index,len(arr)):
        if (arr[i][6]=="_"):
            arr[i][7]="wrong_link"
            arr[i][6]="0"
            
        arr[i][0]=str(int(arr[i][0])+index)
        if (type==0):
            arr[i][6]=str(int(arr[i][6])+index)
        else:
             arr[i][6]=str(int(arr[i][6])+index-1)
            

    for i in range(len(arr)):
        arr[i][6]=str(int(arr[i][6])+start)
        

def two_simple_with_conj(start,finish,tmp):
    root=0
    roo_t=0
    conj_id=0
    
    for i in range(len(tmp)):
        if tmp[i][3]=="V" or (tmp[i][3]=="A" and "brev" in tmp[i][4]):
            tmp[i][7]="root"
            root=root+1
    
    for i in range(len(tmp)):
        if tmp[i][7]=="root":
           roo_t=roo_t+1

        if roo_t>0 and tmp[i][1]=="и":
            conj_id=i

    f=open(path+"\\tmp\\tmp.txt","w",encoding="utf-8")

    for i in range(conj_id):
        f.write(str(i+1)+'	'+tmp[i][1]+'	'+tmp[i][2]+'	'+tmp[i][3]+'	'+tmp[i][4]+'	'+tmp[i][5]+'	'+tmp[i][6]+'	'+tmp[i][7]+'	'+tmp[i][8]+'	'+tmp[i][9])

    f.close()
    
    subprocess_cmd('python syntax_hand.py ')


    f=open(path+"\\tmp\\tmp.txt","w",encoding="utf-8")
    
    for i in range(conj_id,len(tmp)):
        f.write(str(i+1)+'	'+tmp[i][1]+'	'+tmp[i][2]+'	'+tmp[i][3]+'	'+tmp[i][4]+'	'+tmp[i][5]+'	'+tmp[i][6]+'	'+tmp[i][7]+'	'+tmp[i][8]+'	'+tmp[i][9])

    f.close()

    subprocess_cmd('python syntax_hand.py ')

    return root




def func_1(start,limit,diff,type_): #разбор простых предложений или 2-х простых связанных союзом
    if (type_==0):
        if conll[limit][1]==".":
            fin=limit+1
        else:
            fin=limit


        for i in range(start,fin):
            tmp.append(conll[i])  #положили необходимые значения в массив


        root=0
        root=two_simple_with_conj(start,limit,tmp)

        #if (root==1):
            #write_to_file("tmp.txt",tmp,"w") #записали в файл
            #subprocess_cmd('python syntax_hand.py ') #запускаем синтаксический анализ
            

    if (type_==1):
         tmp.clear()
         for i in range(start,limit):
             tmp.append(conll[i])

         write_to_file("tmp.txt",tmp,"w") #записали в файл
         subprocess_cmd('python syntax_hand.py ') #запускаем синтаксический анализ
         
       
    if type_==0: #считали данные анализа.Сделали пересчет
        f=open(path+"\\tmp\\tmp2.txt","r",encoding="utf-8")

        for line in f:
            row=line.split('	')
            new_conll.append(row)

        f.close()

        for i in range(len(new_conll)):
            if new_conll[0][3]=="CONJ" and new_conll[1][7]=="unkn_link_for_adv":
                new_conll[1][7]="root"
                new_conll[1][6]="1"
            if (len(new_conll)==2 and new_conll[0][3]=="V" and new_conll[1][1]==","):
                new_conll[0][7]="root"
                new_conll[1][6]="1"


        recount(start,finish,new_conll,0)       
        write_to_file("tmp3.txt",new_conll,"a") #записали в файд
        new_conll.clear()


    tmp.clear()
        

    
def func_2(start,finish,diff,type_): # простое предложений + оборот + конец.
    index=0
    for i in range(start,finish):
        if conll[i][3]=="PARTCP":
            index=i
            conll[i][6]="0" # назначили причастие корнем
            conll[i][7]="root"
            break

    end=0

    if (conll[finish-1][1]!="." and conll[finish][3]!="CONJ"):
        end=finish+1
    else:
        end=finish

    func_1(start,index,diff,1)
    func_1(index,end,diff,1)
            
    f=open(path+"\\tmp\\tmp2.txt","r",encoding="utf-8")

    for line in f:
        row=line.split('	')
        new_conll.append(row)

    f.close()

    recount(start,finish,new_conll,0)
          
    for i in range(start,finish):
        if new_conll[i][1]==",":
            new_conll[i][6]=str(i)
            new_conll[i][7]="punc"

        if new_conll[i][3]=="PARTCP":
            j=i
            str4=find_case(i+1)

            while(j>=0):
                if new_conll[j][3]=="S" and str4 in new_conll[j][4]:
                    new_conll[i][6]=str(j+1)
                    new_conll[i][7]="opred"
                    break

                j=j-1
                   
            
        if new_conll[i][1]==".":
            new_conll[i][6]=str(i)
            new_conll[i][7]="punc"
           

    write_to_file("tmp3.txt",new_conll,"a") #записали в файд         
    new_conll.clear()

               
def func_3(start,finish,diff): # простое + оборот + конец простого
    word=''
    buff=[]
    punc_buff=[]
    str4=""
    leng=0
    word_before=""

    for i in range(start,finish+1):#заносим знаки пунктуации в отдельный массив
        if (conll[i][1]=="," or conll[i][1]=="."):
            punc_buff.append(conll[i-1][1])
            punc_buff.append(conll[i-2][1])
            punc_buff.append(conll[i])

    for i in range(start,finish):
        if conll[i][3]=="PARTCP":
            conll[i][6]="0"
            conll[i][7]="root"
            word=conll[i][1]
            index=i
            break

    j=index
    while(j>=start):
        str4=find_case(index)
        if conll[j][3]=="S" and str4 in conll[j][4]:
            word_before=conll[j][1] #существительное к которому будет отнесено причастие
            break
        
        j=j-1

    tmp.clear()
    index_stop=0    
    j=index
    flag=0
    while(conll[j][1]!=","): #оборот в массив tmp
        tmp.append(conll[j])
        j=j+1
        index_stop=j

    i=start
    while(i<index-1): # в buff остальное предложение
        buff.append(conll[i])
        i=i+1

    
    i=index_stop+1
    while(i<finish):
        buff.append(conll[i])
        i=i+1

    
    str4=conll[len(buff)]
    write_to_file("tmp.txt",buff,"w") #записали в файд
    
    leng=len(buff)
    buff.clear()
    subprocess_cmd('python syntax_hand.py')

    line=str(len(tmp))+'.	.	SENT	SENT	SENT	_	_	_	_'

    write_to_file("tmp.txt",tmp,"w") #записали в файд

    tmp.clear()
    subprocess_cmd('python syntax_hand.py')
    
    conll.clear()

    f=open(path+"\\tmp\\tmp2.txt","r",encoding="utf-8")

    for line in f:
        row=line.split('	')
        conll.append(row)

    f.close()
 
    remember=0
    
    flag7=0
    for i in range(len(conll)):
        for j in range(0,len(punc_buff),3):
            if (conll[i][1]==punc_buff[j] and conll[i-1][1]==punc_buff[j+1]):
               conll.insert(len(conll),punc_buff[j+2])
               conll[len(conll)-1][7]="punc"
               conll[len(conll)-1][6]=str(i+1)
            #conll.insert(len(conll),str4)
        
        
        #if  flag7==0 and conll[i][1]==",":
            #conll[i][6]=str(index)
            #remember=i
            #flag7=1
            
        #if (remember!=i and conll[i][1]==","):
            #conll[i][6]=str(i-1)
            

        #if (conll[i][1]=="."):
            #conll[i][6]=str(i)



    #for i in range(start,finish):
        #if conll[i][1]==".":
            #str4=conll[i]
            #conll.insert(len(conll),str4)
            #del conll[i]
           
    recount(0,len(conll),conll,1)
          
    index2=0
    for i in range(len(conll)):
        if (conll[i][3]=="PARTCP"):
           index2=i
           j=i
           while(j>=start):
               if conll[i][2]=="который":
                   if conll[j][3]=="S":
                       conll[i][6]=str(j+1)
                       conll[i][7]="opred"
                       break

               str4=find_case(j)

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

    recount(start,finish,conll,1)
    write_to_file("tmp3.txt",conll,"a") #записали в файд      


def analyze(start,finish,conj_now):
    type1=0
    
    for i in range(start,finish):
        if (conll[i][1]=="," and (conll[i+1][4]=="который" or conll[i+1][3]=="PARTCP")):
            j=i+1
            while(j<finish): #оборот с двух сторон имеет запятые - после оборота может быть часть предложения
                if "," in conll[j][1] and conll[j+1][3]!="CONJ":
                    type1=2
                    index=i
                    word=conll[i+1][1]
                    break

                j=j+1

        
    if (type1==0):
         for i in range(start,finish):
                for j in range(len(conj)):
                    if (conll[i-1][1]=="," and conll[i][3]=="PARTCP"):
                        type1=1 
                        conj_now=conll[i][1]
                        type2=2 # указание что разделитель причастие
                        break

    
    return type1
   
    
           
        
    


    
try:
    f=open(path+"\\tmp\\"+in_name,"r",encoding="utf-8")

    for line in f:
        row=line.split('	')
        conll.append(row)

    f.close()


    ### ДЕЛАТЬ ПРОВЕРКУ НА ОДНОРОДНОСТЬ
    ######################################

    start=0
    finish=0
    main_type=0
    diff=0
    ex_diff=0

    for i in range(start,len(conll)):
        for j in range(len(conj)):
            if ((conll[i-1][1]=="," and conll[i][1]==conj[j]) or conll[i][1]=="."): ## предл1 , союз(conj) предл2
                    conj_now=conj[j]
                    finish=i
                    main_type=analyze(start,finish,conj_now)
                   
                    if (main_type==1):
                        if (os.path.isfile(path+"\\tmp\\tmp2.txt")):
                            os.remove(path+"\\tmp\\tmp2.txt")
                        
                        func_2(start,finish,diff,2) #третий аргумент для союза 1
                        diff=10+finish-start
                        break

                    if (main_type==0):
                        if (os.path.isfile(path+"\\tmp\\tmp2.txt")):
                            os.remove(path+"\\tmp\\tmp2.txt")
                        
                        func_1(start,finish,diff,0)
                        diff=finish-start
                        break
                    
                    if (main_type==2):
                        if (os.path.isfile(path+"\\tmp\\tmp2.txt")):
                            os.remove(path+"\\tmp\\tmp2.txt")
                        
                        func_3(start,finish,diff)
                        diff=finish-start                  
                        break
           
        start=finish
   

    f=open(path+"\\tmp\\tmp3.txt","r",encoding="utf-8")

    conll.clear()
    for line in f:
        row=line.split('	')
        conll.append(row)

    f.close()

    root_count=0
    conj_index=0
    main_root=0

    for i in range(len(conll)):
        if conll[i][7]=="root" and conll[i+1][3]=="NONLEX":
            conll[i+1][6]=str(i+1)
            conll[i+1][7]="1-kompl"

        if (conll[i][3]=="CONJ" and conll[i][1]=="а" and conll[i-1][1]==","):
            j=i
            while(j>0):
                if (conll[j][1]=="," and conll[j+1][1]=="если"):
                    conll[i][6]=str(j+2)
                    break
                j=j-1

    for i in range(len(conll)):
        if conll[i][7]=="root":
            if (root_count==0):
                main_root=i+1
                root_count=root_count+1
            else:
                conll[i][6]=str(conj_index+1)
                conll[i][7]="podch-soyuzn"
                root_count=root_count+1
            
        if conll[i][1]=="что" or conll[i][1]=="если" and conll[i-1][1]==",":
            conll[i][6]=str(main_root)
            conj_index=i
            conll[i-1][7]="punc"

        if conll[i][1]=="а" or conll[i][1]=="то" and conll[i][3]=="CONJ":
            j=i
            while(j>0):
                if (conll[j][7]=="podch-soyuzn" or conll[j][7]=="root"):
                    conll[i][6]=str(j+1)
                    break
                j=j-1


    for i in range(len(conll)):
        if conll[i][1]=="а" or conll[i][1]=="то" and conll[i][3]=="CONJ":
            j=i
            while(j<len(conll)):
                if (conll[j][7]=="podch-soyuzn"):
                    conll[j][6]=str(i+1)
                    break
                j=j+1

    add_sm=0
    for i in range(len(conll)):
        if "см" in conll[i][1]:
            add_sm=1

        if add_sm==1 and conll[i][1].isdigit():
            conll[i][1]=conll[i][1]+' '+"см"
            conll[i][2]=conll[i][1]
        
except:
    f=open(path+"\\tmp\\tmp3.txt","w",encoding="utf-8")
    f.write("error")
    f.close()
    
    f=codecs.open(path+out_name,"a","utf-8")
    f2=codecs.open(path+"\\graph.txt","w","utf-8")

    f.write("error")
    f.close()
    
    f2.write("error")
    f2.close()

else:         
    f=open(path+"\\tmp\\tmp3.txt","w",encoding="utf-8")
    for i in range(len(conll)):
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][5]+'	'+conll[i][6]+'	'+conll[i][7]+'	'+conll[i][8]+'	'+conll[i][9])

    f.close()        
    
    conll.clear()

    f=open(path+"\\tmp\\tmp3.txt","r",encoding="utf-8")

    for line in f:
        row=line.split('	')
        conll.append(row)

    f.close()

    f=codecs.open(path+"\\"+out_name,"a","utf-8")
    f2=codecs.open(path+"\\tmp\\graph.txt","w","utf-8")

    for i in range(len(conll)):
        f.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][6]+'	'+conll[i][7]+'\n')
        f2.write(str(i+1)+'	'+conll[i][1]+'	'+conll[i][2]+'	'+conll[i][3]+'	'+conll[i][4]+'	'+conll[i][6]+'	'+conll[i][7]+'\n')


    f.write('\n')
    f.close()
    f2.close()
    
    os.remove(path+"\\tmp\\tmp3.txt")
    print('finished')



    
