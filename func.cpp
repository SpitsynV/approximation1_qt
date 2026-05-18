#include "func.h"
#include <cmath>
#include <vector>
#include <iostream>



void printVector(const std::vector<double>& vec, const int m) {
    for (int i = 0; i < m; ++i) {
        std::cout << std::setw(10) << std::setprecision(3) << std::scientific << vec[i] << " ";
    }
    std::cout << std::endl;
}
void initialize(std::vector<double>& x, std::vector<double>& f, int n, int k, double a, double b) {
    for (int i = 0; i < n; ++i) {
        // Корни многочлена Чебышёва T_n(t) на [-1, 1]
        double t = cos(M_PI * (2.0 * i + 1.0) / (2.0 * n));
        // Преобразование к отрезку [a, b]
        x[i] = (a + b) / 2.0 + (b - a) / 2.0 * t;
        f[i] = GetExactValue(x[i], k);
    }
}


