#include "func.h"
#include <cmath>
#include <vector>
#include <iostream>


double SecondDerivative(double x, int k) {
    switch(k) {
        case 0: return 0.0;
        case 1: return 0.0;
        case 2: return 2.0;
        case 3: return 6.0 * x;
        case 4: return 12.0 * x * x;
        case 5: return exp(x);
        case 6: {
            double denom = 25.0 * x * x + 1.0;
            return 50.0 * (75.0 * x * x - 1.0) / (denom * denom * denom);
        }
        default: throw std::invalid_argument("Error: Wrong formula number");
    }
}
void printVector(const std::vector<double>& vec, const int m) {
    for (int i = 0; i < m; ++i) {
        std::cout << std::setw(10) << std::setprecision(3) << std::scientific << vec[i] << " ";
    }
    std::cout << std::endl;
}
void initialize(std::vector<double>& x, std::vector<double>& f, int n, int k, double a, double b) {
    for (int i = 0; i < n; ++i) {
        double t = cos(M_PI * (2.0 * i + 1.0) / (2.0 * n));
        x[i] = (a + b) / 2.0 + (b - a) / 2.0 * t;
        f[i] = GetExactValue(x[i], k);
    }
}


