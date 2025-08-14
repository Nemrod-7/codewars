
def calculate (rect) :
    area = 0
    hist = {}
    line = set()

    for i in range(0,len(rect)) :
        x1, x2 = rect[i][0], rect[i][2]

        hist[x1] = [i] if not x1 in hist else hist[x1] + [i]
        hist[x2] = [i] if not x2 in hist else hist[x2] + [i]

    # print(hist)
    sweep = [ [x, hist[x]] for x in hist ]
    sweep.sort()
    for i in range(0,len(sweep)) :
        [x, record] = sweep[i]

        if line :
            x0 = sweep[i - 1][0]
            miny, maxy = 999, -999
            vertical, interval = [], []

            for idx in line :
                vertical.append([rect[idx][1], rect[idx][3]])

            vertical.sort();

            for y in vertical :
                if len(interval) == 0 :
                    interval.append(y)
                else :
                    last = len(interval) - 1

                    if interval[last][1] < y[0] :
                        interval.append(y)
                    else :
                        interval[last][1] = max(interval[last][1], y[1])

            # print(interval)
            for [y0,y1] in interval :
                area += (x - x0) * (y1 - y0)

        for idx in record :
            [x1,y1,x2,y2] = rect[idx]

            if x == x1 : line.add(idx)
            if x == x2 : line.remove(idx)

    return area

print('\n\n\n\n\n\n')
rect = [[3,3,8,5], [11,6,14,12], [6,3,8,9]]
# actual = calculate(rect)
# print(actual)

sweep = {}
for ix in range(len(rect)) :
    x1, x2 = rect[ix][0], rect[ix][2]

    sweep[x1] = [ix] if not x1 in sweep else sweep[x1] + [ix]
    sweep[x2] = [ix] if not x2 in sweep else sweep[x2] + [ix]

line = set()
sweep = sorted(sweep.items(), key=lambda x: x[0])
#
print(sweep)

for [x, record] in sweep :
    # print(x, line)

    for i1 in record :
        [x1,y1,x2,y2] = rect[i1]

        if x == x1 : line.add(i1)
        if x == x2 : line.remove(i1)
