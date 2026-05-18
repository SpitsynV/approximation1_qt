#include <QApplication>
#include "plotwidget.h"
#include "approximator.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[])
{
    // Обязательные параметры командной строки
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " a b n k" << std::endl;
        return EXIT_FAILURE;
    }
    double a = std::stod(argv[1]);
    double b = std::stod(argv[2]);
    int n = std::stoi(argv[3]);
    int k = std::stoi(argv[4]);

    QApplication app(argc, argv);

    Approximator approx(a, b, n, k);
    approx.rebuild();

    PlotWidget plot(&approx);
    plot.setWindowTitle("Approximation Plot");
    plot.show();

    return app.exec();
}