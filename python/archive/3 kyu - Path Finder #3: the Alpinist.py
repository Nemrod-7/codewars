import heapq

infinity = 99999

def todig (char) :  return ord(char) - ord('0')
def isinside (pos, lim) : return pos >= 0 and pos < lim

def path_finder (maze) :

    size = maze.find('\n') + 1
    direction = [-1, 1, -size, size]

    queue = [[0,0]]
    visit = [False] * len(maze) 
    distc = [infinity] * len(maze)
    
    heapq.heapify(queue);
    
    while queue :
        [dist,curr] = heapq.heappop(queue)
        visit[curr] = True
        
        if curr == len(maze) - 1 : return dist

        for dir in direction :
            next = curr + dir

            if isinside(next, len(maze)) and maze[next] != '\n' :
                alt = dist + abs(todig(maze[curr]) - todig(maze[next]))
                
                if not visit[next] and alt < distc[next] :
                    distc[next] = alt
                    heapq.heappush(queue, [alt,next])
                    
    return False
