#include "plotwidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <cmath>
#include <cstdio>
#include "func.h"
#include "task1.h"
#include "task2.h"
PlotWidget::PlotWidget(Approximator *approx, QWidget *parent)
    : QWidget(parent), m_approx(approx), m_lastMaxAbs(-1.0)
{
    setFocusPolicy(Qt::StrongFocus);
}

QSize PlotWidget::minimumSizeHint() const
{
    return QSize(400, 300);
}

QSize PlotWidget::sizeHint() const
{
    return QSize(800, 600);
}

void PlotWidget::paintEvent(QPaintEvent * /*event*/)
{
    if (!m_approx) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Получаем список функций для отрисовки
    std::vector<std::function<double(double)>> funcs;
    std::vector<QColor> colors;
    std::vector<QString> names;
    m_approx->getPlotData(funcs, colors, names);

    if (funcs.empty()) return;

    // Текущий отображаемый отрезок по X (с учётом масштаба s)
    double a0 = m_approx->a();
    double b0 = m_approx->b();
    int s = m_approx->scaleExp();
    double mid = (a0 + b0) / 2.0;
    double halfLen = (b0 - a0) / 2.0 / (1 << s); // делим на 2^s
    double xMin = mid - halfLen;
    double xMax = mid + halfLen;

    int steps = width();
    if (steps < 2) steps = 2;
    double dx = (xMax - xMin) / (steps - 1);

    // Найдём min_y и max_y среди всех функций на этом отрезке
    double minY = 0.0, maxY = 0.0;
    bool first = true;
    for (double x = xMin; x <= xMax; x += dx) {
        for (auto &func : funcs) {
            double y = func(x);
            if (std::isfinite(y)) {
                if (first) {
                    minY = maxY = y;
                    first = false;
                } else {
                    if (y < minY) minY = y;
                    if (y > maxY) maxY = y;
                }
            }
        }
    }
    if (first) return;
        // Реальные min/max функций (до отступов)
    double realMin = minY;
    double realMax = maxY;

    //максимум модуля среди реальных min/max
    double maxAbs = std::max(std::abs(realMin), std::abs(realMax));

    // Отступы
    double delta = realMax - realMin;
    double margin;
    if (delta < 1e-12) {
        margin = 0.1 * std::max(1.0, std::abs(realMax));
    } else {
        margin = 0.05 * delta; // 5% от диапазона
    }
    minY = realMin - margin;
    maxY = realMax + margin;

    // Вывод в консоль максимального по модулю значения
    if (std::abs(maxAbs - m_lastMaxAbs) > 1e-16 || m_lastMaxAbs < 0) {
    fprintf(stderr, "max|F| = %g\n", maxAbs);
    m_lastMaxAbs = maxAbs;
}

    // Функция преобразования логических координат в экранные
    auto l2g = [&](double x, double y) -> QPointF {
        double px = (x - xMin) / (xMax - xMin) * width();
        double py = (maxY - y) / (maxY - minY) * height();
        return QPointF(px, py);
    };

    // Оси
    painter.setPen(QPen(Qt::gray, 1));
    if (0.0 >= xMin && 0.0 <= xMax) {
        painter.drawLine(l2g(0.0, minY), l2g(0.0, maxY));
    }
    if (0.0 >= minY && 0.0 <= maxY) {
        painter.drawLine(l2g(xMin, 0.0), l2g(xMax, 0.0));
    }
        // === Числовые метки по осям ===
    painter.setPen(QPen(Qt::black, 1));
    QFont tickFont = painter.font();
    tickFont.setPointSize(8);
    painter.setFont(tickFont);
    const int numTicks = 5;

    // Метки оси X
    if (0.0 >= minY && 0.0 <= maxY) {
        for (int i = 0; i <= numTicks; ++i) {
            double x = xMin + (xMax - xMin) * i / numTicks;
            QPointF pt = l2g(x, 0.0);
            painter.drawLine(pt + QPointF(0, -3), pt + QPointF(0, 3));
            painter.drawText(pt + QPointF(-15, 15), QString::number(x, 'g', 3));
        }
    }

    // Метки оси Y
    if (0.0 >= xMin && 0.0 <= xMax) {
        for (int i = 0; i <= numTicks; ++i) {
            double y = minY + (maxY - minY) * i / numTicks;
            QPointF pt = l2g(0.0, y);
            painter.drawLine(pt + QPointF(-3, 0), pt + QPointF(3, 0));
            painter.drawText(pt + QPointF(-50, 4), QString::number(y, 'g', 3));
        }
    }
    // Рисуем каждый график
    for (size_t i = 0; i < funcs.size(); ++i) {
        painter.setPen(QPen(colors[i], 2));
        double prevX = xMin;
        double prevY = funcs[i](prevX);
        bool prevValid = std::isfinite(prevY);
        for (double x = xMin + dx; x <= xMax; x += dx) {
            double y = funcs[i](x);
            if (std::isfinite(y) && prevValid) {
                painter.drawLine(l2g(prevX, prevY), l2g(x, y));
            }
            prevX = x;
            prevY = y;
            prevValid = std::isfinite(y);
        }
    }

    // Текстовая информация
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    QString info;
    info += QString("k=%1 %2").arg(m_approx->k()).arg(m_approx->functionName());
    info += QString(" n=%1  s=%2  p=%3")
                .arg(m_approx->n())
                .arg(m_approx->scaleExp())
                .arg(m_approx->p());
    info += QString(" max|F|=%1").arg(maxAbs);
    painter.drawText(10, 20, info);
        // === Легенда ===
    painter.setPen(Qt::black);
    QFont legendFont = painter.font();
    legendFont.setPointSize(9);
    painter.setFont(legendFont);
    QFontMetrics fm(legendFont);

    int maxTextWidth = 0;
    for (const auto &name : names) {
        int w = fm.horizontalAdvance(name);
        if (w > maxTextWidth) maxTextWidth = w;
    }

    const int boxSize = 12;              // размер цветного квадратика
    const int textPadding = 4;          // отступ от квадратика до текста
    const int rowHeight = boxSize + 4;  // расстояние между строками легенды
    int legendX = width() - maxTextWidth - boxSize - textPadding - 10; // правый край с отступом
    int legendY = 25;                   // начинаем чуть ниже верха

    for (size_t i = 0; i < names.size(); ++i) {
        // Цветной прямоугольник
        painter.fillRect(legendX, legendY, boxSize, boxSize, colors[i]);
        painter.drawRect(legendX, legendY, boxSize, boxSize);

        // Текст подписи
        painter.drawText(legendX + boxSize + textPadding,
                         legendY + boxSize - 2,
                         names[i]);

        legendY += rowHeight;
    }
}

