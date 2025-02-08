
def getstart (graph) :
    start = 0
    minv = 99

    for i in range(1, len(graph)) :
        if len(graph[i]) <= minv :
            minv = len(graph[i])
            start = i

    return start

def dfs (index, seq, visit, graph) :

    if index == len(seq) - 1 : return True
    
    curr = seq[index]
    visit[curr] = True

    print(seq)
    for edge in graph[curr] :
        if visit[edge] == False :
            visit[edge] = True
            seq[index + 1] = edge

            if dfs(index + 1, seq, visit, graph) == True : return True

            seq[index + 1] = 0
            visit[edge] = False

    return False

def square_sums (N) :

    i, squares = 2, []
    graph = [[] for _ in range(0,N+1)] 
    visit = [False] * (N + 1)

    while i * i <= (2 * N) : squares.append(i * i); i += 1

    for i in range(1, N+1) :
        for sq in squares :
            j = sq - i
            if j > 0 and j <= N and visit[j] == False:
                graph[i].append(j)

        print(i, graph[i])

    seq = [0] * N

    seq[0] = getstart(graph)
    visit[seq[0]] = True

    if dfs(0, seq, visit, graph) == True :
        return seq
    else :
        print(seq)
        return False


class node :
    def __init__ (self, id) :
        self.id = id
        self.visit = False
        self.edges = []

    def show (self) :
        print(self.id, self.visit, self.edges)


class graph :
    def __init__(self, N) :
        i = 2
        self.squares = []
        self.vmap = [node(i) for i in range(0,N+1)]

        while i * i <= (2 * N) : self.squares.append( i * i); i += 1

        for i in range(1, N + 1) :
            self.update(self.vmap[i])


    def update(self, node) :
        node.edges = []

        for sq in self.squares :
            j = sq - node.id
            if j > 0 and j < len(self.vmap) and self.vmap[node.id].visit == False :
                node.edges.append(j)
        pass

        

def struct(N) :

    curr = graph(N)

    for i in range(1, N + 1) :
        curr.vmap[i].show()

    pass

# struct(15)
res = square_sums(23)
print(res)


