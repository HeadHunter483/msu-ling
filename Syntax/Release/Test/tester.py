import os
import sys

if __name__ == "__main__":
    infile=sys.argv[1] ### входной файл
    outfile=sys.argv[2] ### выходной файл
    etalon=sys.argv[3] ### эталон
    mode=sys.argv[4] #prog-моя программа
                     ## Malt - есть послабления
		
file_1=[] 
file_2=[] 
file_3=[] 

sent=[]

f=open(infile+".txt","r",encoding="utf-8")
f2=open("prog_my_in.txt","r",encoding="utf-8") ### считали предложения

for line in f2:
    row=line.split('	')
    sent.append(row)

f2.close()

mas=[]
for line in sent:
    for s in line:
        s2=s.split(' . ')
        for i in range(1,len(s2)):
            mas.append(s2[i])


def correct_or_not(k):
    str1=''
    str2=''

    if file_1[k][0]==file_2[k][0] and file_1[k][1]==file_2[k][1] and file_1[k][2]==file_2[k][2] and file_1[k][3]==file_2[k][3] and file_1[k][4]==file_2[k][4]:
        return 1

    if str(file_1[k][1]).lower()==str(file_2[k][1]).lower() and str(file_1[k][3]).lower()==str(file_2[k][3]).lower() and str(file_1[k][4]).lower()==str(file_2[k][4]).lower():
        return 1

    if mode=="malt":
        if file_1[k][2]=="NONLEX" or file_1[k][2]=="CONJ" or file_1[k][2]=="ADV" or file_1[k][2]=="PR" or file_1[k][2]=="SENT" or file_1[k][2]=="-" or file_1[k][2]=="," or file_1[k][2]=="PART" or str(file_2[k][1]).isdigit():
            return 1
        
        

count_of_sentence_1=0
for i,line in enumerate(f):
    line=line.rstrip()
    
    if line=="":
        count_of_sentence_1=count_of_sentence_1+1
        continue

    row=line.split('	')
    file_1.append(row)

f.close()

f=open(etalon+".txt","r",encoding="utf-8")
count_of_sentence_2=0

for i,line in enumerate(f):
    if count_of_sentence_2==count_of_sentence_1+1: 
        break
   
    line=line.rstrip()

    if line=="":
        count_of_sentence_2=count_of_sentence_2+1
        continue
   
    row=line.split('	')    
    file_2.append(row)

f.close()
error_flag=0
parent_link_type_link=0
parent_link = 0
type_link=0
morph_error=0
line=0

elements_in_sent=0
general_procent_1=0.0
general_procent_2=0.0
general_procent_3=0.0

a=0
b=0
c=0

procent_1=0
correct=0


f=open(outfile+".txt","w",encoding="utf-8")

table_1={} ## храним кол-во верных связей
table_2={} ## храним общее количество интересующих нас связей

########################
# 0 - root
# 1 - predic
# 2 - opred
# 3 - 1-kompl
# 4 - kvaziagent
# 5 - obst
# 6 - predl
# 7 - others

for i in range(8):
    table_1[i,1]=0
    table_2[i,1]=0

##########################################

str_=''
fl=0
sentences_now=0

