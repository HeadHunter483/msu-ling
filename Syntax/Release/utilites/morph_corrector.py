import os
import re

path = os.path.dirname(os.path.abspath(__file__))

pr=[]

f3=open(path+"\\..\\tmp\\morph_iskl2.txt","r",encoding="utf-8")
f2=open(path+"\\..\\tmp\\predlogipadeji.txt","r",encoding="utf-8")

morph_iskl2=[]

for line in f3:
    row=line.split('	')
    morph_iskl2.append(row)

f3.close()
    
for line in f2:
    row=line.split('	')
    pr.append(row)

f2.close()

def morph_corrector(m):
    conll=m
    par_mas=[]
    word_root=""
    word_root_mas=[]

    def find_padej(index):
        arr1=['nom','gen','dat','acc','ins','loc']
        s=''
        
        for i in range(6):
            if arr1[i] in conll[index][4]:
                s=arr1[i]
                break
        
        return s

    def find_num(index):
        arr1=['pl','sg']
        s=''
    
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

    def change(i,j,index,padej):
        k=i-1
        while(k>=0):
           if conll[k][3]!="A" and conll[k][3]!="SPRO" and conll[k][3]!="PR":
               break
           if conll[k][3]=="PR":
               pr_synt(conll[k][1])
               str4=find_padej(i)
               if str4!=par_mas[0] and str4!=par_mas[1]:
                   if conll[i-1][3]!="SPRO":
                       conll[i][4]=conll[i][4].replace(str4,par_mas[0])
               padej=find_padej(i)
               
           k=k-1

        if (conll[i-1][3]=="SPRO" or conll[i-1][3]=="S") and "gen" in conll[i-1][4] and conll[i][3]=="S":
            str4=find_padej(i)
            if conll[i+1][3]=="A" and "brev" in conll[i+1][4] and conll[i-1][2]!="это":
                conll[i][4]=conll[i][4].replace(str4,"nom")
                
            if conll[i+1][3]=="A" and "brev" in conll[i+1][4] and conll[i-1][2]=="это":
                conll[i][4]=conll[i][4].replace(str4,"gen")
                
            padej=find_padej(i)
            
        if (i==0 and conll[i][3]=="S"):
            str4=find_padej(i)
            conll[i][4]=conll[i][4].replace(str4,"nom")
            padej=find_padej(i)

      
        if padej=="acc":
            if conll[i][8]=="after_punc" and conll[i-1][3]!=conll[i][3]:
                conll[i][4]=conll[i][4].replace("acc","nom")
            if conll[i-1][3]=="CONJ" and conll[i-1][1]!="и":
                 conll[i][4]=conll[i][4].replace("acc","nom")

        if padej=="loc":
            if conll[i-1][3]!="PR" and (conll[i-2][3]!="A" and "plen" in conll[i-2][4]):
                if conll[i-1][1]=="больше" or conll[i-1][1]=="меньше":
                    conll[i][4]=conll[i][4].replace("loc","gen")
                else:
                    conll[i][4]=conll[i][4].replace("loc","dat")
                    
        if padej=="nom":
            k=i-1
            while(k>=0):
                if conll[k][3]!="A" and conll[k][3]!="SPRO" and conll[k][3]!="PR":
                    break
                if conll[k][3]=="PR":
                    pr_synt(conll[k][1])
                    str4=find_padej(i)
                    conll[i][4]=conll[i][4].replace(str4,par_mas[0])
                    
                k=k-1

            k=i
            while(k>=0):
                if "nom" in conll[i][4] and ((conll[k][3]=="V" and "imper" in conll[k][4]) or conll[k][2]=="найти"):
                    if (conll[i-1][3]=="CONJ" and conll[i-1][1]!="и") or (conll[i+1][3]=="A" and "brev" in conll[i+1][4]):
                        str4=find_padej(i)
                        conll[i][4]=conll[i][4].replace(str4,"nom")
                        break
                    else:
                        conll[i][4]=conll[i][4].replace("nom","acc")
                        break
                k=k-1
                
        if morph_iskl2[j][1]==morph_iskl2[j][2] and (conll[i][1]==morph_iskl2[j][1] or conll[i][1]==morph_iskl2[j][2]):#множ число и в род падеже
            f_5=0
            if conll[i-1][3]=="CONJ" and conll[i-1][1]!="и":
                conll[i][4]=conll[i][4].replace(padej,"nom")
                f_5=1
            if conll[i-1][3]=="S" and not "loc" in conll[i-1][4]:
                conll[i][4]=conll[i][4].replace(padej,"gen")
                f_5=1
            if conll[i-1][3]=="V" or (conll[i-1][3]=="A" and "plen" in conll[i-1][4] and "pl " in conll[i-1][4]) or conll[i-1][1].isdigit():
                if conll[i-1][3]=="V":
                    conll[i][4]=conll[i][4].replace(padej,"acc")
                if conll[i-1][3]=="A" and conll[i-2][3]!="V":
                    conll[i][4]=conll[i][4].replace(padej,"nom")
                if (i>2) and conll[i-1][1].isdigit() and conll[i-2][3]=="V":
                    conll[i][4]=conll[i][4].replace(padej,"acc")
                if (i<2) and conll[i-1][1].isdigit():
                    conll[i][4]=conll[i][4].replace(padej,"nom")
                    
                conll[i][4]=conll[i][4].replace("sg","pl")
                f_5=1
                
            h=i
            while(h<len(conll) and f_5==0):
                if ((conll[h][3]=="A" or conll[h][3]=="PARTCP") and "brev" in conll[h][4]) or conll[h][3]=="V":
                    if find_num(h)=="pl":
                        conll[i][4]=conll[i][4].replace("sg","pl")
                        conll[i][4]=conll[i][4].replace(padej,"nom")
                        break
                    else:
                        conll[i][4]=conll[i][4].replace("pl","sg")
                        
                        if conll[h][3]=="A" and "brev" in conll[h][4]:
                            if conll[i-1][3]!="PR" and ( (conll[i-1][3]=="S" or conll[i-1][3]=="SPRO") and not "gen" in conll[i-1][4]):
                                conll[i][4]=conll[i][4].replace(padej,"nom")
                        else:
                            if not "loc" in conll[i-1][4] and i>0:
                                conll[i][4]=conll[i][4].replace(padej,"gen")
                            
                        break

                h=h+1
                        
    for i in range(len(conll)):
        if conll[i][1]=="-":
            if conll[i+1][3]=="S":
                g=i
                while(g>=0):
                    if conll[g][3]=="S":
                        if "pl" in conll[g][4] and (conll[i+2][1]!="и" or conll[i+2][8]=="after_punc"):
                            conll[i+1][4]=conll[i+1][4].replace("sg","pl")
                        break
                    g=g-1
                    
                str4=find_padej(i+1)
                conll[i+1][4]=conll[i+1][4].replace(str4,"acc")
                
        for j in range(len(morph_iskl2)):
            if morph_iskl2[j][0] in conll[i][2]:
                for l in range(0,7):
                    if conll[i][1]==morph_iskl2[j][l]:
                        str4=find_padej(i)
                        
                        if conll[i-1][3]=="CONJ" and conll[i-1][1]!="и":
                            conll[i][4]=conll[i][4].replace("acc","nom")
                            break
                                   
                        change(i,j,l,str4)
                        break

        if conll[i][3]=="CONJ" and conll[i][1]!="и":
            if conll[i+1][3]=="S":
                str4=find_padej(i+1)
                conll[i+1][4]=conll[i+1][4].replace(str4,"nom")
                

        for i in range(len(conll)-1):
             if conll[i-1][3]=="PR" and conll[i][3]=="S" and "nom" in conll[i][4]:
                  pr_synt(conll[i-1][1])
                  str4=find_padej(i)
                  conll[i][4]=conll[i][4].replace(str4,par_mas[0])
                  
             if conll[i][3]=="PARTCP" and "brev" in conll[i][4]:
                 r=i+1
                 while(r<len(conll)):
                     if conll[r][3]!="SPRO" and conll[r][3]!="A" and conll[r][3]!="S":
                         break
                     if conll[r][3]=="S" and conll[r-1][3]!="S":
                         str4=find_padej(r)
                         conll[r][4]=conll[r][4].replace(str4,"acc")
                         
                     r=r+1
                     
             if conll[i][1]=="больше" or conll[i][1]=="меньше":
                 if conll[i+1][3]=="SPRO" or conll[i+1][3]=="S" or conll[i+1][3]=="A":
                     str4=find_padej(i+1)
                     conll[i+1][4]=conll[i+1][4].replace(str4,"gen")

             if conll[i][1]=="точки" or conll[i][1]=="отрезки":
                 if conll[i][1]=="точки":
                     conll[i][2]="точка"

                 if conll[i][1]=="отрезки":
                     conll[i][2]="отрезок"
                     
                     if conll[i-1][3]=="PR":
                         str4=find_padej(i)
                         conll[i][4]=conll[i][4].replace(str4,"acc")
                
             if conll[i][2]=="отрезка":
                 conll[i][2]="отрезок"
                 if conll[i][1]!="отрезки":
                     conll[i][4]=conll[i][4].replace("pl","sg")
                 else:
                     conll[i][4]=conll[i][4].replace("sg","pl")

             if conll[i][1]=="части":
                 conll[i][4]=conll[i][4].replace("sg","pl")
                 conll[i][4]=conll[i][4].replace("gen","acc")
                 

             if conll[i][3]=="S" and conll[i+1][1]=="-":
                 str4=find_padej(i)
                 conll[i][4]=conll[i][4].replace(str4,"nom")
                     
             if conll[i][2]=="половина":
                 if conll[i+1][3]=="S":
                     str4=find_padej(i+1)
                     conll[i+1][4]=conll[i+1][4].replace(str4,"gen")
                     
             if conll[i][1]=="окружностей":
                 str4=find_padej(i)
                 conll[i][4]=conll[i][4].replace(str4,"gen")
                     
             if "см" in conll[i][1]:
                conll[i][3]="NUM"
                conll[i][4]="NUM - - -"

             if conll[i-1][3]!="PR" and conll[i][3]=="SPRO" and conll[i+1][3]=="S":
                 str4=find_padej(i)
                 conll[i][4]=conll[i][4].replace(str4,"gen")
                 
             if conll[i][3]=="A" and "plen" in conll[i][4] and conll[i+1][3]=="S":
                 str3=find_padej(i)
                 str4=find_padej(i+1)
                 conll[i][4]=conll[i][4].replace(str3,str4)

             if conll[i][2]=="угол":
                 if conll[i+1][2]=="треугольник" or conll[i+1][2]=="четырехугольник" or conll[i+1][2]=="трапеция":
                      str4=find_padej(i+1)
                      conll[i+1][4]=conll[i+1][4].replace(str4,"gen")

             if conll[i][3]=="PARTCP" and "pl " in conll[i][4] and conll[i+1][3]=="S":
                 conll[i+1][4]=conll[i+1][4].replace("sg","pl")
                 str4=find_padej(i+1)
                 conll[i+1][4]=conll[i+1][4].replace(str4,"acc")


        if (conll[1][2]=="угол" and conll[0][3]=="A"):
            str4=find_padej(1)
            conll[1][4]=conll[1][4].replace(str4,"nom")
        
        
        

    return [[str(i+1)]+conll[i][1:10] for i in range(len(conll))]	

