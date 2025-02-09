class vertex :
    def __init__ (self, id) :
        self.id = id
        self.visit = False
        self.edges = []

graph = [
        [8,4,5],
        [15,7],
        [1,6,8,9],
        [145]
        ]


graph.sort(key=len)

print(graph)
