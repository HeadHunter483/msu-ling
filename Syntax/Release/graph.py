import pydotplus as ptp
import os
import codecs
import sys

path = os.path.dirname(os.path.abspath(__file__))
os.environ["PATH"] += os.pathsep + path+'/visual/bin/'

def sep(l):
	return len(l.replace('=',' ').split())==0

def makegraph(syn):
        rows=syn

        for i in range(len(rows)):
                if rows[i][1]==",":
                        rows[i][1]=" ,"
        edges=[]
        nodes=[]

        graph = ptp.Dot(graph_name="syntax_tree",graph_type='digraph')
        graph.set_node_defaults(color='black', shape='box')#,fontname='bookman old style', fontsize='10')
        #graph.set_edge_defaults(fontsize='11')

        edges=len(rows)
        edge=0
        flag=0

        for edge in range(edges):
                if rows[edge][1]=='.' or rows[edge][1]=='!' or rows[edge][1]=='?':
                        id_1=str(rows[edge][0])
                        node2=ptp.Node(id_1,label=str(rows[edge][1]),fillcolor="white",style="dashed")
                        graph.add_node(node2)
                        break
                
                id_1=str(rows[edge][0])
                id_2=str(rows[edge+1][0])

                if (rows[edge][1]==' ,' or rows[edge][1]=='!'):
                        node2=ptp.Node(id_1,label=str(rows[edge][1]),fillcolor="white",style="dashed")
                else:
                        line_1=str(rows[edge][2])+' : '+str(rows[edge][1]) if rows[edge][2]!='NONLEX' else str(rows[edge][1]).upper() #+'\n'+str(rows[edge][3])
                        node2=ptp.Node(id_1,label=line_1,fillcolor="white",style="filled")

                graph.add_node(node2)
                graph.add_node(ptp.Node(id_2,label=str(rows[edge+1][1])))


        for edge in range(edges):
                id_1=str(rows[edge][0])
                id_2=str(rows[edge][3])

                if (rows[edge][3]!=str(0)):
                        edge_2=ptp.Edge(id_2,id_1)
                        edge_2.set_label(' '+str(rows[edge][4]))
                        graph.add_edge(edge_2)
        return graph
	
	###############################################
def drawgraph(fin, fout):
        f=codecs.open(fin,"r","utf-8")

        syn = []
        i=0
        for line in f:
                row=line.split('	')
                syn.append(row)

        if len(syn)>0:
                i+=1
                try:
                        g = makegraph(syn)
                        g.write_png(str(fout)+".png")
                        print('\nРабота завершена успешно\n')
                except:
                        print('\nВизуализировать граф не удалось\n')

                    
        syn=[]
                 


if __name__ == "__main__":
	if len(sys.argv)<=2:
		print("usage : python graph_m.py <input file> <output file>")
		exit(1)
	drawgraph(path+"\\Graphs\\"+sys.argv[1], path+"\\Graphs\\"+sys.argv[2])
