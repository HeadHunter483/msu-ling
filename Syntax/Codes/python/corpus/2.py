import os
import subprocess
import sys
from glob import glob


path = 'D:/Analyz/notstable/ru-syntax-master/test_corpus'
files = os.listdir(path)
i = 1

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))
	
### если нужно переименовать

if __name__ == '__main__':
	mode = sys.argv[1]

if mode=='rename':	
	k=1
	for afile in os.listdir('.'):
		filename, file_extension = os.path.splitext(afile)
		if file_extension == '.tgt':
			os.rename(afile, str(k) + '.xml')
			k=k+1

if mode=='corp':		
    x=0
    for files in os.listdir(path):
        if files.endswith('.xml'):
            x+=1

    k=1
    x=x+1   
    while(k<x):
        print('python 3.py corp '+str(k)+'.xml')
        subprocess_cmd('python 3.py corp '+str(k)+'.xml')
        k=k+1
	
    y=1

    while(y<x):
        os.remove(str(y)+'.xml')
        y=y+1
	
    print('finished!')
