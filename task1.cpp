#include "task1.h"
//
#include <iostream>
#include <vector>

//
void GetCoeficients(int n, const std::vector<double> &x, const std::vector<double> &f,
                    double a, double b, std::vector<double> &coef, int deg)
{

    double gij = 0.0, val = 0.0;
    double zj = 0.0;
    std::vector<double> gk0(n + 2, 0);
    std::vector<double> gk1(n + 2, 0);
    std::vector<double> z(n + 2, 0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == 0)
            {
                gij = f[j];
                gk0[j] = gij;
            }
            else if (i == 1)
            {
                double t = (2.0 * x[j] - (a + b)) / (b - a); // t ∈ [-1,1]
                zj = t;
                gij = zj * f[j];
                gk1[j] = gij;
                z[j] = 2.0 * zj;
            }
            else
            {
                if (i % 2 == 0)
                {
                    gij = z[j] * gk1[j] - gk0[j];
                    gk0[j] = gij;
                }
                else
                {
                    gij = z[j] * gk0[j] - gk1[j];
                    gk1[j] = gij;
                }
            }
            val += gij;
        }

        if (i == 0)
        {
            coef[i] = val / n;
        }
        else
        {
            coef[i] = 2.0 * val / n;
        }

        val = 0;
    }
}
double GetValue(double point, double a, double b, int n, const std::vector<double> &coef1, int deg)
{
    double z = 2.0 * (2.0 * point - (b + a)) / (b - a);
    double value = 0;
    double T0 = 0, T1 = 0;
    double T = 0;
    if (deg == 0)
        return 0;
    T0 = 1;
    T1 = z / 2;
    value = coef1[0] * T0;
    if (deg > 1)
        value += coef1[1] * T1;
    for (int i = 2; i <= (deg - 1); i++)
    {
        T = z * T1 - T0;
        value += coef1[i] * T;
        T0 = T1;
        T1 = T;
    }
    return value;
}