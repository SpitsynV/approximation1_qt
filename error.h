#ifndef ERROR_H
#define ERROR_H

#include <vector>
#include <functional>

double maxAbsoluteError(double a, double b,
                        const std::function<double(double)> &exactFunc,
                        const std::function<double(double)> &approxFunc,
                        int N = 10000);
double integralError(double a, double b, int n,
                     const std::function<double(double)> &exactFunc,
                     double (*approxFunc)(double, double, double, int, const std::vector<double>&, int),
                     const std::vector<double> &coef, int deg,
                     int N = 20000);

double discreteError(const std::vector<double> &x,
                     const std::vector<double> &f,
                     double a, double b, int n,
                     const std::function<double(double)> &exactFunc,
                     double (*approxFunc)(double, double, double, int, const std::vector<double>&, int),
                     const std::vector<double> &coef, int deg);

#endif