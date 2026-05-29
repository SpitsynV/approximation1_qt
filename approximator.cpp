//Погрешность квадратично зависит от n


#include "approximator.h"
#include "func.h"        
#include "task1.h"     
#include "task2.h"  
#include <cmath>
#include <algorithm>
#include <qcolor.h>

Approximator::Approximator(double a, double b, int n, int k)
    : m_a(a), m_b(b), m_n(n), m_k(k), m_p(0), m_scale(0), m_graphMode(0)
{
    m_deg = m_n;
    m_x.resize(m_n);
    m_f.resize(m_n);
    m_coef1.resize(m_deg, 0.0);
    m_coef2.resize(4*(m_n-1), 0.0);
    initGrid();
}
double Approximator::getIntegralError() const
{
    if (m_n > 50) return -1.0;
    // Передаём нашу функцию с возмущением = this->f
    return integralError(m_a, m_b, m_n,
                         [this](double x) { return this->f(x); },
                         GetValue,
                         m_coef1, m_deg);
}

double Approximator::getDiscreteError() const
{
    if (m_n > 50) return -1.0;
    return discreteError(m_x, m_f, m_a, m_b, m_n,
                         [this](double x) { return this->f(x); },
                         GetValue,
                         m_coef1, m_deg);
}
/*double Approximator::getMaxError1() const
{
    if (m_n > 50) return -1.0;
    return maxAbsoluteError(m_a, m_b,
        [this](double x) { return this->f(x); },
        [this](double x) { return this->approx1(x); });
}
*/
double Approximator::getMaxError2() const
{
    if (m_n < 2) return -1.0;
    return maxAbsoluteError(m_x[0], m_x[m_n-1],
        [this](double x) { return this->f(x); },
        [this](double x) { return this->approx2(x); });
}
void Approximator::setN(int n)
{
    m_n = n;
    m_deg = std::min(m_n, 50);
    m_x.resize(m_n);
    m_f.resize(m_n);
    m_coef1.resize(m_deg, 0.0);
    m_coef2.resize(4 * (m_n - 1), 0.0);
}

void Approximator::setK(int k)
{
    m_k = k;
}

void Approximator::nextK()
{
    m_k = (m_k + 1) % 7; // 0..6
}

void Approximator::nextGraphMode()
{
    m_graphMode = (m_graphMode + 1) % 5; // 0..4
}

QString Approximator::functionName() const
{
    switch(m_k) {
    case 0: return "f(x) = 1";
    case 1: return "f(x) = x";
    case 2: return "f(x) = x^2";
    case 3: return "f(x) = x^3";
    case 4: return "f(x) = x^4";
    case 5: return "f(x) = e^x";
    case 6: return "f(x) = 1/(25x^2+1)";
    }
    return "";
}

void Approximator::initGrid()
{
    // заполняем m_x и m_f как раньше
    for (int i = 0; i < m_n; i++) {
        double t = cos(M_PI * (2.0 * (m_n - 1 - i) + 1.0) / (2.0 * m_n));
        m_x[i] = (m_a + m_b) / 2.0 + (m_b - m_a) / 2.0 * t;
        m_f[i] = GetExactValue(m_x[i], m_k);
    }
    //std::reverse(m_x.begin(), m_x.end());
    //std::reverse(m_f.begin(), m_f.end());
    // заполнить вторые производные
        double f0dd=SecondDerivative(m_x[0],m_k);
        double f1dd=SecondDerivative(m_x[(m_n-1)],m_k);
        m_dd.assign({f0dd, f1dd});
    

    // запоминаем max|f| исходной функции
    m_maxAbsF = 0.0;
    for (double val : m_f)
        m_maxAbsF = std::max(m_maxAbsF, std::abs(val));
    // возмущение
    if (m_p != 0 && m_n > 0) {
        int mid = m_n / 2;
        m_f[mid] += m_p * 0.1 * m_maxAbsF;
    }
}

void Approximator::rebuild()
{
    m_deg=std::min(m_n,50);
    m_x.resize(m_n);
    m_f.resize(m_n);
    m_coef1.resize(m_deg, 0.0);
    m_coef2.resize(4*(m_n-1), 0.0);
    initGrid();
    //std::fill(m_coef1.begin(), m_coef1.end(), 0.0);
    if(m_n<=50) GetCoeficients(m_n, m_x, m_f, m_a, m_b, m_coef1, m_deg);
    GetCoeficients2(m_n, m_x, m_f, m_a, m_b, m_coef2, m_dd);
}

double Approximator::f(double x) const
{
    double y = GetExactValue(x, m_k);
    if (m_p != 0 && m_n > 0) {
        y += m_p * 0.1 * m_maxAbsF;
    }
    return y;
}
double Approximator::approx1(double x) const
{
    if (m_n > 50) return 0.0; // требование задания
    return GetValue(x, m_a, m_b, m_n, m_coef1, m_deg);
}

double Approximator::approx2(double x) const
{
   
    //debug
    //for(int i=0; i<4*(m_n-1); i++){
    //    fprintf(stdout,"coef2[%d]= %lf \n", i, m_coef2[i]);
    //}
    //
    return GetValue2(x, m_x, m_n, m_coef2);
}

void Approximator::getPlotData(
    std::vector<std::function<double(double)>> &funcs,
    std::vector<QColor> &colors,
    std::vector<QString> &names) const
{
    funcs.clear(); colors.clear(); names.clear();
    // Вспомогательные цвета
    QColor colorF(Qt::blue);
    QColor color1(Qt::red);
    QColor color2(Qt::darkGreen);

    switch (m_graphMode) {
    case 0: // только функция
        funcs.push_back([this](double x){ return this->f(x); });
        colors.push_back(colorF);
        names.push_back("f(x)");
        break;
    case 1: // f + метод 1
        funcs.push_back([this](double x){ return this->f(x); });
        colors.push_back(colorF);
        names.push_back("f(x)");
        funcs.push_back([this](double x){ return this->approx1(x); });
        colors.push_back(color1);
        names.push_back("Approx1");
        break;
    case 2: // f + метод 2
        funcs.push_back([this](double x){ return this->f(x); });
        colors.push_back(colorF);
        names.push_back("f(x)");
        funcs.push_back([this](double x){ return this->approx2(x); });
        colors.push_back(color2);
        names.push_back("Approx2");
        break;
    case 3: // f + метод1 + метод2
        funcs.push_back([this](double x){ return this->f(x); });
        colors.push_back(colorF);
        names.push_back("f(x)");
        funcs.push_back([this](double x){ return this->approx1(x); });
        colors.push_back(color1);
        names.push_back("Approx1");
        funcs.push_back([this](double x){ return this->approx2(x); });
        colors.push_back(color2);
        names.push_back("Approx2");
        break;
    case 4: // Разности истинной фуункции и методов 1 и 2
        funcs.push_back([this](double x){ return this->approx1(x) - this->f(x); });
        colors.push_back(color1);
        names.push_back("Error1");
        funcs.push_back([this](double x){ return this->approx2(x) - this->f(x); });
        colors.push_back(color2);
        names.push_back("Error2");
        break;
    }
}