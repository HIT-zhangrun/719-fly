#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "serial.h"
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    uint8_t open_close_serial(void);
    void rx_serial_dispaly(QByteArray *buf);
    Serial *serial;
    QThread serialThread;
};
#endif // WIDGET_H
