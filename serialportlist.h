#ifndef SERIALPORTLIST_H
#define SERIALPORTLIST_H

#include <QWidget>
#include <QComboBox>
#include <QMouseEvent>
#include "serial.h"

class SerialPortList : public QComboBox
{
    Q_OBJECT
public:
    explicit SerialPortList(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void serial_list_display(void);
    void set_current_com(void);
    QString get_current_com(void);

private:
    static QString *current_com;

signals:

};

#endif // SERIALPORTLIST_H
