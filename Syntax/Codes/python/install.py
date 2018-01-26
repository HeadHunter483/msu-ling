import sys
import subprocess
import os

path = os.path.dirname(os.path.abspath(__file__))

def subprocess_cmd(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    proc_stdout = process.communicate()[0].strip()
    print(proc_stdout.decode('windows-1251'))

print('Для корректной работы программы  необходимо:')
print('	1.Операционная система Windows.(работоспособность программы проверялась на операционной системе Windows 10)')
print('	2.Java development kit.Необходим для работы MaltParser')
print('	MaltParser по умолчанию версии 1.8.1')
print('	3.В переменные среды нужно добавить путь.....\ru-syntax-master\visual\bin')
print('	При сборке необходимо будет докачать недостающие компоненты:')
print('   python -m pip install pydotplus')
print('   python -m pip install pymorphy2')
print('   python -m pip install pymorphy2-dicts')
print('	4.Python версии не ниже 3.6')
print('	Для корректной работы программы необходима библиотека chardet.Установить ее из локального хранилища ?')
print(' Y/N')
text=input()

if(text=="Y"):
	os.chdir(path+'\\install')
	subprocess_cmd('python -m pip install chardet-3.0.4-py2.py3-none-any.whl')
	os.chdir(path)
