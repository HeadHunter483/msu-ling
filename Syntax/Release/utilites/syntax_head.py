import os
import re
import sys
import codecs
import subprocess
import time
from utilites.syntax_hand import syntax_hand 

def syntax(morph, pr=True):
        path = os.path.dirname(os.path.abspath(__file__))
        conj=['а','если','что','когда']
        conll=[]
        fictive_temp=[]
        aver_temp=[]

        type1=0
        type2=0

        for i in range(len(morph)):
                if morph[i]==['\n']:
                        continue
                conll.append(morph[i])

        
        conj_index=0
        
        def find_case(index,arr): # в слове с указанным индексом ищем падеж
                arr_cases=['nom','gen','dat','acc','ins','loc']
                s=''
                for i in range(6):
                        if arr_cases[i] in arr[index][4]:
                                s=arr_cases[i]
                                break
                        
                return s

       

        def func_1(start,limit,conll,aver_temp): #разбор простых предложений или 2-х простых связанных союзом
                tmp=[]
                verb_flag=0
                index_conj=0

                #if conll[limit][8]=="before_punc":
                        #fin=limit
                        #fin=limit+
                #else:
               
                ######### Если предложение содержит несколько потенциальных корней , которые связаны союзом "И" то будем разбирать их отдельно
                
                for i in range(start,limit):
                        if conll[i][3]=="CONJ" and conll[i][8]!="after_punc":
                               index_conj=i
                               for k in range(start,index_conj):
                                       if conll[k][3]=="V" or (conll[k][3]=="A" and "brev" in conll[k][4]):
                                               verb_flag=0.5
                                               break
                                        
                               for l in range(index_conj,limit):
                                        if conll[l][3]=="V" or (conll[l][3]=="A" and "brev" in conll[l][4]):
                                               verb_flag=verb_flag+0.5
                                               break

                fict_fl=0
                for i in range(start,limit):
                        if conll[i][3]=="V" or (conll[i][3]=="A" and "brev" in conll[i][4]) or (conll[i][3]=="PARTCP" and "brev" in conll[i][4]) or conll[i][1]=="-":
                                fict_fl=1
                                break

                if fict_fl==0:
                        line=str(len(conll))+'\tсуществовать'+'\tсуществовать'+'\tV'+'\tV ipf - - inf - -'+'\t-'+'\t'+'0'+'\tfictive_root'+'\tbefore_punc'+'\t_\n'
                        line=line.split('\t')
                                               
                ####### предложение простое . Предполагаемый корень 1 ######
                                        
                if verb_flag!=1 and verb_flag!=2:
                      ##### вторая часть съедается если с союза

                      if conll[start][3]=="CONJ" or conll[limit][8]=="before_punc":
                              limit=limit+1

                      for i in range(start,limit):
                              tmp.append(conll[i])

                      tmp=syntax_hand(tmp)
                      
                      for m in range(len(tmp)):
                              aver_temp.append(tmp[m])

                ######### предполагаемых корней 2 #########################

                if verb_flag==1:
                       limit=limit+1
                       
                       for i in range(start,index_conj):
                               tmp.append(conll[i])

                       tmp=syntax_hand(tmp)
                      
                       for m in range(len(tmp)):
                              aver_temp.append(tmp[m])

                       tmp.clear()

                       for i in range(index_conj,limit):
                               tmp.append(conll[i])

                       tmp=syntax_hand(tmp)
                      
                       for m in range(len(tmp)):
                              aver_temp.append(tmp[m])

                       tmp.clear()


                        
                
        def func_2(start,finish,conll,aver_temp): # простое предложений + оборот + конец.
                index=0
                end=0
                count=0
                fl=0
                word1=''                
                tmp=[]
                leng=len(aver_temp)
                flag_oborot=0
                
                #if (conll[finish][8]!="before_punc" and conll[finish][3]!="CONJ"):
                        #end=finish+1
                #else:
                end=finish+1

                ######### Запустили синтаксический анализ для каждой части. Потом собрали ###########
                        
                for i in range(start,len(conll)):
                        if conll[i][5]=="start_oborot":
                                index=i
                                word1=conll[i][1]
                                conll[i][7]="root"
                                conll[i][6]="0"
                                break
                        tmp.append(conll[i])

                tmp=syntax_hand(tmp)

                for i in range(len(tmp)):
                        aver_temp.append(tmp[i])
        
                tmp.clear()
                
                for i in range(index,end):
                        tmp.append(conll[i])  

                tmp=syntax_hand(tmp)

                for i in range(len(tmp)):
                        aver_temp.append(tmp[i])
        
                tmp.clear()

                
                ################# Перенумеровали  #######################################

                for i in range(len(aver_temp)):
                        if fl==0 and aver_temp[i][1]!=word1:
                                count=count+1

                        if aver_temp[i][1]==word1:
                                if aver_temp[i][3]=="PARTCP" and "plen" in aver_temp[i][4]:
                                        str4=find_case(i,aver_temp)
                                        
                                        j=i
                                        while(j>0):
                                                if aver_temp[j][3]=="S" and str4 in aver_temp[j][4]:
                                                        aver_temp[i][6]=str(j+1)
                                                        aver_temp[i][7]="opred"
                                                        break
                                                j=j-1

                                if aver_temp[i][2]=="который":
                                        if "f " in aver_temp[i][4]:
                                                str4="f "

                                        if "m " in aver_temp[i][4]:
                                                str4="m "

                                        j=i
                                        while(j>0):
                                                if aver_temp[j][3]=="S" and ("acc" in aver_temp[j][4]):
                                                        aver_temp[i][6]=str(j+1)
                                                        aver_temp[i][7]="opred"
                                                        break
                                                j=j-1

                                
                                fl=1

                        if fl==1:
                                if aver_temp[i][6]=="_":
                                        aver_temp[i][6]="0"
                                if aver_temp[i][1]==word1 or aver_temp[i][2]=="который":
                                        aver_temp[i][0]=str(int(aver_temp[i][0])+count)
                                        continue
                                
                                aver_temp[i][6]=str(int(aver_temp[i][6])+count)
                                aver_temp[i][0]=str(int(aver_temp[i][0])+count)
                                

                fl=0
                count=0

                
                

        def func_3(start,finish,conll,aver_temp): # простое + оборот + конец простого
                str4=""
                buff=[]
                tmp=[]
                index_stop=0
                index=0
                flag=0
                word=''
                word_after=''
                fl=0
                count=0

                for i in range(start,finish):
                        if conll[i][5]=="start_oborot":
                                index=i
                                word=conll[i][1]
                                word_after=conll[i+1][1]
                                break

                j=index
                
                ####################################
                
                while(flag!=1): #оборот в массив tmp
                        if (conll[j][5]=="end_oborot_zap" or conll[j+1][8]=="after_punc") and conll[j-2][3]!=conll[j-1][3]:
                                index_stop=j
                                flag=1
                                
                                
                        tmp.append(conll[j])
                        j=j+1

                ###################################
         
                i=start
                while(i<index): # в buff остальное предложение
                        buff.append(conll[i])
                        i=i+1

                i=index_stop+1
                while(i<len(conll)):##поедает последний элемент
                        if conll[i][8]=="before_punc" or conll[i][8]=="end_oborot_zap":
                                break
                        buff.append(conll[i])
                        i=i+1

                ####################################

                buff=syntax_hand(buff)

                for i in range(len(buff)):
                               aver_temp.append(buff[i])
                       
                tmp=syntax_hand(tmp)

                for i in range(len(tmp)):
                               aver_temp.append(tmp[i])
                       
                for i in range(len(punc)):
                               aver_temp.append(punc[i])
                               
              
                for i in range(len(aver_temp)):
                        if aver_temp[i][1]==word:
                                if aver_temp[i][3]=="PARTCP" and "plen" in aver_temp[i][4]:
                                        str4=find_case(i,aver_temp)
                                        
                                        j=i
                                        while(j>0):
                                                if aver_temp[j][3]=="S" and str4 in aver_temp[j][4]:
                                                        aver_temp[i][6]=str(j+1)
                                                        aver_temp[i][7]="opred"
                                                        break
                                                j=j-1

                      
        ###########################  Делаем разметку предложения  ##################################
        
        start=0
        finish=0
        
        for i in range(start,len(conll)):
               if (conll[i][8]=="after_punc" or conll[i][8]=="before_punc"): ## предл1 , союз(conj) предл2     
                       if ((conll[i][8]=="after_punc" and conll[i-1][3]!=conll[i][3]) or conll[i][8]=="before_punc"):
                               j=i
                               while((conll[j][8]=="after_punc" and conll[j-1][3]!=conll[j][3] ) or conll[j][8]=="before_punc"):
                                        finish=j
                                        j=j+1

                               if (conll[start][3]=="PARTCP" and "plen" in conll[start][4]) or conll[start][3]=="CONJ":     
                                        if conll[start][3]=="CONJ":
                                                conll[start-1][5]="end_sent_zap"
                                                conll[start][5]="start_new_sent"
                                        else:
                                                conll[start][5]="start_oborot"

                               if conll[finish][8]=="after_punc":
                                        fl_vloj_oborot=0
                                        if conll[start][3]=="CONJ":
                                                conll[finish-1][5]="end_sent_zap"
                                        else:
                                                m=finish-1
                                                while(m<len(conll)):
                                                        if (conll[m][3]=="PARTCP" and "plen" in conll[m][4]) or conll[m][2]=="который":
                                                                fl_vloj_oborot=1
                                                        if conll[m][8]=="before_punc":
                                                                conll[m][5]="end_oborot_dot"
                                                                break
                                                        m=m+1

                                                if fl_vloj_oborot==0:
                                                        conll[finish-1][5]="end_oborot_zap"

                               if conll[finish-1][8]=="after_punc":
                                        if conll[start][3]=="CONJ":
                                                conll[finish-2][5]="end_sent_zap"
                                        else:
                                                conll[finish-2][5]="end_oborot_zap"
                                                                       
                               if conll[finish][8]=="before_punc" and conll[finish-1][8]!="after_punc":
                                        if conll[start][3]=="CONJ":
                                                conll[finish][5]="end_sent_dot"
                                        else:
                                                conll[finish][5]="end_oborot_dot"

                               ######################  ##################################### #############
                               l=finish
                               fl_kot=0
                               while(l<len(conll)):
                                       if conll[l][2]=="который":
                                               conll[finish][5]="start_oborot"

                                       if l!=finish and conll[l][8]=="after_punc" or conll[l][8]=="before_punc":
                                               fl_kot=1
                                               break
                                       l=l+1

                               if fl_kot==0:
                                       conll[len(conll)-2][8]="before_punc"

                               start=finish
       
        ##############################  В зависимости от разметки запускаем различные методы анализа ############

        index=0
        punc=[]
        g=0

        find_root=0

        for i in range(len(conll)):

              ####### Сохраняем знаки препинания ###########
                
              if conll[i][1]=="," or conll[i][1]=="." or conll[i][1]=="?":
                      punc.append(conll[i])

              #################  Первый прогон ################

              if conll[i][3]=="V" or ((conll[i][3]=="A" or conll[i][3]=="PARTCP") and "brev" in conll[i][4]): #### Есть ли сказуемое до запятой
                      find_root=1

              if (conll[i][8]=="after_punc" or conll[i][8]=="before_punc") and index==0:
                      
                      ######## вводное слово #########
                      
                      if conll[i][1]=="зная":
                              conll[i][7]="vvodn"
                              finish=i+1
                              continue
                        
                     #################################
                      
                      if conll[i][8]=="after_punc": ## если части речи одинаковые
                              if conll[i][3]!="CONJ" and conll[i-1][3]==conll[i][3]:
                                      continue
                              if conll[i][3]=="CONJ" and conll[i][8]=="after_punc":
                                      conll[i][5]="start_new_sent"

                      if conll[i][5]=="start_new_sent":
                              t=i
                              while(t<len(conll)):
                                      if conll[t][5]=="start_oborot":
                                              i=i+1
                                              break
                                      t=t+1
                                      
                              zap=0 ## запоминаем слово после запятой
                              for m in range(0,i):
                                      if conll[m][8]=="after_punc" and conll[m][3]=="S":
                                              zap=m
                                              fictive_temp.append(zap)
                                              
                                      if conll[m][2]=="который":
                                              break

                              func_1(0,zap,conll,aver_temp)
                              func_1(zap,i,conll,aver_temp)
                              finish=i
                              index=1
                              continue
                                              
 
                      if conll[i][5]=="start_oborot":
                              k=i
                              while(k<len(conll)):
                                    if conll[k][3]=="V" or conll[k][3]=="A" and "brev" in conll[k][4]:
                                        conll[k][7]="podch-soyuzn"
                                    if find_root==1 and conll[k][5]=="end_oborot_dot" or conll[k][5]=="end_oborot_zap":   
                                            func_2(0,k,conll,aver_temp)
                                            finish=k+1
                                            index=1
                                            break
                                    if find_root==0 and conll[k][5]=="end_oborot_zap" and conll[k+1][5]!="start_new_sent":
                                            y=k
                                            while(y<len(conll)):
                                                    if conll[y][8]=="before_punc" or conll[y][8]=="after_punc":
                                                            func_3(0,y,conll,aver_temp)
                                                            finish=y
                                                            index=1
                                                            break
                                                    y=y+1
                                            break
               
                                    k=k+1
        
                              continue


                      if conll[i][5]=="end_oborot_zap":
                              if conll[i+1][3]=="CONJ":
                                      func_2(0,i,conll,aver_temp)
                              else:
                                      func_3(0,i,conll,aver_temp)

                              finish=i
                              index=1
                              continue

                      if (conll[i][5]!="end_oborot_dot" and conll[i][5]!="end_oborot_zap" and conll[i][8]=="after_punc" and conll[i][3]!="PARTCP"):
                              func_1(0,i,conll,aver_temp)
                              finish=i
                              index=1
                              continue
                

                      if conll[i][8]=="before_punc" or conll[i][5]=="end_oborot_dot":
                              func_1(0,len(conll)-1,conll,aver_temp)
                              break
              
              
              if index==1 and g!=1 and conll[finish][5]!="start_new_sent" and conll[finish][5]!="end_oborot_zap" and conll[finish][5]!="end_sent_zap" and conll[finish][8]=="after_punc"and conll[finish][3]!="PARTCP" and conll[finish][3]!="V":
                    t=finish
                    f_3=0
                    while(t>=0):
                            if conll[t][8]=="after_punc":
                                    if conll[t][3]=="PARTCP" and "plen" in conll[t][4]:
                                            f_3=1
                                            break
                            t=t-1

                    if f_3==0:
                            fictive_temp.append(finish) ## добавляем фиктивный узел ##
                            
                    finish2=0
                    simp=0
                    m=finish+1
                    
                    while(m<len(conll)):
                            if (conll[m][8]=="after_punc"  and conll[m][5]=="start_new_sent" ) or (conll[m][8]=="after_punc" and conll[m][5]!="end_oborot_zap" and conll[m][5]!="end_sent_zap"):
                                        if conll[m-1][3]==conll[m][3]:
                                                m=m+1
                                                continue

                                        finish2=m
                                        break

                            if (conll[m][3]=="PARTCP") and "plen" in conll[m][4] and conll[m][8]=="after_punc":
                                    conll[finish][5]="start_oborot"
                                    g=1
                                    simp=1
                                    break

                            if conll[m][8]=="before_punc":
                                    finish2=m
                                    break
                                
                            m=m+1

                    if simp==0:
                            func_1(finish,finish2,conll,aver_temp)
                            finish=finish2
                            simple_flag=1
                            continue
                        
                      
              if (index==1 and (conll[finish][5]=="start_new_sent" or (conll[finish][3]=="V" and conll[finish][8]=="after_punc"))):
                      fl_2=0
                      ind=0

                      u=finish+1
                      while(u<len(conll)):
                              if conll[u][5]=="end_sent_zap":
                                      if (conll[u][3]=="NUM" or conll[u][1]=="=" or conll[u][3]=="NONLEX") and conll[u+1][3]!="CONJ":
                                              u=u+1
                                              continue
                                      
                                      fl_2=0
                                      ind=u 
                                      break

                              if fl_2==0 and conll[u][8]=="after_punc" and conll[u-1][3]!=conll[u][3] and conll[u][3]!="PARTCP" and conll[u][3]!="CONJ":
                                     ind=u-1
                                     break
                                
                              if conll[u][5]=="end_oborot_zap":
                                      if conll[u+1][3]=="V":
                                              conll[u+1][5]="start_new_sent"
                                              u=u+1
                                              fl_2=0
                                              continue
                                              
                                      r=u+1
                                      while(r<len(conll)):
                                              if conll[r][3]=="CONJ" and conll[r+1][3]=="V":
                                                      conll[r][5]="end_sent_zap"
                                                      conll[r+1][5]="start_new_sent"
                                                      break
                                              r=r+1
                                              
                                      fl_2=2
                                      ind=u
                                      break

                              if conll[u][8]=="before_punc" or conll[u][5]=="end_oborot_dot":
                                      if conll[u+1][1]=="-":
                                              conll[u][8]="-"
                                              conll[u+2][8]="before_punc"
                                              u=u+1
                                              continue
                                      fl_2=0
                                      ind=u
                                      break
                                                                      
                              u=u+1


                      if fl_2==0: # предложение начинающееся с союза без оборотов
                              func_1(finish,ind,conll,aver_temp)
                              finish=ind+1
                              index=1
                              continue
                                     

                      if fl_2==3: # сложное предложение начинающееся с союза + оборот
                              func_3(finish,ind,conll,aver_temp)
                              finish=ind
                              index=1
                              continue
                                                                                            
                      if fl_2==2:
                              func_2(finish,ind,conll,aver_temp)
                              finish=ind
                              index=1
                              continue
                        
        if conll[finish][3]!="SENT" and conll[finish][8]!="before_punc":
                f_7=0
                while(finish<len(conll)-len(punc)):
                        r=finish
                        while(r<len(conll)-len(punc) or conll[r][8]=="before_punc"):
                                if f_7==0 and conll[r][8]=="before_punc":
                                        func_1(finish,len(conll)-len(punc),conll,aver_temp)
                                        finish=r+1
                
                                if conll[r][5]=="start_new_sent":
                                        f_7=1
                                        break
                                r=r+1

                        if f_7==1:
                                if conll[r][3]=="CONJ" and conll[finish-1][3]!="CONJ":
                                       func_1(finish-1,r,conll,aver_temp)                    
                                else:
                                       func_1(finish,r,conll,aver_temp)
                               
                                finish=r+1
        
        ########## Cборка всех кусочков и финальный пересчет #########

        leng=0
        fl=0
        for j in range(len(aver_temp)):
                if aver_temp[j][3]=="IDIOM" and aver_temp[j][8]=="after_punc":
                        aver_temp[j][6]=str(int(aver_temp[j][6])+int(aver_temp[j][0])-1)
                           
                if aver_temp[j][0]=="1" and j!=0:
                        leng=int(aver_temp[j-1][0])
                        fl=1
                        
                if fl==1:
                   if aver_temp[j][6]=="_":
                           aver_temp[j][6]="0"
                           aver_temp[j][7]="wrong_link"

                   
                   if aver_temp[j][5]=="start_oborot" and aver_temp[j][3]=="PARTCP" and "plen" in aver_temp[j][4]:
                           continue

                   
                   aver_temp[j][0]=str(int(aver_temp[j][0])+leng)
                   aver_temp[j][6]=str(int(aver_temp[j][6])+leng)


        ################## Союзы и корни ##########################

        root_count=0
        main_root=0

        for i in range(len(aver_temp)):
                if aver_temp[i][3]=="PARTCP" and "plen" in aver_temp[i][4] and aver_temp[i][8]=="after_punc":
                        if aver_temp[i-1][3]=="S":
                                str3=find_case(i,aver_temp)
                                str4=find_case(i-1,aver_temp)
                                aver_temp[i][4]=aver_temp[i][4].replace(str3,str4)
                                
                                aver_temp[i][6]=str(i)
                                aver_temp[i][7]="opred"
                                
                        if aver_temp[i][2]=="описать" or aver_temp[i][2]=="вписать":
                                str4=find_case(i,aver_temp)
                                aver_temp[i][4]=aver_temp[i][4].replace(str4,"gen")

                        str4=find_case(i,aver_temp)
                        r=i
                        while(r>=0):
                                if aver_temp[r][3]=="S":
                                        if aver_temp[i][2]=="описать" or aver_temp[i][2]=="вписать":
                                                aver_temp[i][6]=str(r+1)
                                                aver_temp[i][7]="opred"
                                                break
                                        if str4 in aver_temp[r][4]:
                                                aver_temp[i][6]=str(r+1)
                                                aver_temp[i][7]="opred"
                                                break
                                        
                                r=r-1

                if aver_temp[i][3]=="PARTCP" and aver_temp[i][6]=="_":
                        t=i
                        while(t>=0):
                                if conll[t][3]=="S":
                                        str4=find_case(t,aver_temp)
                                        str3=find_case(i,aver_temp)
                                        aver_temp[i][4]=aver_temp[i][4].replace(str3,str4)
                                        aver_temp[i][6]=str(t+1)
                                        aver_temp[i][7]="opred"
                                        break
                                t=t-1

                ################ ... который равно --> ищем слово в соотв падеже перед оборотом ##########
                                
                if aver_temp[i][2]=="который":
                        if aver_temp[i][8]!="after_punc":
                                aver_temp[i][6]=str(i)
                                aver_temp[i][7]="opred"
                        if aver_temp[i+1][7]=="root":
                                r=i
                                while(r>=0):
                                        if aver_temp[r][8]=="after_punc":
                                                fictive_temp.append(r)
                                                break
                                        r=r-1
                               
                                        
                #############################################################################
                                                
                if (i<len(aver_temp)-1) and aver_temp[i][3]=="ADV" and aver_temp[i+1][3]=="CONJ" and aver_temp[i+1][1]!="и": ## известно , что
                        aver_temp[i][7]="root"
                        aver_temp[i][6]="0"
                        
                if aver_temp[i][7]=="root":
                        #### вводное слово ####
                        if "зная" in aver_temp[i][1]:
                                aver_temp[i][7]="vvodn"

                        #### если стоит отметка root , но родитель не 0 .
                        if aver_temp[i][6]!="0":
                                aver_temp[i][6]="0"

                        ### в сложных предложениях несколько корней. Выделяем главный ###
                        if root_count==0:
                                main_root=i+1
                                root_count=root_count+1

                        if root_count!=0:
                                k=i
                                while(k>0):
                                        if aver_temp[k][3]=="CONJ" and aver_temp[k][1]!="и":
                                                if aver_temp[i][7]!="vvodn":
                                                        aver_temp[i][7]="podch-soyuzn"
                                                        
                                                if aver_temp[k][1]=="а":
                                                        aver_temp[k][7]="sent-soch"
                                                        aver_temp[i][7]="soch-soyuzn"
                                               
                                                aver_temp[i][6]=str(k+1)
                                                break

                                        k=k-1

                                        #### 1 корень главный. Остальные зависимые. Присоединяются либо к союзу либо к главному корню ##
                                        if i!=main_root-1:
                                                aver_temp[i][6]=str(main_root)
                                                aver_temp[i][7]="podch-soyuzn"
                                
                
                                                
                ### связываем корни и союзы ###
                if aver_temp[i][1]=="и" and aver_temp[i+1][7]=="root":
                        aver_temp[i+1][6]=str(i+1)
                        aver_temp[i+1][7]="soch-soyuzn"
                        
                        j=i-1
                        while(j>=0):
                                if (aver_temp[j][7]=="sent-soch" or aver_temp[j][7]=="podch-soyuzn" or aver_temp[j][7]=="root"):
                                        aver_temp[i][6]=str(j+1)
                                        break
                                j=j-1
                        
                if (aver_temp[i][3]=="CONJ" and aver_temp[i][1]!="и"):
                        j=i
                        while(j>=0):
                                if (aver_temp[j][7]=="sent-soch" or aver_temp[j][7]=="podch-soyuzn" or aver_temp[j][7]=="root"):
                                        aver_temp[i][6]=str(j+1)
                                        break
                                j=j-1

                                
        for i in range(len(aver_temp)-1):
                ######### если встретили слово первый второй без привязки к другому существительному. ищем его выше ###
                if aver_temp[i][1]=="1" and aver_temp[i+1][1]=="из":
                        aver_temp[i][6]=str(int(aver_temp[i][6])+int(aver_temp[i-1][0]))

                ######### если подлежащее определено но родитель 0 -- ищем родителя ###

                if aver_temp[i][7]=="predic" and aver_temp[i][6]=="0":
                        t=i
                        while(t<len(aver_temp)):
                                if aver_temp[t][7]=="root" or aver_temp[t][7]=="podch-soyuzn":
                                        aver_temp[i][6]=str(t+1)
                                        break
                                t=t+1

                ### наречие + союз  etc известно , что или если что ######
                if (aver_temp[i][3]=="ADV" or aver_temp[i][3]=="CONJ" ) and aver_temp[i+1][3]=="CONJ":
                        aver_temp[i+1][6]=str(i+1)

                ###### для слова провести ищем слово к котрому его можно присоединить по смыслу ####
                if aver_temp[i][3]=="PARTCP":
                       if aver_temp[i][2]=="провести":
                        h=i
                        while(h>=0):
                                if conll[h][2]=="высота":
                                        aver_temp[i][6]=str(h+1)
                                        break
                                h=h-1

                ##### части союза по разным частям предложения ###
                if aver_temp[i][1]=="если":
                        y=i
                        while(y<len(aver_temp[y][1])):
                                if aver_temp[y][1]=="то":
                                        aver_temp[y][6]=str(i+1)
                                        break
                                y=y+1
       
        if len(fictive_temp)>0:
                for i in range(fictive_temp[0],len(aver_temp)):
                        parent_for_fictive=0
                        if aver_temp[i][7]=="podch-soyuzn":
                               k=i-1
                               while(k>=0):
                                       if aver_temp[k][7]=="root" or aver_temp[k][7]=="podch-soyuzn" or aver_temp[k][7]=="sent-soch":
                                               parent_for_fictive=k+1
                                               break
                                       
                                       k=k-1

                               line=str(len(aver_temp))+'\tи'+'\tи'+'\tCONJ'+'\tCONJ'+'\tCONJ'+'\t'+str(k+1)+'\tfictive_node'+'\t_'+'\t_\n'
                               line=line.split('\t')
                               aver_temp.append(line)
                               aver_temp[i][6]=str(len(aver_temp))
                               aver_temp[i][7]="sent-soch"
                      
        
        ################### Знаки препинания #######################

        aver_temp=aver_temp+punc
      
        for i in range(len(aver_temp)):
                if aver_temp[i][8]=="after_punc":
                        k=i
                        while(k<len(aver_temp)):
                                if aver_temp[k][1]=="," and aver_temp[k][8]!="pass":
                                        aver_temp[k][8]="pass"
                                        aver_temp[k][6]=str(i)
                                        aver_temp[k][7]="punc"
                                        break
                                k=k+1
                                
                if aver_temp[i][8]=="before_punc":
                        k=i
                        while(k<len(aver_temp)):
                                if aver_temp[k][1]==".":
                                        aver_temp[k][6]=str(i+1)
                                        aver_temp[k][7]="punc"
                                        break
                                k=k+1

        ######## проверяем есть ли знак окончания предложения в конце ###

                if aver_temp[i][6]=="_" or aver_temp[i][7]=="_":
                        aver_temp[i][6]=str(i-1)
                        aver_temp[i][7]="wrong_link"

        if aver_temp[len(aver_temp)-1][1]!='.' or aver_temp[len(aver_temp)-1][1]=="?":
               aver_temp.insert(len(aver_temp),conll[len(conll)-1])
               aver_temp[len(aver_temp)-1][6]=str(len(aver_temp)-1)
               aver_temp[len(aver_temp)-1][7]="punc"
                
        
        #############################################################

        conll.clear()
        conll=aver_temp
        
        result = [[str(i+1),conll[i][1],conll[i][2],conll[i][3],conll[i][4],conll[i][6],conll[i][7]] for i in range(len(conll))]
 
        return result
    