for k in range(len(file_1)):
    
    ########## Выводим текущее предложение #######
    m=k
    while(fl==0 and m<len(file_1)):
        if file_1[m][2]=="SENT":
            str_=str_+' '+file_1[m][1]+'\n'
            f.write(str_)
            fl=1
            break
        else:
           str_=str_+file_1[m][1]+' '

        m=m+1    

    #############################################
        
    elements_in_sent=elements_in_sent+1

    ######## заполняем таблицу #################

    if file_2[k][4]=="root":
            table_2[0,1]=table_2[0,1]+1
    if file_2[k][4]=="predic":
            table_2[1,1]=table_2[1,1]+1
    if file_2[k][4]=="opred":
            table_2[2,1]=table_2[2,1]+1
    if file_2[k][4]=="1-kompl":
            table_2[3,1]=table_2[3,1]+1
    if file_2[k][4]=="kvaziagent":
            table_2[4,1]=table_2[4,1]+1
    if file_2[k][4]=="obst":
            table_2[5,1]=table_2[5,1]+1
    if file_2[k][4]=="predl":
            table_2[6,1]=table_2[6,1]+1
    if file_2[k][4]!="root" and file_2[k][4]!="predic" and file_2[k][4]!="opred" and file_2[k][4]!="kvaziagent" and file_2[k][4]!="obst" and file_2[k][4]!="predl":
            table_2[7,1]=table_2[7,1]+1

    #################################

    if (k<len(file_1)-1 and int(file_1[k][0])>int(file_1[k+1][0])):#если следующий элемент по индексу меньше предыдущего. предложение закончилось.
        fl=0
        str_=''
        if (error_flag==0): ## если ошибок нету
            f.write('Correct\n')
            correct=correct+1
            general_procent_1=general_procent_1+100.0
            general_procent_2=general_procent_2+100.0
            general_procent_3=general_procent_3+100.0
            f.write("===========================\n")
            
        if (error_flag==1): ## если есть ошибка
            if (morph_error==0 and parent_link==0 and type_link==0): ## делаем проверку если все нет всех 3 типов ошибок то предложение верно разобранно
                f.write('Correct\n')
                f.write("===========================\n")
                correct=correct+1
                general_procent_1=general_procent_1+100.0
                general_procent_2=general_procent_2+100.0
                general_procent_3=general_procent_3+100.0
                continue

            ######### Выводим результат . Вычисляем средний процент по всему тексту ######
            
            procent_1=100/(int(elements_in_sent))

            general_procent_1=general_procent_1+(100-parent_link*procent_1)
            general_procent_2=general_procent_2+(100-type_link*procent_1)
            general_procent_3=general_procent_3+(100-parent_link_type_link*procent_1)

            a=100-parent_link_type_link*procent_1
            b=100-type_link*procent_1
            c=100-parent_link*procent_1
      
            f.write("ME: "+str(morph_error)+'\n')
            f.write("PL:" +str(parent_link)+'\n')
            f.write("TL:"+str(type_link)+'\n')
            f.write("L: "+str(line)+'\n')
            f.write("Type+parent "+str(a)+"\n")
            f.write("Type "+str(b)+"\n")
            f.write("Parent "+str(c)+"\n")
            f.write("===========================\n")

        error_flag==0
        morph_error=0
        parent_link=0
        type_link=0
        line=0
        elements_in_sent=0
        procent_1=0
        parent_link_type_link=0
       
              
    if correct_or_not(k)==1:### проверка данной строки. Если ==1 то ошибок в строке нет
        if file_2[k][4]=="root":
            table_1[0,1]=table_1[0,1]+1
        if file_2[k][4]=="predic":
            table_1[1,1]=table_1[1,1]+1
        if file_2[k][4]=="opred":
            table_1[2,1]=table_1[2,1]+1
        if file_2[k][4]=="1-kompl":
            table_1[3,1]=table_1[3,1]+1
        if file_2[k][4]=="kvaziagent":
            table_1[4,1]=table_1[4,1]+1
        if file_2[k][4]=="obst":
            table_1[5,1]=table_1[5,1]+1
        if file_2[k][4]=="predl":
            table_1[6,1]=table_1[6,1]+1
        if file_2[k][4]!="root" and file_2[k][4]!="predic" and file_2[k][4]!="opred" and file_2[k][4]!="kvaziagent" and file_2[k][4]!="obst" and file_2[k][4]!="predl":
            table_1[7,1]=table_1[7,1]+1      
        continue
    else:
        error_flag=1

        if (file_1[k][3]!=file_2[k][3] or file_1[k][4]!=file_2[k][4]):
            parent_link_type_link = parent_link_type_link+1

        if file_1[k][4]==file_2[k][4] and file_1[k][3]==file_2[k][3]:
            if file_2[k][4]=="root":
                table_1[0,1]=table_1[0,1]+1
            if file_2[k][4]=="predic":
                table_1[1,1]=table_1[1,1]+1
            if file_2[k][4]=="opred":
                table_1[2,1]=table_1[2,1]+1
            if file_2[k][4]=="1-kompl":
                table_1[3,1]=table_1[3,1]+1
            if file_2[k][4]=="kvaziagent":
                table_1[4,1]=table_1[4,1]+1
            if file_2[k][4]=="obst":
                table_1[5,1]=table_1[5,1]+1
            if file_2[k][4]=="predl":
                table_1[6,1]=table_1[6,1]+1
            if file_2[k][4]!="root" and file_2[k][4]!="predic" and file_2[k][4]!="opred" and file_2[k][4]!="kvaziagent" and file_2[k][4]!="obst" and file_2[k][4]!="predl":
                table_1[7,1]=table_1[7,1]+1
                     
        if (file_1[k][3]!=file_2[k][3]):
            parent_link = parent_link+1

        if (file_1[k][4]!=file_2[k][4]):
            type_link = type_link+1

        if (file_1[k][2]!=file_2[k][2]):
            morph_error = morph_error+1

        f.write(file_1[k][1]+" "+file_1[k][2]+" "+file_1[k][3]+" "+file_1[k][4]+"\n")
        f.write(file_2[k][1]+" "+file_2[k][2]+" "+file_2[k][3]+" "+file_2[k][4]+"\n")

        line=line+1

