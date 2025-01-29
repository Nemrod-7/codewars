import heapq

def isinside (pos, lim) :
    return pos >= 0 and pos < lim

def path_finder (maze) :

    size = maze.find('\n') + 1
    direction = [-1, 1, -size, size]

    visit = [False] * len(maze) 
    distc = [99999] * len(maze) 
    queue = [[0,0]]
    
    heapq.heapify(queue);
    
    while queue :
        [dist,curr] = heapq.heappop(queue)

        visit[curr] = True

        if curr == len(maze) - 1 :
            return dist

        for dir in direction :
            next = curr + dir
            altt = dist + 1

            if isinside(next, len(maze)) and not visit[next] and maze[next] == '.' :
                heapq.heappush(queue, [altt,next])

    return False

a = "\n".join([
  ".W.",
  ".W.",
  "..."
])

b = "\n".join([
  ".W.",
  ".W.",
  "W.."
])

c = "\n".join([
  "......",
  "......",
  "......",
  "......",
  "......",
  "......"
])

d = "\n".join([
  "......",
  "......",
  "......",
  "......",
  ".....W",
  "....W."
])
path_finder(a)
#test.assert_equals(path_finder(a), 4)
#test.assert_equals(path_finder(b), False)
#test.assert_equals(path_finder(c), 10)
#test.assert_equals(path_finder(d), False)
