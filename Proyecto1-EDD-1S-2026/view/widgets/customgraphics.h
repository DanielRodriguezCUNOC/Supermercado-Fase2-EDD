#ifndef CUSTOMGRAPHICS_H
#define CUSTOMGRAPHICS_H

#include <QGraphicsView>
#include <QWheelEvent>

class CustomGraphics: public QGraphicsView
{Q_OBJECT

public:
    explicit CustomGraphics(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // CUSTOMGRAPHICS_H
