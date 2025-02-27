import time
import sys

sys.setrecursionlimit(1100)
start = time.time()

class vertex :
    def __init__ (self, id) :
        self.id = id
        self.visit = False
        self.edges = []

class Graph :
    def __init__ (self, N) :
        self.squares = [i * i for i in range(2, int(N ** 0.5) + 1)]
        self.vmap = [vertex(i) for i in range(0,N+1)]

        for i in range(1, N + 1) : self.update(self.vmap[i])

    def update (self, node) :
        node.edges = []

        for sq in self.squares :
            j = sq - node.id
            if j > 0 and j < len(self.vmap) and j != node.id and self.vmap[j].visit == False :
                node.edges.append(j)

def getstart (vmap) :
    start, minv = 0, 99

    for i in range(1, len(vmap)) :
        if len(vmap[i].edges) <= minv :
            minv = len(vmap[i].edges)
            start = i

    return start

def dfs (index, seq, curr) :

    if index == len(seq) - 1 : return True

    graph = curr.vmap
    node = graph[seq[index]]

    for edge in node.edges :
        curr.update(graph[edge])

    node.edges.sort(key=lambda x: len(graph[x].edges))

    for edge in node.edges :
        if graph[edge].visit == False :
            graph[edge].visit = True
            seq[index + 1] = edge

            if dfs(index + 1, seq, curr)  : return True

            seq[index + 1] = 0
            graph[edge].visit = False

    return False

def square_sums (N) :

    curr = Graph(N)
    seq = [0] * N

    graph = curr.vmap
    seq[0] = getstart(graph)
    graph[seq[0]].visit = True

    return seq if dfs(0, seq, curr) == True else False


for num in [5, 15, 23, 50, 56, 128, 512, 993, 1000] :
    res = square_sums(num)


end = time.time()
print( "elapsed : ", end - start)
