
def calculate (rect) :
    area = 0
    hist = {}
    line = set()

    for i in range(0,len(rect)) :
        x1 = rect[i][0]
        x2 = rect[i][2]

        hist[x1] = [i] if not x1 in hist else hist[x1] + [i]
        hist[x2] = [i] if not x2 in hist else hist[x2] + [i]

    sweep = [ [key, hist[key]] for key in hist]
    sweep.sort()


    for i in range(0,len(sweep)) :
        [x, record] = sweep[i]

        if line :
            vertical, interval = [], []
            x0 = sweep[i - 1][0]
            miny, maxy = 999, -999

            for idx in line :
                vertical.append([rect[idx][1], rect[idx][3]])
            
            vertical.sort();
            # print(vertical)
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
rect = [[3,3,8,5], [6,3,8,9], [11,6,14,12]]
actual = calculate(rect)
print(actual)
