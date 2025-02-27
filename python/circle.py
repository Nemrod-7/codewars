from typing import NamedTuple
import math
import heapq

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Circle

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

def direction (a, b, c) :
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0.0

def inside_circle (p, c) :
    return (c.r - distance(c.ctr, p)) > epsilon

def slope (a, b) :
    return (b.y - a.y) / (b.x - a.x) if (a.x != b.x) else 1e8

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

def tangent (p1, c) : # tangent points of a cricle => gives two lines p1,t1 and p1,t2
    [c1,rad] = c

    theta = math.acos (rad / distance (p1,c1)) # angle == adj / hyp
    direc = math.atan2 (p1.y - c1.y, p1.x - c1.x)  # direction angle of point P from C

    d1 = direc + theta
    d2 = direc - theta

    t1 = Point(c1.x + rad * math.cos (d1), c1.y + rad * math.sin (d1)) # tangent to circle c1 => line : p1 ~ p2
    t2 = Point(c1.x + rad * math.cos (d2), c1.y + rad * math.sin (d2)) # tangent to circle c1 => line : p1 ~ p2

    return [t1,t2]

def collision (p1, p2, circles) :
    dist = distance (p1,p2)

    for c1 in circles :
        if inside_circle (p1, c1) or inside_circle (p2, c1) : return True

        for ip in interception ([p1,p2], c1) :
            if (distance (p1,ip) + distance (ip,p2) - dist) <= epsilon :
                print(distance (p1,ip) + distance (ip,p2) - dist)
                return True

    return False

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

def circles_intersection (c1,c2) : # intercection points of 2 circles
    [p1,r1] = c1
    [p2,r2] = c2
    dist = distance (p1, p2)

    if dist <= r1 + r2 and dist >= abs (r2 - r1) :
        dx = (p2.x - p1.x) / dist
        dy = (p2.y - p1.y) / dist

        a = (sqr(r1) - sqr(r2) + sqr(dist)) / (2 * dist)
        h = math.sqrt (r1 * r1 - a * a)

        p3 = Point(p1.x + a * dx - h * dy, p1.y + a * dy + h * dx)
        p4 = Point(p1.x + a * dx + h * dy, p1.y + a * dy - h * dx)

        return [p3,p4]

    return []

