import os
import codecs

buff=[]
new_buff=[]
dict=[]

path = os.path.dirname(os.path.abspath(__file__))
os.chdir(path+"\\tmp")

f=open("in_raw.conll","r",encoding="utf-8")
f2=open("dict.txt","r",encoding="utf-8")


for line in f2:
    row=line.split('	')
    dict.append(row)

f2.close()


rows=[]

for line in f:
    row=line.split('	')
    rows.append(row)

i=0
flag=0
flag2=1
del_index=0
str4=""

while(i<(len(rows))):
    j=0

    while(j<len(dict)):
        if rows[i][1] in dict[j][0]:
            if rows[i+1][1] in dict[j][0]:
                rows[i][1]=dict[j][0]
                rows[i][2]=dict[j][0]
                str4=dict[j][1]
                str4=str4.replace('\n','')
                rows[i][3]=str4
                rows[i][4]=str4
                rows[i][5]=str4
                del_index=i

                k=0
                while(k<int(dict[j][2])):
                    del rows[del_index+1]
                    k=k+1


        j=j+1


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
                    rows[u][1]=rows[u][1]+" " +"см"
                    rows[u][2]=rows[u][2]+" "+"см"
            else:
                flag2=0
            u=u-1

    if rows[i][1]=="см" and rows[i-1][3]=="NUM":
        if not "см" in rows[i-1][1]:
            rows[i-1][1]=rows[i-1][1]+" см"
            rows[i-1][2]=rows[i-1][2]+" см"

    if rows[i][1]=="см":
        del rows[i]

    if rows[i][1]=='.' or rows[i][1]=='!' or rows[i][1]=='?':
        flag=1


    i=i+1


f.close()

f=codecs.open("in_raw.conll","w","utf-8")
i=0

for i in range(len(rows)):
    print(str(i+1)+'	'+rows[i][1]+'	'+rows[i][2]+'	'+rows[i][3]+'	'+rows[i][4]+'	'+rows[i][5]+'	'+rows[i][6]+'	'+rows[i][7]+'	'+rows[i][8]+'	'+rows[i][9])
    f.write(str(i+1)+'	'+rows[i][1]+'	'+rows[i][2]+'	'+rows[i][3]+'	'+rows[i][4]+'	'+rows[i][5]+'	'+rows[i][6]+'	'+rows[i][7]+'	'+rows[i][8]+'	'+rows[i][9])

f.close()





