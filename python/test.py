import numpy as np
from scipy.cluster.vq import whiten,kmeans
import matplotlib.pyplot as plt

points = np.array( [[6, 7], [1, 29], [3, 14], [8, 3], [18, 2], [4, 10], [5, 10], [2, 16], [7, 4], [14, 2], [12, 1], [13, 1], [10, 1], [15, 1], [9, 1], [17, 1]], dtype=float)

# data = sorted(points, key = lambda x: x[0])
# seed1 = points[np.argmax([points[:,0]])]
# seed2 = points[np.argmin([points[:,0]])]

norm = np.std(points, 0)
whit = whiten(points)

ctoid = kmeans(points, 3)[0]
stand = np.sort(ctoid * norm)

print(points)
print(ctoid)

# plt.scatter(points[:, 0], points[:, 1])
# plt.scatter(ctoid[:, 0], ctoid[:, 1])
# plt.show()
