#ifndef ERROR_H
#define ERROR_H
#include <cmath>
#include <functional>
#include <vector>
#include "func.h"
double weight(double x, double a, double b);
double integralError(double a, double b, int n,
                     int k, const std::vector<double> &coef,int deg, int N = 20000) ;
double discreteError(const std::vector<double>&x, const std::vector<double>&f,double a,
    double b, int n,const std::vector<double>&coef, int deg );
#endif
