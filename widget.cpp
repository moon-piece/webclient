#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // socket_.connected();
    // socket_.readyRead();
    QObject::connect(&socket_,&QAbstractSocket::connected,this,&Widget::doConnected);
    QObject::connect(&socket_,&QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&socket_,&QAbstractSocket::readyRead,this,&Widget::doReadyRead);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::doConnected() {
    ui->pteMessage->insertPlainText("Connected \n");
}

void Widget::doDisconnected() {
    ui->pteMessage->insertPlainText("Disconnected \n");
}

void Widget::doReadyRead(){
    ui->pteMessage->insertPlainText(socket_.readAll());
}

void Widget::on_pbConnect_clicked()
{
    // socket_.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort()); //SSL
    socket_.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort()); //TCP,UDP
}

void Widget::on_pbDisconnect_clicked()
{
    socket_.close();
}

void Widget::on_pbSend_clicked()
{
    socket_.write(ui->pteSend->toPlainText().toUtf8());
}

void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}

