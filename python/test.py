from typing import NamedTuple

class Point (NamedTuple):
    x: float
    y: float
    
class Circle (NamedTuple):
    ctr: Point
    r:   float


start, exit = Point(-3, 1), Point(4.25, 0)
circles = [Circle(Point(0,0), 2.5), Circle(Point(1.5,2), 0.5), Circle(Point(3.5,1), 1), Circle(Point(3.5,-1.7), 1.2)]


dict = {}

dict[circles[0]] = 0
dict[circles[1]] = 1

print(dict[circles[0]])
