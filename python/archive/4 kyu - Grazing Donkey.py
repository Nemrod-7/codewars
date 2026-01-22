import numpy as np

def sqr(x) : return x * x

def get_rope_length (diameter, ratio) :
    if ratio == 1.0 : return diameter
    if diameter == 0.0 or ratio == 0.0 : return 0.0

    low, high = 0, diameter
    r1, D = diameter * 0.5, diameter * 0.5
    limit = np.pi * D * D * ratio

    print(limit)
    while high - low > 1 :
        r2 = int((high + low) * 0.5)
        theta1 = np.arccos( ( sqr(D) + sqr(r1) - sqr(r2)) / (2.0 * D * r1)) * 2.0
        theta2 = np.arccos( ( sqr(D) + sqr(r2) - sqr(r1)) / (2.0 * D * r2)) * 2.0
        area1 = sqr(r1) * (theta1 - np.sin(theta1)) * 0.5
        area2 = sqr(r2) * (theta2 - np.sin(theta2)) * 0.5

        if area1 + area2 <= limit :
            low = r2
        elif (area1 + area2 > limit) :
            high = r2

    return low

rope = get_rope_length(200, 0.5) # 115
print(rope)
