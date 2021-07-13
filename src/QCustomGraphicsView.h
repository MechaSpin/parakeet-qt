/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#ifndef PARAKEET_QT_QCUSTOMGRAPHICSVIEW_H
#define PARAKEET_QT_QCUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>

class QCustomGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    QCustomGraphicsView(QWidget *parent = nullptr);

    void resetToCenterPosition();
    void setZoomTo(int millimeters);

signals:
    void onZoom();

private:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent* event);

    bool doDrag;
    QPoint lastPoint;
};

#endif