import os
import fileinput
import sys
import subprocess

path = os.path.dirname(os.path.abspath(__file__))

if __name__ == "__main__":
		mode=sys.argv[1]
		show=sys.argv[2]
		
		
def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))
	
def func(name,text):
    try:
        with open(name): pass
        os.remove(name)

        if (int(show)==1):
            print(text+'1')
            
    except IOError:
            if (int(show)==1):
                print(text+'0')
        
def func2(name):
    try:
        with open(name): pass
        if (int(show)==1):
                print(' '+name+' 1')
    except IOError:
            if (int(show)==1):
                print(' '+name+' 0')
                print('Добавьте недостающие файлы.Затем запустите программу')

            subprocess_cmd('break')
            subprocess_cmd('exit')

if (int(show)==1):		
    print('3.1.Удаление результатов работы предыдущего запуска:')

os.chdir(path)
func('out.txt','	Удаление выходного файла:')
func('semantic.txt','	Удаление файла для семантики:')

if (mode=="test"):
    func('test_results.txt','	Удаление данных тестирования:')

os.chdir(path+'/tmp')
func('in_raw.conll','	Удаление файлов in_raw.conll:')
func('tmp3.txt','')

if (int(show)==1):
    print('3.2.Проверка наличия необходимых файлов:')
    
func2('dict.txt')
func2('predlogipadeji.txt')
func2('rules.txt')

if (mode=='test'):
    func2('Tests.txt')
    func2('test.txt')
    

os.chdir(path)

	
	


