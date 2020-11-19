
class Kata {
    public:
        static bool validate(long long int number) {
            int dig, cnt = 0, sum = 0;
            while (number) {
                dig = number % 10;
                number /= 10;

                if (cnt++ % 2) dig *= 2;
                if (dig > 9) dig -= 9;

                sum += dig;
            }
            return sum % 10 == false;
        }
};
