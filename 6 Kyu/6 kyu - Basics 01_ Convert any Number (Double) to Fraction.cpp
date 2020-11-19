#include <vector>
#include <cmath>

double rnd (double num) { return floor (num * 10000)  / 10000; }
class Kata {
    public:
        std::string toFraction(double number) {
            double integral;
            std::vector<int> range;
            int index, num = 1, den, temp = 0;

            do {
                integral = floor (number);
                range.push_back (integral);

                if (rnd (number) == integral)
                    break;

                number -= integral;
                number = 1 / number;

            } while (true);

            index = range.size();

            while (index-->0) {
                den = num;
                num = num * range[index] + temp;
                temp = den;
            }

            if (num == 0) return "0";
            if (den > 1) return {std::to_string(num) + "/" + std::to_string(den)};
            else return {std::to_string(num)};
        }
};
