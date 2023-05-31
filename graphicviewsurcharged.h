#ifndef GRAPHICVIEWSURCHARGED_H
#define GRAPHICVIEWSURCHARGED_H


#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>

class GraphicViewSurcharged : public QGraphicsView
{
public:
    GraphicViewSurcharged();

protected:
    void mousePressEvent(QMouseEvent* event) override {
        qDebug() << "Mouse Pressed: %f, %f" << event->position().x() << event->position().y();
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        qDebug() << "Mouse Released: %d, %f" << event->position().x() << event->position().y();
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        qDebug() << "Mouse Moved: %d, %d" << event->position().x() << event->position().y();
    }
};

#endif // GRAPHICVIEWSURCHARGED_H
