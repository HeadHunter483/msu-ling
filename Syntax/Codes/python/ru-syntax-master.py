import os
import fileinput
import io
import sys
import subprocess

current_dir = os.path.dirname(os.path.abspath(__file__))


if __name__ == "__main__":
	path = sys.argv[1] # путь до родительской директории
	mode = sys.argv[2] # режим тестирования,авто или добавления
	iterationnum = sys.argv[3] # номер текущей итерации
	itistheend = sys.argv[4] # общее количество итераций

def add_two_to_one():
	f1=open('test_out.txt', 'r')
	f2=open('find_errors.txt', 'r')
	f1_out=[]
	f2_out=[]
	res=[]
	f1_out=f1.read()
	f2_out=f2.read()
	res=f1_out+f2_out
	fin = open('test_results.txt', 'w')
	fin.write(res)
	f1.close()
	f2.close()
	fin.close()
	
	
	
def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))
	
def test(pth,iternum,itend):
	print('5.Исправление ошибок:')
	subprocess_cmd('python converter.py out.txt ansi')
	subprocess_cmd('Errors.exe test')  # запускаем исправление ошибок
	subprocess_cmd('python converter.py final.txt utf8')
	print('6.Тестирование '+iternum+ ':')
	subprocess_cmd('test.exe final.txt test_' + iternum + '.txt ' + iternum) # тестирующей программе на вход подается файл с разбором,файл с соответствующим правильным разбором и номер текущего теста
	
	if(iternum == itend):  # если это последняя итерация
		subprocess_cmd('Stats.exe')
		add_two_to_one()
		os.chdir(pth)
		subprocess_cmd("copy " + pth + "\\bin\\Malt\\results.txt " + pth+" /Y")# копируем файл-результат в исходную папку
		os.rename('results.txt','out.txt')
		subprocess_cmd("copy " + pth + "\\bin\\Malt\\test_results.txt " + pth+" /Y")# копируем файл-результат в исходную папку
		os.chdir(pth)   # переходим в родительскую директорию

		
def auto(ph,inum,iend):
		print('6.Исправление ошибок:')
		#subprocess_cmd('python converter.py out.txt ansi')
		subprocess_cmd('Errors.exe auto')  # запускаем исправление ошибок
		#subprocess_cmd('python converter.py final.txt utf8') 

		#print('7.Визуализация синтаксического дерева: '+inum)
		subprocess_cmd('python '+ph+'\\'+'\\graph.py '+inum)
		
		#subprocess_cmd("python conllutotikzdep.py "+ph+" "+inum) # вызываем скрипт визуализации
		#f = open("temp.txt", "r",encoding="utf8")
		#for line in f:
			#print(line) # выводим на экран содержимое
		#os.remove('temp.txt')
		
		
		if(inum == iend):  # если это последняя итерация	
			os.chdir(ph)   # переходим в родительскую директорию 
			subprocess_cmd("copy " + ph + "\\bin\\Malt\\temp2.txt " + ph+" /Y")# копируем файл-результат в исходную папку
			os.rename('temp2.txt','out.txt') # переименовываем
			subprocess_cmd("copy " + ph + "\\bin\\Malt\\semantic.txt " + ph+" /Y")# копируем файл-результат в исходную папку
			os.chdir(ph)   # переходим в родительскую директорию
			
			
			
def addmode(pth,iternum,itend):
	subprocess_cmd('python converter.py out.txt ansi')
	subprocess_cmd('Errors.exe test')  # запускаем исправление ошибок
	subprocess_cmd('python converter.py final.txt utf8')
	os.chdir(pth)
	

os.chdir(path)
#subprocess_cmd('python 2.py')
#subprocess_cmd('python morph.py '+path)
#subprocess_cmd('python 4.py')
os.chdir(current_dir + '/bin/Malt')
#subprocess_cmd('java -jar maltparser-1.8.1.jar -c PTM -i ' + path +'\\' + '\\tmp' + '\\' + '\\in_raw.conll -o out.txt -m parse')
#subprocess_cmd('lowercase.py')  # приводим к нижнему регистру
# для корректной работы исправления нужно,чтобы входной файл был в ansi-кодировке


if (mode == "auto"):
	auto(path,iterationnum,itistheend)
   
if (mode == "test"):
	test(path,iterationnum,itistheend)
	
if (mode=="addmode"):
	addmode(path,iterationnum,itistheend)
