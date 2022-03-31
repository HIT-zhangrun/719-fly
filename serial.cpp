#include "serial.h"
#include "serialportlist.h"
#include <QDebug>

serial_status_e Serial::serial_status = serial_close;
QByteArray *Serial::rx_data_buf_a = new QByteArray;
QByteArray *Serial::rx_data_buf_b = new QByteArray;
QByteArray *Serial::rx_data_buf_send = new QByteArray;
char Serial::use_buf = 'a';
int64_t Serial::rx_num = 0;
Serial::Serial(QWidget *parent)
    : QSerialPort{parent}
{
    setBaudRate(115200);
}

QStringList Serial::update_serial_list()
{
    QStringList serial_name;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serial_name << info.portName() + ": " + info.description();
    }
    return serial_name;
}

serial_status_e Serial::get_serial_status()
{
    return serial_status;
}


uint8_t Serial::open_serial()
{
    SerialPortList serialportlist;

    setPortName(serialportlist.get_current_com());
    if(!open(QIODevice::ReadWrite))
    {
        return 1;
    }
    setDataBits(QSerialPort::Data8);
    setParity(QSerialPort::NoParity);
    setStopBits(QSerialPort::OneStop);
    setFlowControl(QSerialPort::NoFlowControl);
    serial_status = serial_open;

    connect(this, &QSerialPort::readyRead, this, &Serial::wait_read_data);
    return 0;
}

uint8_t Serial::close_serial()
{
    close();
    serial_status = serial_close;

    disconnect(this, &QSerialPort::readyRead, this, &Serial::wait_read_data);

    return 0;
}

uint8_t Serial::wait_read_data()
{
    if(use_buf == 'a')
    {
        rx_data_buf_a->append(readAll());
        return 0;
    }
    if(use_buf == 'b')
    {
        rx_data_buf_b->append(readAll());
        return 0;
    }

    rx_data_buf_a->clear();
    rx_data_buf_b->clear();
    use_buf = 'a';
    return 1;
}

QByteArray *Serial::get_rx_data(void)
{
    if(use_buf == 'a')
    {
        if(rx_data_buf_a->length()!=0)
        {
            use_buf = 'b';
            rx_num +=rx_data_buf_a->length();
            rx_data_buf_b->clear();
            *rx_data_buf_send = *rx_data_buf_a;
            return rx_data_buf_send;
        }
        return nullptr;
    }
    if(use_buf == 'b')
    {
        if(rx_data_buf_b->length()!=0)
        {
            use_buf = 'a';
            rx_num +=rx_data_buf_b->length();
            rx_data_buf_a->clear();
            *rx_data_buf_send = *rx_data_buf_b;
            return rx_data_buf_send;
        }
        return nullptr;
    }
    return nullptr;
}
/*
uint8_t Serial::wait_read_data()
{
    if(rx_number > 100)
    {
        rx_number = 0;
        if(use_buf == 'a')
        {
            emit receive_data_display(rx_data_buf_a);
            rx_data_buf_b->clear();
            use_buf = 'b';
            return 0;
        }
        if(use_buf == 'b')
        {
            emit receive_data_display(rx_data_buf_b);
            rx_data_buf_a->clear();
            use_buf = 'a';
            return 0;
        }
    }
    if(use_buf == 'a')
    {
        rx_data_buf_a->append(readAll());
        rx_number++;
        return 0;
    }
    if(use_buf == 'b')
    {
        rx_data_buf_b->append(readAll());
        rx_number++;
        return 0;
    }
    return 1;
}
*/
qint32 Serial::set_serial_baud(QString baud_text)
{
    setBaudRate(baud_text.toInt());
    return 0;
}

uint8_t Serial::clear_rx_num(void)
{
    rx_num =0;
    return 0;
}

QString Serial::get_rx_num(void)
{
    return QString::number(rx_num);
}

uint8_t Serial::send_data(QString tx_data)
{
    write(tx_data.toUtf8());
    return 0;
}