def shortest_path_length (start: Point, exit: Point, circles: list[Circle]) -> float:

    if not collision (start,exit, circles) :
        return distance(start,exit)

    fig , ax = plt.subplots()             # Create a figure
    ax.set_aspect('equal')

    ax.plot(start.x, start.y, 'r.')
    ax.plot(exit.x, exit.y, 'r.')

    for c1 in circles :
        ax.add_patch(plt.Circle(c1.ctr, c1.r, color='y' ))

    size = len(circles)

    # finding all tangents
    # node => eg : [Point, circle_id]
    # line => eg : [Node, node]
    graph = []                           # tangent visibility graph                               => [[line], [line],[line]]
    edges = [[] for _ in range(0, size)] # list of all reference of tangent lines for each circle => [c1:[int,int,...], c2:[int,int,...]] (i references a graph[i])
    icept = [[] for _ in range(0, size)] # list of all intersections points between circles       => [[Point, Point, ...],[],[],[]]

    for i in range(0,size) :
        c1 = circles[i]

        for j in range(i + 1, size) :
            c2 = circles[j]

            for p1 in hcenter (c1,c2) :
                if not inside_circle(p1,c1) and not inside_circle(p1,c2) :
                    t1, t2 = tangent(p1,c1), tangent(p1,c2)

                    if not collision(t1[0],t2[0], circles) :    # if the tangent point doesn't lie on a circle
                        graph.append([[t1[0], i], [t2[0], j]])

                        edges[i].append(len(graph) - 1)
                        edges[j].append(len(graph) - 1)

                    if not collision(t1[1],t2[1], circles) :    # if the tangent point doesn't lie on a circle
                        graph.append([[t1[1], i], [t2[1], j]])

                        edges[i].append(len(graph) - 1)
                        edges[j].append(len(graph) - 1)

        pass

    for i in range(0,size) :
        for j in range(i+1, size) :
            for p in circles_intersection(circles[i], circles[j]) :
                icept[i].append(p)
                icept[j].append(p)

        pass

    # handling start and exit point
    heap = []

    for i in range(0,size) :
        c1 = circles[i]
        t1 = tangent(start, c1)
        t2 = tangent(exit , c1)

        if not collision(start,t1[0], circles) :
            graph.append([[start, -1 ], [t1[0], i]])
            heapq.heappush(heap, [0, distance(start, t1[0]), [t1[0], i], len(graph) - 1, [[[start, -1 ], [t1[0], i]]]])

        if not collision(start,t1[1], circles) :
            graph.append([[start, -1 ], [t1[1], i]])
            heapq.heappush(heap, [0, distance(start, t1[1]), [t1[1], i], len(graph) - 1, [[start, -1 ], [t1[1], i]]])

        if not collision(exit,t2[0], circles) :
            graph.append([[t2[0], i], [exit, -1]])
            edges[i].append(len(graph) - 1)

        if not collision(exit,t2[1], circles) :
            graph.append([[t2[1], i],[exit, -1]])
            edges[i].append(len(graph) - 1)

    # pathfinder
    visit = [False] * len(graph) #

    for i in range(0, len(graph)) :
        x, y = [], []
        for j in range(0, len(graph[i])) :
            x.append(graph[i][j][0].x)
            y.append(graph[i][j][0].y)
        ax.plot(x,y, 'b')

    if heap :
        [heur, dist, [p1,id], edge, path] = heapq.heappop(heap)

        [ctr, rad] = circles[id]
        visit[edge] = True

        if p1 == exit :
            x, y = [], []

            for p in path :
                x.append(p[0].x)
                y.append(p[0].y)

            ax.plot(x,y, 'b')

            return dist

        minv = 2.0 * math.pi
        for p2 in icept[id] :
            angle = 2.0 * math.asin(distance(p1, p2) * 0.5 / rad)
            minv = min(minv, angle)

            pass

        for next in edges[id] :
            if not visit[next] :
                [n1,n2] = graph[next]
                if n1[1] != id : n1, n2 = n2, n1

                alt = dist + distance (n1[0], n2[0])

                angle = 2.0 * math.asin(distance(p1, n1[0]) * 0.5 / rad) # minor angle in radian
                major = 2.0 * math.pi - angle                            # major angle in radian
                dir = direction(p1,ctr,n1[0])

                # print(n1[0], dir)
                nxdist = angle * rad + alt
                heapq.heappush(heap,[heur, nxdist, n2, next, path + [n1,n2]])
            pass

    plt.show()
    return -1.0

def tangraph (start: Point, exit: Point, circles: list[Circle]) :
    fig , ax = plt.subplots()             # Create a figure
    ax.set_aspect('equal')

    ax.plot(start.x, start.y, 'r.')
    ax.plot(exit.x, exit.y, 'r.')

    for c1 in circles :
        ax.add_patch(plt.Circle(c1.ctr, c1.r, color = 'y'))

    size = len(circles)

    c1, c2 = circles[0], circles[2]

    for p1 in hcenter (c1,c2) :
        if not inside_circle(p1,c1) and not inside_circle(p1,c2) :
            t1, t2 = tangent(p1,c1), tangent(p1,c2)

            ax.plot([t1[0].x, t2[0].x], [t1[0].y, t2[0].y], 'b')
            ax.plot([t1[1].x, t2[1].x], [t1[1].y, t2[1].y], 'g')

            p3, p4 = t1[1], t2[1]
            dist = distance(p3,p4)

            for c3 in circles :
                for ip in interception ([t1[1], t2[1]], c3) :
                    if distance(p3,ip) + distance(ip,p4) - dist < epsilon :
                        ax.plot(ip.x, ip.y, 'b+')
                        pass
                    pass
                pass

    plt.show()
    pass

a, b = Point(-3, 1), Point(4.25, 0)
c = [Circle(Point(0,0), 2.5), Circle(Point(1.5,2), 0.5), Circle(Point(3.5,1), 1), Circle(Point(3.5,-1.7), 1.2)]


tangraph(a,b,c)
# shortest_path_length(a, b, c)


print("exit")