######## Для последнего #######
        
if (morph_error==0 and parent_link==0 and type_link==0):
    f.write('Correct\n')
    general_procent_1=general_procent_1+100
    general_procent_2=general_procent_2+100
    general_procent_3=general_procent_3+100
    print("Finished!\n")
else:
    f.write("ME: "+str(morph_error)+'\n')
    f.write("PL:" +str(parent_link)+'\n')
    f.write("TL:"+str(type_link)+'\n')
    f.write("L: "+str(line)+'\n')

    procent_1=100/(int(elements_in_sent))

    a=100-parent_link_type_link*procent_1
    b=100-type_link*procent_1
    c=100-parent_link*procent_1
    
    f.write("Type+parent "+str(a)+'\n')
    f.write("Type "+str(b)+'\n')
    f.write("Parent "+str(c)+'\n')
    f.write('\n')
    general_procent_1=general_procent_1+(100-parent_link*procent_1)
    general_procent_2=general_procent_2+(100-type_link*procent_1)
    general_procent_3=general_procent_3+(100-parent_link_type_link*procent_1)
    
    print("Finished!\n")

########## Общий результат ##################
    
f.write("===========================\n")
f.write("General stats:\n")
f.write("Task_count: "+str(len(sent))+'\n')
f.write("Sentences: "+str(count_of_sentence_1)+'\n')
f.write("Correct: "+str(correct)+'\n')
f.write("Parent+type_link: "+str(round((general_procent_3/count_of_sentence_1),5))+'\n')
f.write("Type link: "+str(round((general_procent_2/count_of_sentence_1),5))+'\n')
f.write("Parent link: "+str(round((general_procent_1/count_of_sentence_1),5))+'\n')

f.write("root: "+str(table_1[0,1])+"/"+str(table_2[0,1])+'\n')
f.write("predic: "+str(table_1[1,1])+"/"+str(table_2[1,1])+'\n')
f.write("opred: "+str(table_1[2,1])+"/"+str(table_2[2,1])+'\n')
f.write("1-kompl: "+str(table_1[3,1])+"/"+str(table_2[3,1])+'\n')
f.write("kvaziagent: "+str(table_1[4,1])+"/"+str(table_2[4,1])+'\n')
f.write("obst: "+str(table_1[5,1])+"/"+str(table_2[5,1])+'\n')
f.write("predl: "+str(table_1[6,1])+"/"+str(table_2[6,1])+'\n')

f.write("===========================\n")
f.close()
