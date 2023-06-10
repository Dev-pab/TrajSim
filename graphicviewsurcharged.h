#ifndef GRAPHICVIEWSURCHARGED_H
#define GRAPHICVIEWSURCHARGED_H

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

class GraphicViewSurcharged : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicViewSurcharged(QWidget *parent = nullptr);

    QPoint lastMouseCoord() { return _lastMouseCoord; }

signals:
    void MousePressed(QPoint);
    void MouseReleased(QPoint);
    void MouseMooved(QPoint);
    void WheelMooved(QPoint);

protected:
    void mousePressEvent(QMouseEvent* event) override {
        event->accept();
        _lastMouseCoord.setX(event->position().x());
        _lastMouseCoord.setY(event->position().y());

        emit MousePressed(QPoint(event->position().x(), event->position().y()));

    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        event->accept();
        _lastMouseCoord.setX(event->position().x());
        _lastMouseCoord.setY(event->position().y());

        emit MouseReleased(QPoint(event->position().x(), event->position().y()));
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        event->accept();

        _lastMouseCoord.setX(event->position().x());
        _lastMouseCoord.setY(event->position().y());

        emit MouseMooved(QPoint(event->position().x(), event->position().y()));
    }

    void wheelEvent(QWheelEvent * event) override {
        event->accept();
        //_lastMouseCoord.setX(event->position().x());
        //_lastMouseCoord.setY(event->position().y());

        emit WheelMooved(event->angleDelta());
    }

private:

    QPoint _lastMouseCoord;
};

#endif // GRAPHICVIEWSURCHARGED_H
