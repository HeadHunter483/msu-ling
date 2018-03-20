@echo off
@echo morph.py %1 %2
python converter.py %1 utf8
python clean.py auto 0
python morph.py %1 %2
python skleika.py %2 %2
pause