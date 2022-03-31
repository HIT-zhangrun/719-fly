#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "serial.h"
#include <QThread>
#include <QTimer>
#include <QTextBrowser>

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
    uint8_t rx_serial_dispaly(void);
    void add_serial_baud();
    void get_serial_baud();
    QTimer *timer = nullptr;
    void clear_rx_data();
    void clear_tx_data();
    uint8_t pre_tx_data();
    uint8_t print_to_rx(QString buf);
    static int64_t tx_num;


    Serial *serial;
    QThread serialThread;
signals:
    uint8_t send_data_sig(QString tx_buf);
};
#endif // WIDGET_H
