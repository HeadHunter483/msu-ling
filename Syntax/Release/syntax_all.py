import os
import sys
import codecs
from utilites.syntax_head import syntax

path = os.path.dirname(os.path.abspath(__file__))

def sep(l):
	return len(l.replace('=',' ').split())==0

def syntax_io(fin, fout, pr=True , *, graph_num):
        morph = []
        graph = []
        syntax_arr = []
        line=''
        success=1
        
        f = open(path+"\\tmp\\"+fin,"r",encoding="utf-8")
        out = open(path+"\\"+fout,"a",encoding="utf-8")
        out2 = open(path+"\\semantic.txt","a",encoding="utf-8")
        out3 = open(path+"\\test.txt","a",encoding="utf-8")
       
        for line in f:
              row=line.split('	')
              morph.append(row)
        f.close()

        
        print('\n============== Синтаксический разбор ================')
        print('\nПредложение:')

        for i in range(len(morph)):
                if morph[i]==['\n'] or morph[i][1]=="," or morph[i][1]=="." or morph[i][1]=="?" or morph[i][1]=="!":
                        continue
                                        
                if i==0:
                        line=morph[i][1]
                        line=line.title()
                else:
                        line=line+' '+morph[i][1]

                if morph[i+1][8]=="after_punc":
                        line=line+','
                if morph[i][8]=="before_punc":
                        line=line+' . '
                
        print(line+'\n')
        out.write(line+'\n')   

        if len(morph)>0:
                i=0
                try:
                        syntax_arr=syntax(morph, pr)
                except:
                        syntax_arr=['error\n']
                        success=0
                
                if success==1:        
                        for s in syntax_arr:
                                i=i+1
                                out.write(str(i)+'\t'+'\t'.join(s[1:])+'\n')
                                out2.write(str(i)+'\t'+'\t'.join(s[1:])+'\n')
                                out3.write(str(i)+'\t'+s[1]+'\t'+s[3]+'\t'+s[5]+'\t'+s[6]+'\n')
                                graph.append(str(i)+'\t'+s[1]+'\t'+s[3]+'\t'+s[5]+'\t'+s[6]+'\n')

                        for i in range(len(syntax_arr)):
                                print('\t'+syntax_arr[i][0]+'\t'+syntax_arr[i][1]+'\t'+syntax_arr[i][2]+'\t'+syntax_arr[i][3]+'\t'+syntax_arr[i][4]+'\t'+syntax_arr[i][5]+'\t'+syntax_arr[i][6])
                                
                        out.write('\n')
                        out2.write('\n')
                        out3.write('\n')
                        out.close()
                        out2.close()
                        out3.close()

                ######################

                        if graph_num!= None:
                                gr = codecs.open(path+"\\Graphs\\graph"+str(graph_num)+".txt","w","utf-8")
                                for s in graph:
                                        gr.write(s)

                                gr.close()


                #####################
       
                if success==0:
                        out.write('error\n')
                        print('Произошла ошибка при синтаксическом анализе\n')
                        out.close()
        

if __name__ == "__main__":
        if len(sys.argv)<=2:
                print("usage : python syntax_all.py <input file> <output file>")
                exit(1)

        if len(sys.argv)==3:
                syntax_io(sys.argv[1],sys.argv[2], int(sys.argv[3])>0 if len(sys.argv)>3 else True , graph_num=None)

        if len(sys.argv)>3 and len(sys.argv)<=5:
               syntax_io(sys.argv[1],sys.argv[2], graph_num=sys.argv[3])

        
	
