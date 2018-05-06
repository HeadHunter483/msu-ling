import os
import fileinput
import sys
import subprocess

path = os.path.dirname(os.path.abspath(__file__))

def cleaner():
    def subprocess_cmd(command):
        process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
        proc_stdout = process.communicate()[0].strip()
        print(proc_stdout.decode('windows-1251'))

    def func(name,text):
        try:
            with open(name): pass
            os.remove(name)

        except IOError:
            0
            
        
    def func2(name):
        try:
            with open(name): pass
       
        except IOError:
            print("Добавьте недостающие файлы")
            subprocess_cmd('break')
            subprocess_cmd('exit')

    path2=path+'\\..\\tmp'
    os.chdir(path2)
    func('morph.txt',' Удаление файлов morph.txt:')

    func2('dict.txt')
    func2('predlogipadeji.txt')
    func2('measure.txt')
    func2('morph_iskl.txt')
    func2('morph_iskl2.txt')

    path3=path2+'\\..'
    os.chdir(path3)
    func('syntax.txt',' Удаление файлов syntax.txt:')
    func('semantic.txt',' Удаление файлов semantic.txt:')
    func('test.txt','Удаление тестов')
    
