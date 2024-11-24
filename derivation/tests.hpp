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
                //std::cout << std::fixed;
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
        const auto [f, df_dx, d2f_dx2] = differentiate("x/75^x/96.8-41.7^29.5*13.8-x");

        Assert::That(df_dx({ -2.08,4.92 }),       EqualsAdaptive(value_t{ -1788.28,718.963 }), ExtraMessage("The first derivative failed! f(x) = x/75^x/96.8-41.7^29.5*13.8-x"));
       }
    {
        auto [fx,dx,dx2] = differentiate("73^57.5*log(cot(94.9*52.4)+x^55.4/x^(x/x^x/x))");
        std::complex<double> x = {4.31,-9.46};

        Assert::That( dx(x), value_t (1.38132e+108,3.07843e+108), "The first derivative failed! f(x) = 73^57.5*log(cot(94.9*52.4)+x^55.4/x^(x/x^x/x))");
    }
    {
        auto [fx,dx,dx2] = differentiate("100 - 4.9 * x^2");
        std::complex<double> x = {2,0};
        
        Assert::That(dx2(x), value_t (-9.8,0), "The second derivative failed! f(x) = 100 - 4.9 * x^2, x = 2");
    }
       {
       auto [fx,dx,dx2] = differentiate("3.14 * (1 + 5 * x) / log(x)");
       std::complex<double> x = {0,1};

       Assert::That(dx(x), value_t (6.36297,-11.2675), "The first derivative failed! 3.14 * (1 + 5 * x) / log(x), x = (0, 1)");
       }

       /*
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("x^63+58.5+19.2*38*55.2");
        Assert::That(f({-3.35,3.35}),       EqualsAdaptive(value_t{8.7584e+29,4.83113e+43}), ExtraMessage("The function failed!"));
    }
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("92.6/x^2.1*x^67.3+x");
        Assert::That(f({3.82,4.56}),       EqualsAdaptive(value_t{2.64517e+52,1.12433e+52}), ExtraMessage(""));
    }

    */
}
