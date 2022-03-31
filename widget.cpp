#include "widget.h"
#include "ui_widget.h"
#include <QIcon>
#include "serial.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->serial_display->document()->setMaximumBlockCount(2000);

    this->setWindowTitle("719-fly-demo");
    this->setWindowIcon(QIcon(":/image/719logo.png"));

    serial = new Serial;
    serial->moveToThread(&serialThread);

    connect(ui->serial_button, &QPushButton::clicked, this, &Widget::open_close_serial);
    connect(serial, &Serial::receive_data_display, this, &Widget::rx_serial_dispaly);
    serialThread.start();

}

Widget::~Widget()
{
    delete ui;
}

uint8_t Widget::open_close_serial()
{
    if(Serial::get_serial_status() == serial_close)
    {
        if(!serial->open_serial())
        {
            ui->serial_button->setText("关闭串口");
            return 0;
        }
        return 1;
    }

    if(Serial::get_serial_status() == serial_open)
    {
        serial->close_serial();
        ui->serial_button->setText("打开串口");
    }
    return 1;
}

void Widget::rx_serial_dispaly(QByteArray *buf)
{
    ui->serial_display->insertPlainText(*buf);
    QTextCursor cursor=ui->serial_display->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->serial_display->setTextCursor(cursor);
}
