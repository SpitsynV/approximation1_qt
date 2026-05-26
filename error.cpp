#include <cmath>
#include <functional>
#include <vector>
#include "task1.h"
#include "func.h"


double weight(double x, double a, double b) {
    return 1.0 / std::sqrt((b - x) * (x - a));
}

double integralError(double a, double b, int n,
                     const std::function<double(double)> &exactFunc,
                     double (*approxFunc)(double, double, double, int, const std::vector<double>&, int),
                     const std::vector<double> &coef, int deg, int N=20000)
{
    double mid = (a + b) / 2.0;
    double half = (b - a) / 2.0;
    double h = M_PI / N;
    double sum = 0.0;
    double theta,x,exact,approx,diff,w_simpson;
    for (int i = 0; i <= N; ++i) {
        theta = i * h;
        x = mid + half * cos(theta);
        exact = exactFunc(x);
        approx = approxFunc(x, a, b, n, coef, deg);
        diff = exact - approx;
        w_simpson = (i == 0 || i == N) ? 1.0 : (i % 2 == 0 ? 2.0 : 4.0);
        sum += w_simpson * diff * diff;
    }
    return std::sqrt(h / 3.0 * sum);
}

double discreteError(const std::vector<double> &x,
                     const std::vector<double> &f,
                     double a, double b, int n,
                     const std::function<double(double)> &exactFunc,
                     double (*approxFunc)(double, double, double, int, const std::vector<double>&, int),
                     const std::vector<double> &coef, int deg)
{
    double err = 0.0;
    for (int i = 0; i < n; ++i) {
        double diff = f[i] - approxFunc(x[i], a, b, n, coef, deg);
        err += diff * diff;
    }
    return std::sqrt(err);
}