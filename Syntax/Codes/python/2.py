import os
import sys
import io


f1=open('test_out.txt', 'r')
f2=open('find_errors.txt', 'r')
f1_out=[]
f2_out=[]
res=[]
f1_out=f1.read()
f2_out=f2.read()
res=f1_out+f2_out

fin = open('test_result.txt', 'w')
fin.write(res)
     
f1.close()
f2.close()
fin.close()