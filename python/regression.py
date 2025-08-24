

def regressionLine(x,y) :
    n = len(x)
    sx, sy, sx2, sxy = 0, 0, 0, 0

    for i in range(n) :
        sx += x[i]
        sy += y[i]
        sx2 += x[i] * x[i]
        sxy += x[i] * y[i]

    a = (sx2 * sy - sx * sxy) / (n * sx2 - sx * sx)
    b = (n * sxy - sx * sy) / (n * sx2 - sx * sx)
    
    return a,b



regressionLine([25,30,35,40,45,50], [78,70,65,58,48,42]) # == (114.381, -1.4457)
