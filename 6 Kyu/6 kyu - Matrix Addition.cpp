std::vector<std::vector<int>> matrixAddition(std::vector<std::vector<int> > a,std::vector<std::vector<int> > b){
    const size_t n = a.size();
    std::vector<std::vector<int>> c (n, std::vector<int>(n));

    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < n; j++)
            c[i][j] = a[i][j] + b[i][j];

    return c;
}
