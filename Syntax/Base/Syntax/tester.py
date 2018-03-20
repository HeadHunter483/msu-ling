import os
import sys

file_1=[]
file_2=[]

f=open("syntax.txt","r",encoding="utf-8")

count_of_sentence_1=1
for i,line in enumerate(f):
    line=line.rstrip()
    
    if line=="":
        count_of_sentence_1=count_of_sentence_1+1
        continue

    row=line.split('	')
    file_1.append(row)

f.close()


f=open("Tests.txt","r",encoding="utf-8")

count_of_sentence_2=1

for i,line in enumerate(f):
    line=line.rstrip()

    if line=="":
        count_of_sentence_2=count_of_sentence_2+1
        continue

    if count_of_sentence_2==count_of_sentence_1: 
        break
   
    row=line.split('	')    
    file_2.append(row)

f.close()


   
parent_link_type_link=0
parent_link = 0
type_link=0
morph_error=0
line=0
no_strings=0


if len(file_1)!=len(file_2):
    no_strings=abs(len(file_2)-len(file_1))
    length=min(len(file_1),len(file_2))
else:
    length=len(file_1)


f=open("test_result.txt","w",encoding="utf-8")
index=0

ind=[]

for i in range(len(file_1)):
    index=index+1
    if "SENT" in file_1[i][3]:
        ind.append(index)
        index=0

    
u=0
all_write=0
for i in range(length):
    if file_1[i][3]=="SENT":
        if (all_write==0):
            f.write("Correct !\n")
            f.write("------------------------------\n")
            
        all_write=0
        u=u+1
        line=0
        morph_error=0
        parent_link_type_link=0
        parent_link=0
        type_link=0
        procent_1=0

    
    if (file_1[i][4]==file_2[i][4] and file_1[i][5]==file_2[i][5] and file_1[i][6]==file_2[i][6]):
        continue
    else:
        all_write=all_write+1
        if (file_1[i][5]!=file_2[i][5] or file_1[i][6]!=file_2[i][6]):
            parent_link_type_link = parent_link_type_link+1
            f.write(file_1[i][1]+" "+file_1[i][4]+" "+file_1[i][5]+" "+file_1[i][6]+"\n")
            f.write(file_2[i][1]+" "+file_2[i][4]+" "+file_2[i][5]+" "+file_2[i][6]+"\n")
            f.write("------------------------------\n")
        
        if (file_1[i][5]!=file_2[i][6]):
            parent_link = parent_link+1

        if (file_1[i][6]!=file_2[i][6]):
            type_link = type_link+1

        if (file_1[i][4]!=file_2[i][4]):
            morph_error = morph_error+1

        line=line+1

        #f.write("Amount_of_sent1 "+str(count_of_sentence_1)+"\n")
        #f.write("Amount_of_sent2 "+str(count_of_sentence_2)+"\n")
        #f.write("No string "+str(no_strings)+"\n")
        f.write("Line "+str(line)+"\n")
        f.write('Morph '+str(morph_error)+"\n")
        procent_1=100/(int(ind[u]))
        f.write("Type+parent "+str(100-parent_link_type_link*procent_1)+"\n")
        f.write("Type "+str(100-type_link*procent_1)+"\n")
        f.write("Parent "+str(100-parent_link*procent_1)+"\n")
        f.write("------------------------------\n")

    
    
    


f.close()
