#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QColor>
#include <vector>
#include <functional>
#include "approximator.h"

class PlotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlotWidget(Approximator *approx, QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    Approximator *m_approx;

    void updateScaleAndDraw();
};

#endif // PLOTWIDGET_H