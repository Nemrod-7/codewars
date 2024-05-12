class matrix {
    private :
        std::vector<std::vector<int>> mat;
    public :
        matrix (vector<vector<int>> src) {
            mat = src;
        }

        vector<vector<uint64_t>> multiply (const vector<vector<uint64_t>> &a, const vector<vector<uint64_t>> &b){

            uint64_t n = a.size();
            vector<vector<uint64_t>> mat (n, vector<uint64_t>(n));

            for (size_t i = 0; i < n; ++i) {
                for (size_t j = 0; j < n; ++j) {
                    for (size_t k = 0; k < n; ++k) {
                        mat[i][j] += (a[i][k] * b[k][j]);
                    }
                }
            }

            return mat;
        }
        vector<vector<uint64_t>> power (vector<vector<uint64_t>> a, uint64_t exp) {

            uint64_t n = size(a), m = size(a[0]);
            vector<vector<uint64_t>> b (n, vector<uint64_t>(n)); // We need to start with identity matrix because (a^0 = Identity Matrix)

            for (uint64_t i = 0; i < n; i++) b[i][i] = 1;

            while (exp > 0) {
                if (exp & 1) b = multiply(b, a);

                a = multiply(a, a);
                exp >>= 1;
            }

            return b;
        }

        vector<vector<uint64_t>> fibonacci2 (uint64_t n) {
            const vector<vector<uint64_t>> F = {{1,1},{1,0}};
            vector<vector<uint64_t>> m1 = power(F,n), m2 = {{0},{1}};
            vector<vector<uint64_t>> fib = multiply(m1, m2);

            return fib;
        }

};

