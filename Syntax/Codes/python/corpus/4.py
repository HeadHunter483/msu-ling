import fileinput
import glob

a='.'
b='?'
c='!'

with open('corpus.conllu', 'w') as f:
    for line in fileinput.input(glob.glob("test_*.txt")):
        if a in line:
            f.write(line+'\n')
        else:
            if b in line:
                f.write(line+'\n')
            else:
                if c in line:
                    f.write(line+'\n')
                else:
                    f.write(line)
            
            
    fileinput.close()
    
