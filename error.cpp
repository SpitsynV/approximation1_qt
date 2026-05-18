#include <cmath>
#include <functional>
#include <vector>
#include "task1.h"
#include "func.h"


double weight(double x, double a, double b) {
    return 1.0 / std::sqrt((b - x) * (x - a));
}

// функция для вычисления интеграла методом Симпсона
double integralError(double a, double b, int n,
                     int k, const std::vector<double>& coef, int deg,
                     int N = 20000)  // N должно быть чётным
{
    double mid = (a + b) / 2.0;
    double half = (b - a) / 2.0;
    double h = M_PI / N;               // шаг по θ. Плохо работает для быстро колеб ф-ций
    double sum = 0.0;

    for (int i = 0; i <= N; ++i) {
        double theta = i * h;
        double x = mid + half * cos(theta);
        double exact = GetExactValue(x, k);
        double diff = exact - GetValue(x, a, b, n, coef, deg);
        double w_simpson = (i == 0 || i == N) ? 1.0 : (i % 2 == 0 ? 2.0 : 4.0);
        sum += w_simpson * diff * diff;
    }

    double integral = (h / 3.0) * sum;   // ∫ (diff)^2 dθ
    return std::sqrt(integral);          // норма ошибки
}
double discreteError(const std::vector<double>&x, const std::vector<double>&f,double a,double b, int n,const std::vector<double>&coef, int deg ){
    double err=0;
    for(int i=0;i<n;i++){
        err+= pow(f[i] - GetValue(x[i],a,b,n,coef,deg),2);

    }
    err=sqrt(err);
    return err;
}