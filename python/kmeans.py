import numpy as np
from scipy.cluster.vq import kmeans
import matplotlib.pyplot as plt


zero = np.array( [[6, 7], [1, 29], [3, 14], [8, 3], [18, 2], [4, 10], [5, 10], [2, 16], [7, 4], [14, 2], [12, 1], [13, 1], [10, 1], [15, 1], [9, 1], [17, 1]], dtype=float)

zero = np.array([])

zero = np.array(sorted(zero, key = lambda x: x[0]))

r0 = np.sort(zero[:,0])
# seed1 = zero[np.argmin([zero[:,0]])]
# seed2 = zero[np.argmax([zero[:,0]])]

# norm = np.std(zero, 0)
# whit = whiten(zero)

# ctoid = kmeans(zero, 3, iter = 1)[0]
# ctoid = np.array(sorted(ctoid, key = lambda x: x[0]))
# stand = np.sort(ctoid * norm)


# print(ctoid)

# plt.scatter(zero[:, 0], zero[:, 1])
# plt.scatter(ctoid[:, 0], ctoid[:, 1])
# plt.show()
