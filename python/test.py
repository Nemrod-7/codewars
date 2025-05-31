import re

class board :
    def __init__(self, grid) :
        self.width = len(grid[0])
        self.height = len(grid)
        self.grid = grid

    def is_inside(self, p) : return p[0] >= 0 and p[1] >= 0 and p[0] < self.width and p[1] < self.height
    def is_free(self, p) : return self.is_inside(p) and self.grid[p[1]][p[0]] != '0'
    def at(self, p) : return '.' if self.is_free(p) else ' '

# result = diff("(tan (* 2 x))")

"(* 2 (+ 1 (^ (tan (* 2 x)) 2)))", 
"(* 2 (^ (cos (* 2 x)) -2))", 
"(/ 2 (^ (cos (* 2 x)) 2))"

var = 'x'

def form(sym, a, b='') : return '('+ sym + ' ' + a + (' ' if b else '') + b + ')'

n1 = form('sin', 'x')
print(n1)

n2 = form('+', '2', '3')
print(n2)


