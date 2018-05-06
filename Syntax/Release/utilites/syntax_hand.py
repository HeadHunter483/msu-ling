import os 
path = os.path.dirname(os.path.abspath(__file__))


pr=[]
rules=[] # правила из файла

f2=open(path+"\\..\\tmp\\predlogipadeji.txt","r",encoding="utf-8")

####### Набор параметров ####

part_of_speach=[]
case_param=[]
link=[]
krat=[] 
part_of_speach=['S','SPRO','V','A','PARTCP','PR','ADV','NONLEX','NUM','CONJ','IDIOM','PART']
case_param=['nom','gen','dat','acc','ins','loc']
link=['root','predic','opred','predl','obst','1-kompl','kvaziagent','atrib','kolichest','sochin','idiom_link','ogranich','podch-soyuzn','pass-anal']
krat=['brev','plen']

for line in f2: #считали данные предлогов в массив pr
    row=line.split('	')
    pr.append(row)

f2.close()

def syntax_hand(m):
    conll=[]
    par_mas=[]

    for i in range(len(m)):
        conll.append(m[i])

           
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
        s=''
        
        for i in range(6):
            if case_param[i] in conll[index][4]:
                s=case_param[i]
                break
        
        return s

    def razmetka(index,parent_index,type_of_link):
         conll[index][6]=str(parent_index)
         conll[index][7]=type_of_link

    # "Голова" разбивает предложение на части и каждую часть будем разбирать.

    def base_hand(root):
            #подлежащее
            for i in range(len(conll)):
                    if (conll[i][3]==part_of_speach[0] or conll[i][3]==part_of_speach[1]) and case_param[0] in conll[i][4]:
                        if (conll[root-1][3]==part_of_speach[2] and ("imper" in conll[root-1][4] or conll[root-1][2]=="вычислить")) or (i>root-1 and (conll[root-1][1]=="-" or conll[root-1][1]=="=") or conll[root-1][3]==part_of_speach[4]):
                            conll[i][4]=conll[i][4].replace(case_param[0],case_param[3])
                            break

                        razmetka(i,root,link[1])
                                
                        

            #дополнение 
            for i in range(len(conll)):
                    if conll[i][3]==part_of_speach[0] and case_param[3] in conll[i][4]:
                        if (conll[i][3]==case_param[5]):
                            break
                        
                        razmetka(i,root,link[5])

            #квазиагент род
            for i in range(len(conll)):
                    if conll[i][3]==part_of_speach[0] and case_param[1] in conll[i][4]:
                        if (conll[i][3]==case_param[5]):
                            break

                        k=i-1
                        while(k>=0):
                            
                            if conll[k][7]==link[0]:
                                razmetka(i,root,link[5])
                                break
                            
                            if conll[k][3]=="PR":
                                razmetka(i,k+1,"predl")
                                break
                            
                            if conll[k][3]==part_of_speach[0]:
                                razmetka(i,k+1,link[6])
                                break
		    
                            k=k-1

    	    
            #nonlex
            for i in range(len(conll)):
                        if conll[i][3]==part_of_speach[7]:
                                if conll[i-1][3]==part_of_speach[0]: 
                                    razmetka(i,i,link[7])
                                        
                                if conll[i-1][3]==part_of_speach[0] and conll[i-2][3]==part_of_speach[7]: 
                                    razmetka(i-2,root,link[1])
                                
                        if conll[i][3]==part_of_speach[7] and (len(conll[i][1])>1 or conll[i][1]=="o"):
                            conll[i][1]=conll[i][1].upper()
                            conll[i][2]=conll[i][2].upper()
                        

            #opred
            for i in range(len(conll)-1):
                        if (conll[i][3]==part_of_speach[3] or conll[i][3]==part_of_speach[4]) and krat[1] in conll[i][4]:## полное прилагательное
                                 str4=find_padej(i)
                                 
                                 if conll[i+1][3]==part_of_speach[0]:
                                     razmetka(i,i+2,link[2])
                                 
                                 k=i
                                 while(k<len(conll)):
                                     if conll[k][3]==part_of_speach[0] and str4 in conll[k][4]:
                                         razmetka(i,k+1,link[2])
                                         break

                                     k=k+1

            #num
            for i in range(len(conll)-1):
                    if conll[i][3]==part_of_speach[8]:
                        if (conll[i-1][3]==part_of_speach[0] and case_param[4] in conll[i-1][4]):
                            razmetka(i,i,link[8])
                            break
                            
                        if (conll[i-1][3]==part_of_speach[5] and conll[i+1][3]!=part_of_speach[0]):
                            razmetka(i,i,link[3])
                            break
                                          
                        if conll[i+1][3]==part_of_speach[0]:
                            razmetka(i,i+2,link[8])
                            break
                            
                        if conll[i+1][3]!=part_of_speach[0]  and conll[i+1][3]!=part_of_speach[3]:
                            razmetka(i,root,link[8])
                            break
                                
                        if conll[i][1].isdigit() and conll[i+1][3]==part_of_speach[0]: ##  число + существительное
                            razmetka(i+1,root,link[1])
                            razmetka(i,i+2,link[8])

            #spro
            for i in range(len(conll)):
                        if (conll[i-1][3]!=part_of_speach[5] and conll[i][3]==part_of_speach[1]):
                            if conll[i-1][3]==part_of_speach[5]:
                                razmetka(i,i,link[3])
                                break
                            
                            if conll[i-1][7]==link[0] and conll[i+1][3]!=part_of_speach[0] and case_param[1] in conll[i+1][4]:
                                razmetka(i,root,link[5])
                                break
                                
                            k=i+1
                            while(k<len(conll)):
                                if conll[k][3]==part_of_speach[0]:
                                    razmetka(i,k+1,link[2])
                                    break

                                k=k+1


                    
    def add_etap_1_hand(root):                        
        #pr            
        for i in range(len(conll)):
            if conll[i][3]==part_of_speach[5]:
                pr_synt(conll[i][1])
	      
                if ("root" in par_mas[2]):
                    razmetka(i,root,link[4])
	     
                if ("p" in par_mas[2]):
                    if conll[i-1][3]==part_of_speach[3] or conll[i-1][3]==part_of_speach[4]:
                        razmetka(i,i,link[7])
                    if conll[i-1][3]!=part_of_speach[3] and conll[i-1][3]!=part_of_speach[4]:
                        razmetka(i,root,link[4])
                                
                    k=i
                    while(k>=0):
                        if conll[k][3]==part_of_speach[0]:
                            razmetka(i,k+1,link[7])
                            break

                        k=k-1

	     
                j=i
                while(j<len(conll)):
                    if conll[j][7]==link[0]:
                        break

                    str4=find_padej(j)
                    if (conll[j][3]==part_of_speach[0]) and  (str4==par_mas[0] or str4==par_mas[1]):
                        if (conll[j-1][3]==part_of_speach[1] or conll[j-1][3]==part_of_speach[0]) and case_param[1] in conll[j-1][4]:
                            if conll[j-2][3]==part_of_speach[5]:
                                e=j-1
                                while(e>=0):
                                    if conll[e][3]==part_of_speach[0]:
                                        razmetka(j,e+1,link[6])
                                        break
                                        e=e-1
                                    else:
                                        razmetka(j,j,link[6])
                                        break
                        else:
                                razmetka(j,i+1,link[3])
                                break
		 
                    j=j+1        

        #conj
        for i in range(len(conll)):
                if (conll[i][3]==part_of_speach[9]):
                    if conll[i][1]=="и":
                        razmetka(i,i,link[9])
                    else:
                        razmetka(i,root,link[9])

        #idiom and part
        for i in range(len(conll)):
                if (conll[i][3]==part_of_speach[10]):
                         razmetka(i,i+2,link[10])
                         
                if (conll[i][3]==part_of_speach[11]):
                         razmetka(i,root,link[11])
	     	    

        #adv 
        for i in range(len(conll)):
                if conll[i][3]==part_of_speach[6]:
                    k=i
                    while (k>0):
                        if (conll[k][7]==link[0]):
                            razmetka(i,k+1,link[11])
                            break
                                     
                        k=k-1

                
                     
            
    def add_etap_2_hand(root):
            for i in range(len(conll)):
                if conll[i][6]=="_":
                    if conll[i][3]==part_of_speach[3] and krat[1] in conll[i][4]: ## 2 подряд полных прилагательных
                        if (i<len(conll)-1) and conll[i+1][3]==part_of_speach[3]  and krat[1] in conll[i+1][4]:
                            str4=find_padej(i+1)
                            str3=find_padej(i)
                            conll[i][4]=conll[i][4].replace(str3,str4)
                            razmetka(i,conll[i+1][6],conll[i+1][7])
                        else:
                            razmetka(i,root,link[5])
                                
                    if conll[i][3]==part_of_speach[8]:
                        razmetka(i,root,link[8])
                            
                    
            for i in range(len(conll)-1):
                if (conll[i][7]==link[0] and conll[i][3]==part_of_speach[3] and krat[0] in conll[i][4]):
                    if ((conll[i-1][3]==part_of_speach[7] and conll[i-2][3]!=part_of_speach[0]) or (conll[i-1][3]==part_of_speach[3] and case_param[0] in conll[i-1][4])):
                        razmetka(i-1,i+1,link[1])
   
                    if conll[i+1][3]==part_of_speach[7]:
                        razmetka(i+1,i+1,link[5])

                if conll[i-1][3]==part_of_speach[7]  and conll[i][3]==part_of_speach[7]: ## идут через запятую
                    conll[i][6]=conll[i-1][6]
                    conll[i][7]=conll[i-1][7]

                if ((conll[i+1][3]==part_of_speach[7] and conll[i][1]=="и" and conll[i-1][3]==part_of_speach[7])):
                    conll[i+1][6]=conll[i-1][6]
                    conll[i+1][7]=conll[i-1][7]

                if conll[i][1]=="-" or conll[i][1]=="=":
                    if conll[i+1][3]==part_of_speach[0]:
                        razmetka(i+1,i+1,link[5])

                if i<len(conll)-1 and conll[i][3]==part_of_speach[7] and conll[i+1][3]==part_of_speach[7] and conll[i+1][8]!="after_punc":## два подряд атрибута быть не может
                        conll[i+1][6]=str(root)
                        conll[i+1][7]=link[1]
                        razmetka(i+1,root,link[1])


    def corrector(root):
            for i in range(len(conll)-1):
                if conll[i][3]==part_of_speach[8] and conll[i+1][3]==part_of_speach[3] and krat[1] in conll[i+1][4]:
                        razmetka(i,conll[i+1][6],link[8])
                        break
                            
                if conll[i][3]==part_of_speach[4] and krat[1] in conll[i][4]:
                        if conll[i+1][3]==part_of_speach[5]:
                            razmetka(i+1,i+1,link[4])
                       
                if conll[i][1]=="1" and conll[i+1][1]=="из":
                    f_7=0
                    r=i
                    while(r<len(conll)-1):
                        if conll[r][3]==part_of_speach[3] and krat[0] in conll[r][4]: ## 1 из которых равен ....
                                conll[r][7]=link[0]
                                razmetka(i,r+1,link[1])
                                
                                if conll[r+1][3]==part_of_speach[8]:
                                    razmetka(r+1,r+1,link[8])

                                break
                            
                        r=r+1
                       
            for i in range(len(conll)):
                if conll[i][6]=="_" or conll[i][7]=="_":
                    if conll[i][3]==part_of_speach[6]:
                        razmetka(i,root,link[11])
                             
                    if conll[i][3]==part_of_speach[1]:
                        razmetka(i,i,link[3])
                            
                    if conll[i][3]==part_of_speach[5]:
                        razmetka(i,i,link[3])

                    if conll[i][3]==part_of_speach[0]:
                        razmetka(i,root,link[5])
                            
                    if conll[i][7]==link[0] and conll[i-1][3]==part_of_speach[3] and krat[1] in conll[i-1][4]:
                        razmetka(i-1,i+1,link[5])
                        
                    if conll[i][7]==link[0] and conll[i+1][3]==part_of_speach[3] and krat[1] in conll[i+1][4]:
                       razmetka(i+1,i+1,link[5])
                            
                    if conll[i][1]=="самый":
                        if conll[i+1][3]==part_of_speach[3]:
                            conll[i][6]=str(i+2)
                            continue

                    if conll[i][1]=="как" and conll[i+1][3]==part_of_speach[8]:
                       conll[i+1][6]=str(i+1)

    def math_construction():
        for i in range(len(conll)):
            if conll[i][1]=="=":
                if conll[i-2][3]!=part_of_speach[0] and conll[i-1][3]==part_of_speach[7]:
                    razmetka(i-1,i+1,link[1])

            if conll[i][2]=="каждый":
                if conll[i-1][3]==part_of_speach[0]:
                   razmetka(i,i,link[2])
                        
                if conll[i+1][1]=="из":
                    razmetka(i+1,i+1,link[7])
                        
            if ("градус" in conll[i][2] and conll[i-1][3]==part_of_speach[5]):
                razmetka(i,i,link[3])
                    
            if conll[i][2]=="отношение":
                    if conll[i+1][3]==part_of_speach[8]:
                        razmetka(i+1,i+1,link[8])
                    if conll[i+1][3]==part_of_speach[0]:
                        r=i+1
                        while(r<len(conll)):
                            if conll[r][1]=="к":
                                conll[r][6]=str(i+2)
                                break
                            r=r+1

            if conll[i][2]=="относиться" and (conll[i+1][1]=="как" or conll[i+1][1]=="к") and (conll[i+2][3]==part_of_speach[8]):       
                conll[i+2][6]=str(i+2)
                
                        
            if conll[i][2]=="прямая" and conll[i+1][2]=="угол":
                    conll[i][2]="прямой"
                    conll[i][3]=part_of_speach[3]
                    conll[i][4]="A - acc sg plen m -"
                    razmetka(i,i+2,link[2])
                    if conll[i-1][3]=="PR":
                        razmetka(i+1,i,"predl")
                    
            if (conll[i-1][2]=="радиус" or conll[i-1][2]=="длина") and conll[i][3]==part_of_speach[8]:
                    if (i<len(conll)-1) and "sg" in conll[i+1][4] and conll[i+1][3]==part_of_speach[0]:
                        continue
                    razmetka(i,i,link[8])
                    
                    
            if conll[i][7]=="равный" and krat[1] in conll[i][4]:
                    if conll[i+1][3]==part_of_speach[8]:
                        razmetka(i,i,link[2])
                        
                        r=i+1
                        while(r<len(conll)):
                            if conll[r][3]!=part_of_speach[8] and conll[r][3]!=part_of_speach[9]:
                                break
                            if conll[r][3]==part_of_speach[8]:
                                razmetka(r,i+1,link[8])
                                
                            r=r+1
                    
                    
        for i in range(len(conll)-1):
            if conll[i-2][3]==part_of_speach[5] and conll[i-1][2].isdigit() and conll[i][2]=="раз":
                    razmetka(i,i-1,"predl")

            if conll[i][2]=="окружность":
                    t=i
                    while(t>=0):
                        if conll[t][2]=="радиус":
                            razmetka(i,t+1,link[6])
                            
                            break
                        t=t-1
                
            if conll[i][3]==part_of_speach[4] and (conll[i][2]=="описать" or conll[i][2]=="вписать") and krat[1] in conll[i][4]:
                    if conll[i][2]=="описать":
                        if conll[i+1][2]=="около":
                            conll[i+1][6]=str(i+1)
                            
                    if conll[i][2]=="вписать":
                        if conll[i+1][2]=="в":
                            conll[i+1][6]=str(i+1)
                    t=i
                    while(t<len(conll)):
                        if conll[t][3]==part_of_speach[0] or conll[t][2]=="окружность":
                            if conll[t][2]=="окружность":
                                str4=find_padej(t)
                                razmetka(i,t+1,link[2])         
                                break

                            if case_param[1] in conll[t][4]:
                                if conll[i-1][2]=="радиус":
                                    razmetka(t,i,link[6])
                                
                                    
                                    
                        t=t+1

                    
                                

    flag=0
    root=0
    #если предложение сложное то там уже могут быть размечены корни
    
    for i in range(len(conll)):
        if conll[i][7]==link[0] or conll[i][7]==link[12]:
            conll[i][6]="0"
            root=i+1
            break

        if len(conll)==1: # { Докажите ( Известно } , что
            if conll[0][3]==part_of_speach[2]: ## ---> Докажите -- корень
                razmetka(0,0,link[0])    
                flag=1

        if (flag==0):
            for i in range(len(conll)):      
                    ############  Корни --- > глагол ... краткое прилагательное ... краткое причастие #######
                    if conll[i][3]==part_of_speach[2]:
                            if conll[i-1][3]==part_of_speach[2] and conll[i][8]=="after_punc": # быть начали и т.д
                                razmetka(i,i,link[5])
                                break
                            else:
                                razmetka(i,0,link[0])
                                root=i+1
                                break

                    if (conll[i][3]==part_of_speach[4]) and krat[0] in conll[i][4]: 
                            razmetka(i,0,link[0])
                            root=i+1
                            break
                          
                    if (conll[i][3]==part_of_speach[3]) and krat[0] in conll[i][4]: 
                               razmetka(i,0,link[0])
                               root=i+1
                               break

                    if (conll[i][1]=="-" or conll[i][1]=="="):
                        razmetka(i,0,link[0])
                        root=i+1
                        break

                  
                    if conll[i][7]==part_of_speach[2] and conll[i+1][7]==part_of_speach[2]:
                        razmetka(i+1,i+1,link[13]) ########### сложное сказуемое 2 глагола подряд ######
                        root=i+1

    base_hand(root)## растановка основных связей (общий случай) : opred,predic,atrib,num
    add_etap_1_hand(root) # союзы связи между словами связанными союзами , тире и знак равенства .
    add_etap_2_hand(root)# расстановка связей в сложных конструкциях
    corrector(root)
    math_construction()

    return [[str(i+1)]+conll[i][1:10] for i in range(len(conll))]



