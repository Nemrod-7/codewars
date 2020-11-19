std::vector<int> sqrtApproximation(double num) {

    std::vector<int> range;
    double z = 1, y = 1, epsilon = 1e-6;
    int k;
    while (y > epsilon) {
        y = (z * z - num) / (2 * z);
        z -= y;
        y = (y < 0) ? -y : y;
    }
    k = (int)z;

    range.push_back(k);
    if (k * k < num)
        range.push_back(k + 1);

    return range;
}