void PlotWidget::keyPressEvent(QKeyEvent *event)
{
    if (!m_approx) {
        QWidget::keyPressEvent(event);
        return;
    }
    bool needRebuild = false;
    bool needUpdate = true;

    switch (event->key()) {
    case Qt::Key_D:
    {
    const auto &x = m_approx->getX();
    const auto &f = m_approx->getF();
    fprintf(stderr, "=== Debug: n=%d, a=%.6f, b=%.6f ===\n", m_approx->n(), m_approx->a(), m_approx->b());
    /*
    for (int i = 0; i < m_approx->n(); ++i) {
        fprintf(stderr, "x[%d] = %12.8f, f[%d] = %12.8f\n", i, x[i], i, f[i]);
    }
        */
    fprintf(stderr, "Err1= %e\n",m_approx->getIntegralError());
    fprintf(stderr, "Max error method 2 = %e\n", m_approx->getMaxError2());
    /*
    for (int i = 0; i < m_approx->n(); ++i) {
        double exact = m_approx->f(x[i]);      // точное с учётом возмущения
        double approx2 = m_approx->approx2(x[i]);
        fprintf(stderr, "x[%d]=%.6f exact=%.6f approx2=%.6f diff=%.2e\n",
                i, x[i], exact, approx2, exact-approx2);
    }
    */
    // не меняем состояние и не перерисовываем
    return;
    }
    case Qt::Key_0:
        m_approx->nextK();
        needRebuild = true;
        break;
    case Qt::Key_1:
        m_approx->nextGraphMode();
        // графики не требуют пересчёта коэффициентов, только смены набора
        needRebuild = false;
        break;
    case Qt::Key_2:
        m_approx->setScaleExp(m_approx->scaleExp() + 1); // увеличение масштаба – больше s
        needRebuild = false;
        break;
    case Qt::Key_3:
        if (m_approx->scaleExp() > 0)
            m_approx->setScaleExp(m_approx->scaleExp() - 1);
        needRebuild = false;
        break;
    case Qt::Key_4: // увеличить n в 2 раза
    {
        int newN = m_approx->n() * 2;
        m_approx->setN(newN);
        needRebuild = true;
        break;
    }
    case Qt::Key_5: // уменьшить n в 2 раза
    {
        int newN = m_approx->n() / 2;
        if (newN < 2) newN = 2;
        m_approx->setN(newN);
        needRebuild = true;
        break;
    }
    case Qt::Key_6:
        m_approx->setP(m_approx->p() + 1);
        needRebuild = true;
        break;
    case Qt::Key_7:
        if (m_approx->p() > 0)
            m_approx->setP(m_approx->p() - 1);
        needRebuild = true;
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }

    if (needRebuild)
        m_approx->rebuild();
        /*
        double errInt = m_approx->getIntegralError();
        double errDisc = m_approx->getDiscreteError();
        double errMax= m_approx->getMaxError2();
        fprintf(stderr, "Integral error = %e, Discrete error = %e\n", errInt, errDisc);
        fprintf(stderr, "Max error = %e\n",errMax);
        */
    if (needUpdate)
        update();
}