#ifndef APPROXIMATOR_H
#define APPROXIMATOR_H

#include <vector>
#include <functional>
#include <QString>
#include <QColor>
#include "error.h"
class Approximator
{
public:
    Approximator(double a, double b, int n, int k);
    double getIntegralError() const;
    double getDiscreteError() const;
    //double getMaxError1() const;
    double getMaxError2() const;
    // Пересчитать всё при изменении параметров
    void rebuild();

    // Значения функций в точке x
    double f(double x) const;        // исходная функция
    double approx1(double x) const;  // метод 1 (многочлен)
    double approx2(double x) const;  // метод 2 (кусочный) 

    // Геттеры/сеттеры параметров
    double a() const { return m_a; }
    double b() const { return m_b; }
    int n() const { return m_n; }
    int k() const { return m_k; }
    int p() const { return m_p; }
    int scaleExp() const { return m_scale; }
    int graphMode() const { return m_graphMode; }


    void setA(double a) { m_a = a; }
    void setB(double b) { m_b = b; }
    void setN(int n);
    void setK(int k);
    void setP(int p) { m_p = p; }
    void setScaleExp(int s) { m_scale = s; }
    void setGraphMode(int mode) { m_graphMode = mode; }

    // Циклический перебор k и graphMode
    void nextK();
    void nextGraphMode();

    // Текстовое представление функции (для вывода в окне)
    QString functionName() const;

    const std::vector<double>& getX() const { return m_x; }
    const std::vector<double>& getF() const { return m_f; }
    // Получить набор функций для отрисовки в зависимости от graphMode
    // Возвращает список пар (функция, цвет, имя)
    void getPlotData(
        std::vector<std::function<double(double)>> &funcs,
        std::vector<QColor> &colors,
        std::vector<QString> &names
    ) const;

private:
    double m_a, m_b;
    int m_n, m_k;
    int m_p;          // возмущение
    int m_scale;      // показатель степени зума (s)
    int m_graphMode;  // 0..4
    double m_maxAbsF; // max|f| до возмущения
    // Сетка и коэффициенты
    std::vector<double> m_x;        // точки приближения (корни Чебышёва)
    std::vector<double> m_f;        // значения функции в этих точках
    std::vector<double> m_coef1;    // коэффициенты метода 1
    int m_deg;                      // степень многочлена = n-1 (или n)
    // Для метода 2 
    std::vector<double> m_coef2;    // коэффициенты метода 2. Их 4(n-1)
    std::vector<double> m_dd;       // для вторых производных

    void initGrid();                // заполнить m_x и m_f
};

#endif // APPROXIMATOR_H