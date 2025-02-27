from typing import NamedTuple
import math
import heapq

# import numpy as np
# import matplotlib.pyplot as plt
# from matplotlib.patches import Circle

epsilon = 1e-8

class Point (NamedTuple):
    x: float
    y: float

class Circle (NamedTuple):
    ctr: Point
    r:   float

def sqr (a) :
    return a * a

def distance (a, b) :
    return  math.hypot( a.x - b.x, a.y - b.y)

def inside_circle (p, c) :
    return distance(c.ctr, p) < c.r

def slope (a, b) :
    return (b.y - a.y) / (b.x - a.x) if (a.x != b.x) else 1e8

def tangent (p1, c) : # tangent points of a cricle => gives two lines p1,t1 and p1,t2
    [c1,rad] = c

    theta = math.acos (rad / distance (p1,c1)) # angle == adj / hyp
    direc = math.atan2 (p1.y - c1.y, p1.x - c1.x)  # direction angle of point P from C

    d1 = direc + theta
    d2 = direc - theta

    t1 = Point(c1.x + rad * math.cos (d1), c1.y + rad * math.sin (d1)) # tangent to circle c1 => line : p1 ~ p2
    t2 = Point(c1.x + rad * math.cos (d2), c1.y + rad * math.sin (d2)) # tangent to circle c1 => line : p1 ~ p2

    return [t1,t2]

def interception (line, circle) : # return interception points of a line (p1,p2) and a circle
    [p1,p2] = line
    [c1,r1] = circle

    p3 = Point(p1.x - c1.x, p1.y - c1.y)
    p4 = Point(p2.x - c1.x, p2.y - c1.y) # shifted line points

    m = slope (p3,p4)    #  slope of the line
    b = p3.y - m * p3.x  # y intercept of line
    np = sqr(r1) * sqr(m) + sqr(r1) - sqr(b) # quad equation

    if np > epsilon :
        t1 = (-m * b + math.sqrt(np)) / (sqr(m) + 1)
        t2 = (-m * b - math.sqrt(np)) / (sqr(m) + 1)

        i1 = Point(t1 + c1.x, m * t1 + b + c1.y)
        i2 = Point(t2 + c1.x, m * t2 + b + c1.y)
        return [i1,i2]

    return []

def hcenter (c1, c2) : # homothetic centers of two circles
    [p1,r1] = c1
    [p2,r2] = c2
    hc = []
    mn, mx = r1 - r2, r1 + r2
    # 1 : external point, 2: internal point

    if r1 == r2 : #  if 2 circles have the same radius, the external hc == inf. so 2 external hc must be determined perpendicular to the midpoint
        c = Point((p2.x + p1.x) * 0.5, (p2.y + p1.y) * 0.5)
        theta = math.atan2 (p2.y - p1.y, p2.x - p1.x)

        x = c.x - r1 * math.sin (theta)
        y = c.y + r1 * math.cos (theta)
        hc.append (Point(x,y))

        x = c.x + r1 * math.sin (theta)
        y = c.y - r1 * math.cos (theta)
        hc.append (Point(x,y))
    else :
        x = -r2 / mn * p1.x + r1 / mn * p2.x
        y = -r2 / mn * p1.y + r1 / mn * p2.y
        hc.append (Point(x,y))

    x = r2 / mx * p1.x + r1 / mx * p2.x
    y = r2 / mx * p1.y + r1 / mx * p2.y
    hc.append (Point(x,y))

    return hc

def collision (p1, p2, circles) :
    dist = distance (p1,p2)

    for c1 in circles :
        if inside_circle (p1, c1) or inside_circle (p2, c1) : return True

        for ip in interception ([p1,p2], c1) :
            if (distance (p1,ip) + distance (ip,p2) - dist) <= epsilon :
                # print(distance (p1,ip) + distance (ip,p2) - dist)
                return True

    return False

def tangraph (start: Point, exit: Point, circles: list[Circle]) :

    # fig , ax = plt.subplots()             # Create a figure
    # ax.set_aspect('equal')
    #
    # ax.plot(start.x, start.y, 'r.')
    # ax.plot(exit.x, exit.y, 'r.')
    #
    # for c1 in circles :
    #     ax.add_patch(plt.Circle(c1.ctr, c1.r, color = 'y'))

    size = len(circles)

    i, j = 0, 2
    c1, c2 = circles[i], circles[j]

    for p1 in hcenter (c1,c2) :
        if not inside_circle(p1,c1) and not inside_circle(p1,c2) :
            t1, t2 = tangent(p1,c1), tangent(p1,c2)
            p3, p4 = t1[1], t2[1]
            valid = True

            dist = distance (p3,p4)

            for c3 in circles :
                if inside_circle (p3, c3) or inside_circle (p4, c3) :
                    valid = False

                ip = interception ([p3,p4], c3)
                if len(ip) < 2 : continue

                if (distance (p3,ip[0]) + distance (ip[1],p4) - dist) <= epsilon :
                    # ax.plot(ip.x, ip.y, 'g+')
                    valid = False

            if valid == True :
                # ax.plot(p3.x, p3.y, 'g+')
                # ax.plot(p4.x, p4.y, 'g+')
                # ax.plot([t1[1].x, t2[1].x], [t1[1].y, t2[1].y], 'b')
                pass


    # plt.show()
    pass

start, exit = Point(-3, 1), Point(4.25, 0)
circles = [Circle(Point(0,0), 2.5), Circle(Point(1.5,2), 0.5), Circle(Point(3.5,1), 1), Circle(Point(3.5,-1.7), 1.2)]

for _ in range(0,5) : print()

tangraph(start,exit,circles)

print("exit")
