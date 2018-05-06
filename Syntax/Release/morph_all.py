import sys
import codecs
import os
import re
from utilites.num import replacenum
from utilites.skleika import skleika
from utilites.morph import morph
from utilites.morph_corrector import morph_corrector

path = os.path.dirname(os.path.abspath(__file__))

def print_on_screen(arr1):
        arr1=list(arr1[0])
        for i in range(len(arr1)):
                print('\t'+arr1[i][0]+'\t'+arr1[i][1]+'\t'+arr1[i][2]+'\t'+arr1[i][3]+'\t'+arr1[i][4]+'\t')
        

def morph_all(probl, pr=True):
        morphs=[]
        success=1    
        sentences = [' '.join(s.split())+' .' for s in probl.split(' .') if len(s.split())>0]

        if pr:
                print('\n=============== Исходные предложения ================\n')
                for s in sentences:
                        print(s)
                        
        try:
                nums = [replacenum(s) for s in sentences]
        except:
                success=0
                
        if success==1:
                try:
                        morphs = [morph(s) for s in nums]
                except:
                        success=0
        if success==1:
                if pr:
                        print('\n==================== Морфология =====================\n')
                        print_on_screen(morphs)

        if success==1:
                try:
                        skls = [skleika(m) for m in morphs]
                except:
                        success=0

        if success==1:
                try:
                        rigmor = [morph_corrector(m) for m in skls]
                except:
                        success=0
        if success==1:
                if pr:
                        print('\n=================== После исправления ===================\n')
                        print_on_screen(rigmor)
                
                print('\n=========================================')
                return rigmor

        if success==0:
                print('Произошла ошибка при морфологическом анализе\n')
                return 'error\n'
       
                

def morph_io(fin, fout, pr=True):
        f = open(path+"\\tmp\\"+fin,"r",encoding="utf-8")
        problems = [line for line in f if len(line.split())>0]
        f.close()
        f=codecs.open(path+"\\tmp\\"+fout,"w","utf-8")
        res = [morph_all(p,pr) for p in problems]

        if res!=['error\n']:
                for i in range(len(res)):
                        for s in res[i]:
                                for j in range(len(s)):
                                        f.write(str(j+1)+'\t'+'\t'.join(s[j][1:10])+'\n')
                                f.write('\n')
                        if i<len(res)-1:
                                f.write('================================================\n')
                        else: f.write('\n')
                f.close()
        else:
                f.write('error\n')


if __name__ == "__main__":
        if len(sys.argv)<=2:
                print("usage : python syntax_all.py <input file> <output file>")
                exit(1)
        morph_io(sys.argv[1],sys.argv[2], sys.argv[3]>0 if len(sys.argv)>3 else True)
