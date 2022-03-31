#ifndef SERIAL_H
#define SERIAL_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

typedef enum
{
    serial_close = 0,
    serial_open,
} serial_status_e;

class Serial : public QSerialPort
{
    Q_OBJECT
public:
    explicit Serial(QWidget *parent = nullptr);
    static QStringList update_serial_list();
    static serial_status_e get_serial_status();

    uint8_t open_serial();
    uint8_t close_serial();


private:
    static serial_status_e serial_status;

    uint8_t wait_read_data();
    static QByteArray *rx_data_buf_a;
    static QByteArray *rx_data_buf_b;
    static int rx_number;
    static char use_buf;

signals:
    void receive_data_display(QByteArray *buf);

};

#endif // SERIAL_H
