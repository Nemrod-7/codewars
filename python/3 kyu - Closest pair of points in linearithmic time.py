import numpy as np

def distance (p1,p2) :
    return np.hypot(p1[0] - p2[0], p1[1] - p2[1])

def mindist (left, right) :
    return left if distance(left[0],left[1]) < distance(right[0],right[1]) else right

def bruteforce (point) :
    minv = float('inf')
    pair = [(0,0),(0,0)]

    for i in range(0, len(point) - 1) :
        for j in range(i + 1, len(point) ) :
            dist = distance(point[i], point[j])

            if dist < minv :
                minv = dist
                pair = [ point[i], point[j] ]

    return pair

def stripdist (point, close) :
    next = close
    minv = distance(close[0], close[1])

    point.sort(key = lambda x: x[1] )

    for i in range(0,len(point)) :
        j = i + 1

        while j < len(point) and abs(point[i][1] - point[j][1]) < minv :
            dist = distance(point[i], point[j])

            if dist < minv :
                minv = dist
                next = [point[i], point[j]]
            j += 1

    return next

def closest_pair(points):
    points = [p for p in points]
    points.sort()
    
    if len(points) < 4 : return bruteforce(points)

    left, right = [], []
    center = points[int(len(points) / 2)]

    for p in points : left.append(p) if p[0] < center[0] else right.append(p)

    close = mindist(closest_pair(left),closest_pair(right))
    minv = distance(close[0], close[1])
    strip = [ p for p in points if abs(p[0] - center[0]) < minv ]

    return stripdist(strip, close)

point = (
    (2, 2), # A
    (6, 3), # D
    (2, 8), # B
    (5, 5), # C
    (7, 4), # F
    (6, 7), # E
    (7, 9)  # G
)

pair = closest_pair(point)
print('actual : ', pair)

