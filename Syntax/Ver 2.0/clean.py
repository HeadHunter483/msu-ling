import os
import fileinput
import sys
import subprocess

path = os.path.dirname(os.path.abspath(__file__))

if __name__ == "__main__":
		mode=sys.argv[1]
		maxiter=sys.argv[2]
		
def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))
	
def func(name,text):
	try:
		with open(name): pass 
		os.remove(name)
		print(text+'1')
	except IOError:
		print(text+'0')
		
def func2(name):
	try:
		with open(name): pass 
		print('	'+name+' 1')
	except IOError:
		print('	'+name+' 0')
		print('Добавьте недостающие файлы.Затем запустите программу')
		subprocess_cmd('break')
		subprocess_cmd('exit')
		
print('3.1.Удаление результатов работы предыдущего запуска:')

os.chdir(path)
func('out.txt','	Удаление выходного файла:')
func('semantic.txt','	Удаление файла для семантики:')
func('test_results.txt','	Удаление данных первичной проверки:')

os.chdir(path+'/tmp')
func('in_raw.conll','	Удаление файлов in_raw.conll:')


print('3.2.Проверка наличия необходимых файлов:')
func2('dict.txt')
func2('predlogipadeji.txt')
func2('rules.txt')


if(mode=="auto"):
	os.chdir(path)
	print ('-'*40)
	
if(mode=="addmode"):
	os.chdir(path)
	print ('-'*40)
	
if(mode=="test"):
	i=1
	maxiternum=int(maxiter)
	while i <= maxiternum:	
		func2('test_'+str(i)+'.txt')
		i=i+1
	print ('-'*40)
	os.chdir(path)


