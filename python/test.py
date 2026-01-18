for _ in range(3) : print()

branch = [ [0, 'C', []],[1, 'C', [2]],[2, 'C', [1,3]],[3, 'C', [2,4]], [4, 'C', [3]],]

width = len(branch)
height = 1

graph = [ [ [0] for _ in range(width * 3)] for _ in range(height * 3)]

i = 1
atom = branch[i]
id,elt,edge = atom
x,y = 1,1

graph[y][x] = [id]

if len(edge) == 1 :
    if edge[0] == branch[i + 1][0] :
        pass

for y in range(len(graph)) :
    for x in range(len(graph[y])) :
        print(graph[y][x], end='')
    print()
