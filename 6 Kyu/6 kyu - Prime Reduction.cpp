#include <iostream>
#include <vector>

bool is_prime (int num) {

    if (num < 2) return false;
    if (num < 4) return true;
    if (num % 2 == 0 || num % 3 == 0 ) return false;

    for (int i = 5; i * i <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0)
            return false;

    return true;
}
int reduce (int num) {
		int sum = 0;

		do {
				int dig = num % 10;
				sum += (dig * dig);
		}	while (num /= 10);

		return sum;
}
int solve (int a, int b) {

    int cnt = 0;
    if (a % 2 == 0) a += 1;

  	for (int i = a; i < b; i += 2) {
        if (is_prime(i) == true) {
            int num = i;
            for (int j = 0; j < 10; j++) {
                num = reduce (num);
            }

            if (num == 1) cnt++;
        }
		}

    return cnt;
}

int main () {


   // std::cout <<solve(1,25) << "\n";
   std::cout <<solve(100,1000) << "\n"; // (28));
   std::cout <<solve(100,2000) << "\n"; // (47));
   std::cout << solve(100, 3000) << "\n"; // (65));
   std::cout <<solve(100,4000) << "\n"; // (95));

}
