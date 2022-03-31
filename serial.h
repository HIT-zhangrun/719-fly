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
    uint8_t wait_read_data();
    QByteArray *get_rx_data(void);
    qint32 set_serial_baud(QString baud_text);
    uint8_t clear_rx_num(void);
    QString get_rx_num(void);
    uint8_t send_data(QString tx_data);
private:
    static serial_status_e serial_status;
    static QByteArray *rx_data_buf_a;
    static QByteArray *rx_data_buf_b;
    static QByteArray *rx_data_buf_send;
    static char use_buf;
    static int64_t rx_num;
signals:

};

#endif // SERIAL_H
