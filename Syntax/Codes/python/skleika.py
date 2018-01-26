import os
import codecs

buff=[]
new_buff=[]

print("start!")
f=open("D:\\Analyz\\notstable\\ru-syntax-master\\tmp\\in_raw.conll","r",encoding="utf-8")
rows=[]

for line in f:
    row=line.split('	')
    rows.append(row)

i=0
flag=0
flag2=1
del_index=0


while(flag!=1 and i<(len(rows))):
    if rows[i][1]=="sqrt" and rows[i+1][1]=="(" and rows[i+3][1]==")":
        rows[i][1]=rows[i][1]+rows[i+1][1]+rows[i+2][1]+rows[i+3][1]
        rows[i][2]=rows[i][1]
        del_index=i

        j=0
        while(j<3):
            del rows[del_index+1]
            j=j+1
        

    if rows[i][1].isdigit() and rows[i+1][1]=="/" and rows[i+2][1].isdigit():
        rows[i][1]=rows[i][1]+rows[i+1][1]+rows[i+2][1]
        rows[i][2]=rows[i][1]
        del_index=i

        j=0
        while(j<2):
            del rows[del_index+1]
            j=j+1

    if rows[i][1]=="см" and rows[i-1][1].isdigit():
        u=i-1
        while(flag2==1):
            if (rows[u][1].isdigit() or rows[u][1]=="и" or rows[u][1]=="," or rows[u][1]=="см"):
                flag2=1
                if (rows[u][1].isdigit()):
                    rows[u][1]=rows[u][1]+"см"
                    rows[u][2]=rows[u][2]+"см"
            else:
                flag2=0
            u=u-1

    if rows[i][1]=="см" and rows[i-1][3]=="NUM":
        if not "см" in rows[i-1][1]:
            rows[i-1][1]=rows[i-1][1]+"см"
            rows[i-1][2]=rows[i-1][2]+"см"

    if rows[i][1]=="см":
        del rows[i]

    if rows[i][1]=='.' or rows[i][1]=='!' or rows[i][1]=='?':
        flag=1


    i=i+1






print(rows)
f.close()



print('after')
f=codecs.open("D:\\Analyz\\notstable\\ru-syntax-master\\tmp\\in_raw2.conll","w","utf-8")
i=0
j=0

for i in range(len(rows)-1):
        f.write(str(i+1)+'	'+rows[i][1]+'	'+rows[i][2]+'	'+rows[i][3]+'	'+rows[i][4]+'	'+rows[i][5]+'	'+rows[i][6]+'	'+rows[i][7]+'	'+rows[i][8]+'\n')
#for item in rows:
    #print(item)
    #f.write("%s\n" % item)

f.close()





