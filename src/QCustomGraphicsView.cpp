/*
	Copyright 2021 OpenJAUS, LLC (dba MechaSpin). Subject to the MIT license.
*/

#include "QCustomGraphicsView.h"

#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include <QWheelEvent>
#include <QGraphicsItem>

QCustomGraphicsView::QCustomGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    doDrag = false;
}

void QCustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(doDrag && !lastPoint.isNull())
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - lastPoint.x()));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - lastPoint.y()));
    }

    lastPoint = event->pos();
}

void QCustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        doDrag = true;
        lastPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }

    event->ignore();
}

void QCustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        doDrag = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }

    event->ignore();
}

void QCustomGraphicsView::wheelEvent(QWheelEvent* event)
{
    if(event->angleDelta().y() > 0)
    {
       scale(1.15, 1.15);
    }
    else if(event->angleDelta().y() < 0)
    {
       scale(0.85, 0.85);
    }
    else
    {
        return;
    }

    emit onZoom();
}

void QCustomGraphicsView::resetToCenterPosition()
{
    transform().reset();
}

void QCustomGraphicsView::setZoomTo(int millimeters)
{
    double half = millimeters / 2;
    fitInView(-half, -half, millimeters, millimeters, Qt::KeepAspectRatio);

    emit onZoom();
}
