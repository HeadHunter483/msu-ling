import re
import codecs
import subprocess
import os
import sys
from glob import glob
from extract_corp import zapusk

path = os.path.dirname(os.path.abspath(__file__))
files = os.listdir(path+"\\Texts\\")

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))
	
def rename():
    os.chdir(path+"\\Texts\\")
    k=1
    for afile in os.listdir('.'):
        filename, file_extension = os.path.splitext(afile)

        if file_extension == '.tgt':
            os.rename(afile, str(k) + '.xml')
            k=k+1
            
    os.chdir(path)

def corp():
    x=0
    for files in os.listdir(path+"\\Texts\\"):
        if files.endswith('.xml'):
            x+=1

    print(x)
    k=1
    x=x+1   
    while(k<x):
        zapusk(str(k)+'.xml')
        k=k+1
	
    #y=1
    #while(y<x):
        #os.remove(str(y)+'.xml')
        #y=y+1
	
    print('finished!')

rename()
corp()	

























#subprocess_cmd('python 1_2.py corp')

#print('Make test ? Y/N')
#text=input()

#if (text=='N'):
	#sys.exit()
#if (test=='Y'):
	#subprocess_cmd('python 4.py ')
	
