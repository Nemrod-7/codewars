
class node :
    def __init__ (self, id) :
        self.id = id
        self.visit = False
        self.edges = []


def getstart (graph) :
    start = 0
    minv = 99

    for i in range(1, len(graph)) :
        if len(graph[i]) <= minv :
            minv = len(graph[i])
            start = i

    return start

def dfs (index, seq, visit, graph) :

    if index == len(seq) : return True
    
    curr = seq[index]
    visit[curr] = True

    for edge in graph[curr] :
        if visit[edge] == False :
            visit[edge] = True
            seq[index + 1] = edge

            dfs(index + 1, seq, visit, graph)

            visit[edge] = False


    return False

def square_sums (N) :

    graph = [] 
    i, squares = 2, []
    visit = [False] * (N + 1)

    while i * i <= (2 * N) : squares.append( i * i); i += 1

    for i in range(0, N + 1) : graph.append([])

    for i in range(0, N) :
        for j in range(0, N) :
            if ((i + 1) + (j + 1)) in squares :
                graph[i+1].append(j + 1)

    index = 0
    seq = [0] * N

    seq[index] = getstart(graph)
    visit[seq[index]] = True

    dfs(index, seq, visit, graph)

    print(seq)
    # for i in range(1,N+1) :
    #     print(i, visit[i], graph[i])


    return False


# [ 9, 7, 2, 14, 11, 5, 4, 12, 13, 3, 6, 10, 15, 1, 8 ]

square_sums(15)

