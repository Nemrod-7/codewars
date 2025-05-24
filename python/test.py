import numpy as np
# from scipy.cluster.vq import kmeans
# import matplotlib.pyplot as plt

def mk_seeds (grph, mode) :
    size = len(grph) / 7.0
    if not size : return []
    mid = round(grph[0][0] * 3 * size)
    V = [grph[0]]
    if mode > 2 : V.append(np.array([mid, 0.0]))
    V.append(grph[-1])
    return V

def equal (p1, p2) :
    if len(p1) != len(p2) : return False
    for i in range(len(p1)) : 
        if p1[i] != p2[i] : return False
    return True

class k_means :
    def nearest_point (graph, p1) :
        nearest = []
        min = np.inf

        for p2 in graph :
            dist = np.hypot(p1[0] - p2[0], p1[1] - p2[1])

            if dist < min :
                min = dist
                nearest = p2

        return nearest

    def clust (graph, seeds, iter = 10) :
        npts = [0 for _ in range(len(seeds)) ]
        sumX = [0 for _ in range(len(seeds)) ]
        sumY = [0 for _ in range(len(seeds)) ]

        for _ in range(iter) :
            for p in graph :
                near = k_means.nearest_point(seeds, p)

                for i in range(len(seeds)) :
                    if equal (near, seeds[i]) :
                    # if near[0] == seeds[i][0] and near[1] == seeds[i][1] :
                        npts[i] += 1
                        sumX[i] += p[0]
                        sumY[i] += p[1]

            for i in range(len(seeds)) :
                seeds[i][0] = sumX[i] / npts[i]
                seeds[i][1] = sumY[i] / npts[i]

        # print(seeds)
        return seeds


zero =  [[6, 7], [1, 29], [3, 14], [8, 3], [18, 2], [4, 10], [5, 10], [2, 16], [7, 4], [14, 2], [12, 1], [13, 1], [10, 1], [15, 1], [9, 1], [17, 1]]
zero = np.array(sorted(zero, key = lambda x: x[0]))

# seed1 = zero[np.argmin([zero[:,0]])]
# seed2 = zero[np.argmax([zero[:,0]])]

# norm = np.std(zero, 0)
# whit = whiten(zero)

# ctoid = kmeans(zero, mk_seeds(zero, 3), iter = 1)[0]
# ctoid = np.array(sorted(ctoid, key = lambda x: x[0]))
# stand = np.sort(ctoid * norm)


ctoid = k_means.clust (zero, mk_seeds(zero, 3), 1)

print(ctoid)

# plt.scatter(zero[:, 0], zero[:, 1])
# plt.scatter(ctoid[:, 0], ctoid[:, 1])
# plt.show()
