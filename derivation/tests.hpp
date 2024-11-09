#include <iostream>
#include <functional>
#include <tuple>
#include <complex>

using value_t = std::complex<double>;

class Assert {
    public:

    public:
        static void That (value_t actual, value_t expect, const std::string &msg = "") {
            double epsilon = 1e-2;

            if (abs(actual - expect) > epsilon) {
                std::cout << "\n" << msg;
                std::cout << "\nexpect : " << expect ;
                std::cout << "\nactual : " << actual ;
            }
        }
};

value_t EqualsAdaptive (value_t x) { return x; }
std::string ExtraMessage (const std::string &msg) { return msg; }

using func_t = std::function<value_t(value_t)>;

std::tuple<func_t,func_t,func_t> differentiate(const std::string &);

void tests2() {
    //do_test("x", "1");
    //do_test("34", "0");

    //do_test("x + x", "2 * x");
    //do_test("2 + x", "2");

    //do_test("x - x", "0");
    //do_test("2 - x", "-1");

    //do_test("x * x", "2");
    //do_test("2 * x", "2");
    //do_test("x * 3", "3");

    //do_test("2 * x^3");
    //cout << interpret("x * 0") << "\n";
    //cout << interpret("x * 1") << "\n";
    //cout << interpret("x * 2") << "\n";
    //do_test("x ^ 0");
    //do_test("x ^ 1");
    //do_test("x ^ 2");
    //do_test("x ^ 3");
    //do_test("x ^ x");
}

void tests() {


    {
        const auto [f, df_dx, d2f_dx2] = differentiate("2 * x^3");

        Assert::That(f({ 2, 2 }),       EqualsAdaptive(value_t{ -32, 32 }), ExtraMessage("The function failed! f(x) = 2 * x^3, x = (2, 2)"));
        Assert::That(df_dx({ 2, 2 }),   EqualsAdaptive(value_t{   0, 48 }), ExtraMessage("The first derivative failed! f(x) = 2 * x^3, x = (2, 2)"));
        Assert::That(d2f_dx2({ 2, 2 }), EqualsAdaptive(value_t{  24, 24 }), ExtraMessage("The second derivative failed! f(x) = 2 * x^3, x = (2, 2)"));
    }

   {
       const auto [f, df_dx, d2f_dx2] = differentiate("sin(cos(x^x^2))");

       Assert::That(f({ 1, 1 }),       EqualsAdaptive(value_t{ 0.839472, -0.0115338 }), ExtraMessage("The function failed! f(x) = sin(cos(x^x^2)), x = (1, 1)"));
       Assert::That(df_dx({ 1, 1 }),   EqualsAdaptive(value_t{ 0.0752251, -0.0149614 }), ExtraMessage("The first derivative failed! f(x) = sin(cos(x^x^2)), x = (1, 1)"));
       Assert::That(d2f_dx2({ 1, 1 }), EqualsAdaptive(value_t{ 0.12722, 0.402059 }), ExtraMessage("The second derivative failed! f(x) = sin(cos(x^x^2)), x = (1, 1)"));
   }

   {
       const auto [f, df_dx, d2f_dx2] = differentiate("4 * log(x) + x^2 / 2^x");

       Assert::That(f({ -1, 1 }),       EqualsAdaptive(value_t{ -1.16955,   6.34782 }), ExtraMessage("The function failed! f(x) = 4 * log(x) + x^2 / 2^x, x = (-1, 1)"));
       Assert::That(df_dx({ -1, 1 }),   EqualsAdaptive(value_t{ -0.749534,  5.76558 }), ExtraMessage("The first derivative failed! f(x) = 4 * log(x) + x^2 / 2^x, x = (-1, 1)"));
       Assert::That(d2f_dx2({ -1, 1 }), EqualsAdaptive(value_t{  2.5714,  -13.8429 }), ExtraMessage("The second derivative failed! f(x) = 4 * log(x) + x^2 / 2^x, x = (-1, 1)"));
   }

   {
       const auto [f, df_dx, d2f_dx2] = differentiate("(tan(2 * x) + 1) / (cot(x * 3) - 1)");

       Assert::That(f({ 3, 1 }),       EqualsAdaptive(value_t{ -0.971754,  0.0055383 }), ExtraMessage("The function failed! f(x) = (tan(2 * x) + 1) / (cot(x * 3) - 1), x = (3, 1)"));
       Assert::That(df_dx({ 3, 1 }),   EqualsAdaptive(value_t{ -0.022166,  0.117662  }), ExtraMessage("The first derivative failed! f(x) = (tan(2 * x) + 1) / (cot(x * 3) - 1), x = (3, 1)"));
       Assert::That(d2f_dx2({ 3, 1 }), EqualsAdaptive(value_t{ -0.493051, -0.0891051 }), ExtraMessage("The second derivative failed! f(x) = (tan(2 * x) + 1) / (cot(x * 3) - 1), x = (3, 1)"));
   }

   {
        const auto [f, df_dx, d2f_dx2] = differentiate("x^63+58.5+19.2*38*55.2");
        Assert::That(df_dx({-3.35,3.35}),       EqualsAdaptive(value_t{8.7584e+29,4.83113e+43}), ExtraMessage("The function failed!"));
   }
   {
       const auto [f, df_dx, d2f_dx2] = differentiate("92.6/x^2.1*x^67.3+x");
       Assert::That(f({3.82,4.56}),       EqualsAdaptive(value_t{2.64517e+52,1.12433e+52}), ExtraMessage(""));
   }
   
}metal
