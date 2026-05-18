#ifndef TASK1_H
#define TASK1_H

#include <cmath>
#include <vector>
#include <string>
void GetCoeficients(int n, const std::vector<double>& x, const std::vector<double>& f,
                    double a, double b, std::vector<double>& coef, int deg);
double GetValue(double point, double a, double b, int n, const std::vector<double>& coef1, int deg );

#endif