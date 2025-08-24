import random

def calculate1 (rect) :
    area = 0
    sweep = {}
    line = set()
    hist = []

    for i in range(len(rect)) :
        x1, x2 = rect[i][0], rect[i][2]
        sweep[x1] = [i] if not x1 in sweep else sweep[x1] + [i]
        sweep[x2] = [i] if not x2 in sweep else sweep[x2] + [i]

    sweep = sorted(sweep.items(), key=lambda x: x[0])

    for i in range(len(sweep)) :
        [x, record] = sweep[i]

        if line :
            x0 = sweep[i - 1][0]
            miny, maxy = 999, -999
            vertical, interval = [], []
            # print(len(line), end=' ')

            for idx in line :
                vertical.append([rect[idx][1], rect[idx][3]])

            vertical.sort();

            for y in vertical :
                if not len(interval) or interval[-1][1] < y[0]:
                    interval.append(y)
                else :
                    interval[-1][1] = max(interval[-1][1], y[1])

            for [y0,y1] in interval :
                area += (x - x0) * (y1 - y0)

        for idx in record :
            [x1,y1,x2,y2] = rect[idx]
            if x == x1 : line.add(idx)
            if x == x2 : line.remove(idx)

    return area


def calculate3 (rect) :
    area = 0
    sweep = []
    line = set()

    for i in range(len(rect)) :
        x1,y1, x2,y2 = rect[i]
        sweep.append((x1,0,y1,y2, i))
        sweep.append((x2,1,y1,y2, i))

    sweep.sort()

    for i in range(len(sweep)) :
        [x, state, y1, y2, index] = sweep[i]

        if line :
            x0 = sweep[i - 1][0]
            miny, maxy = 999, -999
            vertical, interval = [],[]

            # print(len(line), end=' ')
            # for idx in line :
            #     vertical.append([rect[idx][1], rect[idx][3]])

            # vertical.sort();

            # for y in vertical :
            #     if not len(interval) or interval[-1][1] < y[0]:
            #         interval.append([y[0],y[1]])
            #     else :
            #         interval[-1][1] = max(interval[-1][1], y[1])

            for [y4,y3] in interval :
                area += (x - x0) * (y3 - y4)

        if state == 0 : line.add(index)
        if state == 1 : line.remove(index)

    return area

# for _ in range(10) :
#     graph = []
#
#     for i in range(10000) :
#         x1,x2,y1,y2 = [ random.randint(0, 1000000) for _ in range(4) ]
#         rect = [min(x1,x2), min(y1,y2), max(x1,x2), max(y1,y2)]

lim = 10000
rect = [ (0, i * 2, 9, i * 2 + 1) for i in range(lim) ] + [ (i * 2, 0, i * 2 + 1, 9) for i in range(lim) ]

# rect = [[3,3,8,5], [11,6,14,12], [6,3,8,9]]
actual = calculate1(rect) # 179175
print(actual)
