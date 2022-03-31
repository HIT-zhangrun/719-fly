#include "serialportlist.h"
#include <QDebug>
#include "widget.h"
#include "serial.h"

QString *SerialPortList::current_com = new QString;

SerialPortList::SerialPortList(QWidget *parent)
    : QComboBox{parent}
{
    this->addItem("选择串口...");
    connect(this, &QComboBox::activated, this, &SerialPortList::set_current_com);

}

void SerialPortList::serial_list_display(void)
{
    Serial serial;
    if(serial.get_serial_status() == serial_close)
    {
    this->clear();
    this->addItems(Serial::update_serial_list());
    showPopup();
    }
}

void SerialPortList::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        serial_list_display();
    }
}

void SerialPortList::set_current_com(void)
{
    *current_com = this->currentText().mid(0, this->currentText().indexOf(":"));
}

QString SerialPortList::get_current_com(void)
{
    return *current_com;
}

