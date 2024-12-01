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
        
        Assert::That(f(2),       EqualsAdaptive(value_t{ 16 }), ExtraMessage("The function failed! f(x) = 2 * x^3, x = 2"));
        Assert::That(df_dx(2),   EqualsAdaptive(value_t{ 24 }), ExtraMessage("The first derivative failed! f(x) = 2 * x^3, x = 2"));
        Assert::That(d2f_dx2(2), EqualsAdaptive(value_t{ 24 }), ExtraMessage("The second derivative failed! f(x) = 2 * x^3, x = 2"));
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
        const auto [f, df_dx, d2f_dx2] = differentiate("13.37");
        
        Assert::That(f(0),       EqualsAdaptive(value_t{ 13.37 }), ExtraMessage("The function failed! f(x) = 13.37, x = 0"));
        Assert::That(df_dx(0),   EqualsAdaptive(value_t{     0 }), ExtraMessage("The first derivative failed! f(x) = 13.37, x = 0"));
        Assert::That(d2f_dx2(0), EqualsAdaptive(value_t{     0 }), ExtraMessage("The second derivative failed! f(x) = 13.37, x = 0"));
    }
  
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("x ^ x ^ x");
      
        Assert::That(f(2),       EqualsAdaptive(value_t{      16 }), ExtraMessage("The function failed! f(x) = x ^ x ^ x, x = 2"));
        Assert::That(df_dx(2),   EqualsAdaptive(value_t{  107.11 }), ExtraMessage("The first derivative failed! f(x) = x ^ x ^ x, x = 2"));
        Assert::That(d2f_dx2(2), EqualsAdaptive(value_t{ 958.755 }), ExtraMessage("The second derivative failed! f(x) = x ^ x ^ x, x = 2"));
    }

    {
        {
            const auto [f, df_dx, d2f_dx2] = differentiate("0 * x");
            
            Assert::That(f(1),       EqualsAdaptive(value_t{ 0 }), ExtraMessage("The function failed! f(x) = 0 * x, x = 1"));
            Assert::That(df_dx(1),   EqualsAdaptive(value_t{ 0 }), ExtraMessage("The first derivative failed! f(x) = 0 * x, x = 1"));
            Assert::That(d2f_dx2(1), EqualsAdaptive(value_t{ 0 }), ExtraMessage("The second derivative failed! f(x) = 0 * x, x = 1"));
        }
      
        {
            const auto [f, df_dx, d2f_dx2] = differentiate("x^0");
            
            Assert::That(f(2),       EqualsAdaptive(value_t{ 1 }), ExtraMessage("The function failed! f(x) = x^0, x = 2"));
            Assert::That(df_dx(2),   EqualsAdaptive(value_t{ 0 }), ExtraMessage("The first derivative failed! f(x) = x^0, x = 2"));
            Assert::That(d2f_dx2(2), EqualsAdaptive(value_t{ 0 }), ExtraMessage("The second derivative failed! f(x) = x^0, x = 2"));
        }

        {
            const auto [f, df_dx, d2f_dx2] = differentiate("cos(x * 0)");
            
            Assert::That(f(2),       EqualsAdaptive(value_t{ 1 }), ExtraMessage("The function failed! f(x) = cos(x * 0), x = 2"));
            Assert::That(df_dx(2),   EqualsAdaptive(value_t{ 0 }), ExtraMessage("The first derivative failed! f(x) = cos(x * 0), x = 2"));
            Assert::That(d2f_dx2(2), EqualsAdaptive(value_t{ 0 }), ExtraMessage("The second derivative failed! f(x) = cos(x * 0), x = 2"));
        }      
    }
  
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("100 - 4.9 * x^2");
        
        Assert::That(f(2),       EqualsAdaptive(value_t{  80.4 }), ExtraMessage("The function failed! f(x) = 100 - 4.9 * x^2, x = 2"));
        Assert::That(df_dx(2),   EqualsAdaptive(value_t{ -19.6 }), ExtraMessage("The first derivative failed! f(x) = 100 - 4.9 * x^2, x = 2"));
        Assert::That(d2f_dx2(2), EqualsAdaptive(value_t{  -9.8 }), ExtraMessage("The second derivative failed! f(x) = 100 - 4.9 * x^2, x = 2"));
    }
  
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("3.14 * x^2");
        
        Assert::That(f(3),       EqualsAdaptive(value_t{ 28.26 }), ExtraMessage("The function failed! f(x) = 3.14 * x^2, x = 3"));
        Assert::That(df_dx(3),   EqualsAdaptive(value_t{ 18.84 }), ExtraMessage("The first derivative failed! f(x) = 3.14 * x^2, x = 3"));
        Assert::That(d2f_dx2(3), EqualsAdaptive(value_t{  6.28 }), ExtraMessage("The second derivative failed! f(x) = 3.14 * x^2, x = 3"));
    }
  
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("1 / x^(1/2)");
        
        Assert::That(f(4),       EqualsAdaptive(value_t{  0.5 }), ExtraMessage("The function failed! f(x) = 1 / x^(1/2), x = 4"));
        Assert::That(df_dx(4),   EqualsAdaptive(value_t{ -0.0625 }), ExtraMessage("The first derivative failed! f(x) = 1 / x^(1/2), x = 4"));
        Assert::That(d2f_dx2(4), EqualsAdaptive(value_t{  0.0234375 }), ExtraMessage("The second derivative failed! f(x) = 1 / x^(1/2), x = 4"));
    }
  
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("cos(x) + 4 * sin(x * 2)");
        
        Assert::That(f(3.14),       EqualsAdaptive(value_t{ -1.01274 }), ExtraMessage("The function failed! f(x) = cos(x) + 4 * sin(x * 2), x = 3.14"));
        Assert::That(df_dx(3.14),   EqualsAdaptive(value_t{  7.99837 }), ExtraMessage("The first derivative failed! f(x) = cos(x) + 4 * sin(x * 2), x = 3.14"));
        Assert::That(d2f_dx2(3.14), EqualsAdaptive(value_t{  1.05096 }), ExtraMessage("The second derivative failed! f(x) = cos(x) + 4 * sin(x * 2), x = 3.14"));
    }
  
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("(x + 3) / (1 - x)");
        
        Assert::That(f(-2),       EqualsAdaptive(value_t{ 1. / 3  }), ExtraMessage("The function failed! f(x) = (x + 3) / (1 - x), x = -2"));
        Assert::That(df_dx(-2),   EqualsAdaptive(value_t{ 4. / 9  }), ExtraMessage("The first derivative failed! (x + 3) / (1 - x), x = -2"));
        Assert::That(d2f_dx2(-2), EqualsAdaptive(value_t{ 8. / 27 }), ExtraMessage("The second derivative failed! (x + 3) / (1 - x), x = -2"));
    }
  
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("3^x * log(x)");
        
        Assert::That(f({ 1.6, -1.6 }),       EqualsAdaptive(value_t{ -5.35592, -3.80648 }), ExtraMessage("The function failed! f(x) = 3^x * log(x), x = (1.6, -1.6)"));
        Assert::That(df_dx({ 1.6, -1.6 }),   EqualsAdaptive(value_t{ -4.44022, -6.29953 }), ExtraMessage("The first derivative failed! 3^x * log(x), x = (1.6, -1.6)"));
        Assert::That(d2f_dx2({ 1.6, -1.6 }), EqualsAdaptive(value_t{ -4.40482, -9.03668 }), ExtraMessage("The second derivative failed! 3^x * log(x), x = (1.6, -1.6)"));
    }
  
    {
        const auto [f, df_dx, d2f_dx2] = differentiate("3.14 * (1 + 5 * x) / log(x)");
        
        Assert::That(f({ 0, 1 }),       EqualsAdaptive(value_t{  9.99493, -1.99899 }), ExtraMessage("The function failed! f(x) = 3.14 * (1 + 5 * x) / log(x), x = (0, 1)"));
        Assert::That(df_dx({ 0, 1 }),   EqualsAdaptive(value_t{  6.36297, -11.2675 }), ExtraMessage("The first derivative failed! 3.14 * (1 + 5 * x) / log(x), x = (0, 1)"));
        Assert::That(d2f_dx2({ 0, 1 }), EqualsAdaptive(value_t{  9.37418, -7.98329 }), ExtraMessage("The second derivative failed! 3.14 * (1 + 5 * x) / log(x), x = (0, 1)"));
    }

    /*
    */
}
