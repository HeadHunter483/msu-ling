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
func('buffer1.txt','	Удаление файла_1 для хранения временных файлов:')
func('buffer2.txt','	Удаление файла_2 для хранения временных файлов:')
func('semantic.txt','	Удаление файла для семантики:')
func('test_results.txt','	Удаление данных первичной проверки:')
func('semantic.txt','	Удаление данных для семантики:')


os.chdir(path+'/tmp')
func('in_mystem_in.txt','	Удаление файлов входных данных mystem:')
func('in_mystem_out.txt','	Удаление файлов выходных данных mystem:')
func('in_treetagger_in.txt','	Удаление файлов входных данных treetagger:')
func('in_treetagger_out.txt','	Удаление файлов выходных данных treetagger:')
func('in_raw.conll','	Удаление файлов in_raw.conll:')

os.chdir(path+'/bin/Malt') 
func('output.txt','	Удаление файлов работы MaltParser:')
func('temp.txt','	Удаление файла_3 для хранения временных файлов:')
func('temp2.txt','	Удаление файла_4 для хранения временных файлов:')
func('final.txt','	Удаление выходных файлов результатов исправления:')
func('results.txt','	Удаление результатов тестирования:')
func('temp5.txt','	Удаление файла_5 для хранения временных файлов:')
func('temp6.txt','	Удаление файла_6 для хранения временных файлов:')
func('test_out.txt','	Удаление данных первичной проверки:')


print('3.2.Проверка наличия необходимых файлов:')
func2('dict.txt')
func2('predlogipadeji.txt')
func2('options.ini')
func2('num.txt')
func2('mathword.txt')

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


