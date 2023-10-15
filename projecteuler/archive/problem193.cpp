#include "base.hpp"
#include "ntheory.hpp"

int main () {

    // problem 193 sum of squarefree numbers

    i64 limit = 1L << 50;

    i64 mlim = sqrt(limit);
    i64 sum = 0;
    auto moebius = mu(mlim);

    for (i64 j = 1; j < mlim; j++) {
	sum += moebius[j] * limit / (j * j);
    }


}






