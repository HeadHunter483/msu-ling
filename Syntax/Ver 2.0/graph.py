import pydotplus as ptp
import os
import codecs
import sys

path = os.path.dirname(os.path.abspath(__file__))
os.environ["PATH"] += os.pathsep + 'E:/Analyz/ver_3/ru-syntax-master/visual/bin'

if __name__ == "__main__":
    iterationnum=sys.argv[1]

f=codecs.open("semantic.txt","r","utf-8")
rows=[]
edges=[]
nodes=[]

for line in f:
    print(line)
    if line=='\n':
        continue
    
    row=line.split('	')
    rows.append(row)

for i in range(len(rows)):
    if rows[i][1]==",":
        rows[i][1]=" ,"
        

graph = ptp.Dot(graph_name="syntax_tree",graph_type='digraph')
graph.set_node_defaults(color='black', shape='box',
                            fontname='arial', fontsize='13')

edges=len(rows)
edge=0
flag=0

for edge in range(edges):
    if rows[edge][1]=='.' or rows[edge][1]=='!':
        id_1=str(rows[edge][0])
        node2=ptp.Node(id_1,label=str(rows[edge][1]),fillcolor="white",style="dashed")
        graph.add_node(node2)
        break
    
    id_1=str(rows[edge][0])
    id_2=str(rows[edge+1][0])

    if (rows[edge][1]==' ,' or rows[edge][1]=='!'):
        node2=ptp.Node(id_1,label=str(rows[edge][1]),fillcolor="white",style="dashed")
    else:
        line_1=str(rows[edge][1])+'\n'+str(rows[edge][2])+'\n'+str(rows[edge][3])
        node2=ptp.Node(id_1,label=line_1,fillcolor="white",style="filled")

    graph.add_node(node2)
    graph.add_node(ptp.Node(id_2,label=str(rows[edge+1][1])))


for edge in range(edges):
    id_1=str(rows[edge][0])
    id_2=str(rows[edge][5])

    if (rows[edge][5]!=str(0)):
        edge_2=ptp.Edge(id_2,id_1)
        edge_2.set_label(' '+str(rows[edge][6]))
        graph.add_edge(edge_2)
      
graph.write_pdf(path+"\\Graphs\\file"+str(iterationnum)+".pdf")
graph.write_png(path+"\\Graphs\\file"+str(iterationnum)+".png")
#os.chdir(path)
