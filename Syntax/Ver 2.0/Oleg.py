import re
import codecs
import subprocess
import os
import sys

path = os.path.dirname(os.path.abspath(__file__))

mas=[]

text = ''.join(open('in.txt',encoding="utf-8").readlines())
word=text.split()

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))


#line="Дан треугольник ABC со сторонами AB и BC ."
#line="дан треугольник abc ."
#line="Дан треугольник ABC ."
line="дан треугольник ABC со сторонами AB и BC ."
#line="Дан треугольник ABC со сторонами AB = 10 и BC = 15 ."
#line="Дан треугольник со сторонами AB и BC , равными 10 и 15 соответственно ."
#line="Дан равнобедренный треугольник ."
#line="Дан равнобедренный треугольник ABC ."
#line="Дан равнобедренный треугольник ABC со сторонами AB и BC ."
#line="Дан равнобедренный треугольник ABC со сторонами AB = 10 и BC = 15 ."
#line="Дан равнобедренный треугольник со сторонами AB и BC, равными 10 и 15 соответственно ."
#line="Даны ромб ABCD и треугольник ABC ."



subprocess_cmd('python converter.py in.txt utf8') 
subprocess_cmd('python auto.py')
subprocess_cmd('python converter.py out.txt utf8') 
f=codecs.open("out.txt","r","utf-8")
rows=[]

for line in f:
    row=line.split('	')
    rows.append(row)

f.close()
f=codecs.open("Oleg_semantic_in.txt","w","utf-8")
f.write(str(len(rows)-1)+'\n')
f.close()


f=codecs.open("Oleg_semantic_in.txt","a","utf-8")
i=0

for i in range(len(rows)-1):
	f.write(str(i+1)+'	'+rows[i][1]+'	'+rows[i][2]+'	'+rows[i][3]+'	'+rows[i][4]+'	'+rows[i][5]+'	'+rows[i][6]+'	'+rows[i][7]+'	'+rows[i][8]+'	'+rows[i][9])
   
    
f.close()
subprocess_cmd('python converter.py Oleg_semantic_in.txt ansi') 
subprocess_cmd('Oleg_semantic.exe')



