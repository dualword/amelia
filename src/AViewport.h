#ifndef AVIEWPORT_H
#define AVIEWPORT_H

#include <QPushButton>
#include <QPixmap>

#include "AGeometry.h"

class AViewport : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(int viewport READ viewport)
public:
    AViewport(QWidget* parent=0);
    ~AViewport();

    void setViewport(int _cam);
    int viewport() const;

public slots:
    void handleViewportSwitch(int from,int to);
    void handleViewportUpdate(int cam,QImage newshot);
    void update();

signals:
    void requestViewportSwitch(int to);
    void requestUpdate(int cam);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    int _viewport;
    QPixmap cachedNormal,cachedHighlighted;

    QImage highlightSkin,viewportSkin;
};

#endif // AVIEWPORT_H
