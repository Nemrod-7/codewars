std::string sierpinski (int n) {

    const int dim = pow (3, n);
    std::string asc;

    for (int y = 0; y < dim; y++) {
        for (int x = 0; x < dim; x++) {
            bool flag = true;

            for (int i = dim / 3; i ; i /= 3) {
                if ((y % (i * 3)) / i == 1 && (x % (i * 3)) / i == 1) {
                    flag = false;
                }
            }
            asc += (flag == true ? "██" : "  ");
        }
        if (y < dim - 1) asc += "\n";
    }

    return asc;
}
