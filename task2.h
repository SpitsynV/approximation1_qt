#ifndef TASK2_H
#define TASK2_H

#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
void GetCoeficients2(int n, const std::vector<double> &x, const std::vector<double> &f, double a,
     double b, std::vector<double> &coef, const std::vector<double> &dd);
double GetValue2(double point, const std::vector<double> &x, int n, const std::vector<double> &coef);

#endif