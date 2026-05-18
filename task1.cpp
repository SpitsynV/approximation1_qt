#include "task1.h"
//
#include <iostream>
//
void GetCoeficients(int n, const std::vector<double>& x, const std::vector<double>& f,
                    double a, double b, std::vector<double>& coef, int deg) {
    // lambda вычисление многочлена Чебышева
    auto chebyshevT = [](int k, double t) {
        if (k == 0) return 1.0;
        if (k == 1) return t;
        double t0 = 1.0, t1 = t;
        for (int i = 2; i <= k; ++i) {
            double t2 = 2.0 * t * t1 - t0;
            t0 = t1;
            t1 = t2;
        }
        return t1;
    };

    // Преобразуем x в t ∈ [-1, 1]
    std::vector<double> t(n);
    for (int i = 0; i < n; ++i) {
        t[i] = (2.0 * x[i] - (a + b)) / (b - a);
    }

    // Обнуляем коэффициенты
    std::fill(coef.begin(), coef.end(), 0.0);

    // Суммируем по узлам
    for (int j = 0; j < n; ++j) {
        for (int k = 0; k < deg; ++k) {
            coef[k] += f[j] * chebyshevT(k, t[j]);
        }
    }

    // Масштабирование
    for (int k = 0; k < deg; ++k) {
        if (k == 0) coef[k] /= n;
        else coef[k] *= 2.0 / n;
    }
}

    
double GetValue(double point, double a, double b, int n,const std::vector<double>& coef1,int deg ){
    double z=2.0*(2.0*point-(b+a))/(b-a);
    double value=0;
    double T0=0, T1=0;
    double T=0;
    if(deg==0)return 0;
    T0=1;
    T1=z/2;
    value=coef1[0]*T0;
    if(deg>1)value+=coef1[1]*T1;
    for(int i=2;i<=(deg-1);i++){
        T=z*T1-T0;
        value+=coef1[i]*T;
        T0=T1;
        T1=T;
    }
    return value;
}
///странно что второй и последний коэфициенты всегда нулевые