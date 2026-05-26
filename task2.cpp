#include "task2.h"
//нужны узлы x_i, значения функции в точках f(x_i), значения второй производной в x_0, и x_n-1
void GetCoeficients2(int n, const std::vector<double> &x, const std::vector<double> &f, double a, double b, std::vector<double> &coef, const std::vector<double> &dd)
{
    std::vector<double> d(n, 0); //они индексированы 0...n-1 НЕ как в книге
    double delta1=0, delta2=0;
    double sign1=0, sign2=0;
    delta1=(f[1]-f[0])/(x[1]-x[0]);
    sign1=std::copysign(1.0, delta1);
    for(int i=1; i<n-1; i++){
        delta2=(f[i+1]-f[i])/(x[i+1]-x[i]);
        sign2=std::copysign(1.0, delta2);
        if(sign1==sign2){
            d[i]=sign2*std::min(fabs(delta1),fabs(delta2));
        }else{
            d[i]=0.0;
        }
        delta1=delta2;
        sign1=sign2;
    }
    d[0]=0.5*(-d[1]+3*((f[1]-f[0])/(x[1]-x[0]))-0.5*dd[0]*(x[1]-x[0]));
    d[n-1]=0.5*(-d[n-2]+3*((f[n-1]-f[n-2])/(x[n-1]-x[n-2]))+0.5*dd[1]*(x[n-1]-x[n-2]));
    for(int i=0; i<n-1; i++){
        coef[4*i]=f[i];
        coef[4*i+1]=d[i];
        coef[4*i+2]=(3*((f[i+1]-f[i])/(x[i+1]-x[i]))-2*d[i]-d[i+1])/(x[i+1]-x[i]);
        coef[4*i+3]=(d[i]+d[i+1]-2*((f[i+1]-f[i])/(x[i+1]-x[i])))/((x[i+1]-x[i])*(x[i+1]-x[i]));

    }

}

//Не дает точного приближения даже для x^2. Это метод такой. Квадратичная погрешность
double GetValue2(double point, const std::vector<double> &x, int n, const std::vector<double> &coef)
{
    double dx=0;
    double val=0;
    if(point<x[0]){
        dx=(point-x[0]);
        val=coef[0]+coef[1]*dx+coef[2]*dx*dx+coef[3]*dx*dx*dx;
        return val;
    }
    if(point>=x[n-1]){
        dx=(point-x[n-2]);
        val=coef[4*(n-2)]+coef[4*(n-2)+1]*dx+coef[4*(n-2)+2]*dx*dx+coef[4*(n-2)+3]*dx*dx*dx;
        return val;
    }
    //бин поиск нужного интервала
    auto it = std::upper_bound(x.begin(), x.end(), point);
    int i = std::distance(x.begin(), it) -1;
    dx=(point-x[i]);
    val=coef[4*i]+coef[4*i+1]*dx+coef[4*i+2]*dx*dx+coef[4*i+3]*dx*dx*dx;
    return val;
}
