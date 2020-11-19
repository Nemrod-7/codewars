#include<vector>

unsigned int countPatternsFrom(char dot, unsigned short length) {
    if (length < 0 || length > 9) return 0;

    const std::vector<int> fiv {0,1,5,31,154,684,2516,7104,13792,13792};
    const std::vector<int> sev {0,1,7,37,188,816,2926,8118,15564,15564};
    const std::vector<int> eig {0,1,8,48,256,1152,4248,12024,23280,23280};

    if (dot == 'E') return eig[length];
    if ((dot - 'A') % 2 == 0) return fiv[length];
    else return sev[length];

    return 0;
}
