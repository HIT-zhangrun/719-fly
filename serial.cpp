#include "serial.h"
#include "serialportlist.h"
#include <QDebug>

serial_status_e Serial::serial_status = serial_close;
QByteArray *Serial::rx_data_buf_a = new QByteArray;
QByteArray *Serial::rx_data_buf_b = new QByteArray;
char Serial::use_buf = 'a';
int Serial::rx_number = 0;


Serial::Serial(QWidget *parent)
    : QSerialPort{parent}
{

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
    setBaudRate(115200);
    setDataBits(QSerialPort::Data8);
    setParity(QSerialPort::NoParity);
    setStopBits(QSerialPort::OneStop);
    setFlowControl(QSerialPort::NoFlowControl);
    serial_status = serial_open;

    QObject::connect(this, &QSerialPort::readyRead, this, &Serial::wait_read_data);
    return 0;
}

uint8_t Serial::close_serial()
{
    close();
    serial_status = serial_close;
    QObject::disconnect(this, &QSerialPort::readyRead, this, &Serial::wait_read_data);

    return 0;
}

uint8_t Serial::wait_read_data()
{
    if(rx_number > 1000)
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


