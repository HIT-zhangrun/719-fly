#include "widget.h"
#include "ui_widget.h"
#include <QIcon>
#include "serial.h"

#define MAX_DISPLAY_RX (500)
#define RX_UPDATE_TIME (100)

int64_t Widget::tx_num = 0;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->recv_data->document()->setMaximumBlockCount(MAX_DISPLAY_RX);
    add_serial_baud();

    this->setWindowTitle("719-fly-demo");
    this->setWindowIcon(QIcon(":/image/719logo.png"));

    serial = new Serial;
    connect(ui->serial_button, &QPushButton::clicked, this, &Widget::open_close_serial);

    timer = new QTimer;

    connect(timer, &QTimer::timeout, this, &Widget::rx_serial_dispaly);
    connect(ui->clear_rx, &QPushButton::clicked, this, &Widget::clear_rx_data);
    connect(ui->clear_tx, &QPushButton::clicked, this, &Widget::clear_tx_data);
    connect(ui->send_button, &QPushButton::clicked, this, &Widget::pre_tx_data);
    connect(this, &Widget::send_data_sig, serial, &Serial::send_data);

    serial->moveToThread(&serialThread);
    serialThread.start();
    ui->progress_bar->setValue(100);
    print_to_rx("系统初始化完成");
    print_to_rx("选择串口和波特率后打开");
    ui->progress_bar->setDisabled(true);
}

Widget::~Widget()
{
    serialThread.deleteLater();
    serialThread.wait();
    timer->stop();
    delete ui;
}

uint8_t Widget::open_close_serial()
{
    if(Serial::get_serial_status() == serial_close)
    {
        if(!serial->open_serial())
        {
            timer->start(RX_UPDATE_TIME);
            ui->serial_button->setText("关闭串口");
            print_to_rx("串口已打开");
            return 0;
        }
        print_to_rx("串口打开失败，检查是否选择串口");
        return 1;
    }

    if(Serial::get_serial_status() == serial_open)
    {
        timer->stop();
        serial->close_serial();
        ui->serial_button->setText("打开串口");
        print_to_rx("串口已关闭");
    }
    return 1;
}

uint8_t Widget::rx_serial_dispaly(void)
{
    timer->stop();
    QByteArray *rx_buf = serial->get_rx_data();
    timer->start(RX_UPDATE_TIME);
    if(rx_buf == nullptr)
    {
        return 1;
    }
    if(ui->is_rx_hex->isChecked())
    {
        *rx_buf = rx_buf->toHex();
    }
    ui->recv_data->insertPlainText(*rx_buf);
    rx_buf->clear();
    QTextCursor cursor=ui->recv_data->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->recv_data->setTextCursor(cursor);
    ui->rx_num->setText(serial->get_rx_num());

    return 0;
}

void Widget::add_serial_baud()
{
    QStringList serial_baud_list;
    serial_baud_list << "110" << "300" << "600" << "1200" << "2400" << "4800";
    serial_baud_list << "9600" << "14400" << "19200" << "38400" << "43000" << "57600";
    serial_baud_list << "76800" << "115200" << "128000" << "230400" << "256000" << "460800" << "921600";
    ui->serial_baud->addItems(serial_baud_list);
    ui->serial_baud->setCurrentText("115200");

    connect(ui->serial_baud, &QComboBox::activated, this, &Widget::get_serial_baud);
}

void Widget::get_serial_baud()
{
    serial->set_serial_baud(ui->serial_baud->currentText());
    print_to_rx("选择波特率");
    print_to_rx(ui->serial_baud->currentText());
}

void Widget::clear_rx_data()
{
    ui->recv_data->clear();
    ui->rx_num->setText("0");
    serial->clear_rx_num();
}

void Widget::clear_tx_data()
{
    ui->send_data->clear();
    tx_num = 0;
    ui->tx_num->setText("0");
}

uint8_t Widget::pre_tx_data()
{
    QString send_data = ui->send_data->toPlainText();
    if(send_data.length() == 0)
    {
        return 1;
    }
    if(ui->is_send_new_line->isChecked())
    {
        send_data+="\r\n";
    }

    tx_num+=send_data.length();
    ui->tx_num->setText(QString::number(tx_num));
    emit send_data_sig(send_data);
    return 0;
}

uint8_t Widget::print_to_rx(QString buf)
{
    ui->recv_data->insertPlainText("[719-fly]");
    ui->recv_data->insertPlainText(buf);
    ui->recv_data->insertPlainText("\r\n");

    return 0;
}
