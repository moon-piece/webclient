#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // socket_.connected();
    // socket_.readyRead();
    ui->pbDisconnect->setEnabled(false);
    QObject::connect(&socket_tcp,&QAbstractSocket::connected,this,&Widget::doConnected);
    QObject::connect(&socket_tcp,&QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&socket_tcp,&QAbstractSocket::readyRead,this,&Widget::doReadyRead);
    QObject::connect(&socket_ssl,&QAbstractSocket::connected,this,&Widget::doConnected);
    QObject::connect(&socket_ssl,&QAbstractSocket::disconnected,this,&Widget::doDisconnected);
    QObject::connect(&socket_ssl,&QAbstractSocket::readyRead,this,&Widget::doReadyRead);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::changeEnabled(bool able){
    ui->pbConnect->setEnabled(false);
    // ui->pbConnect->setEnabled(socket_ssl.state() != QAbstractSocket::ConnectedState);
    // ui->pbDisconnect->setEnabled(able);
    // ui->pbDisconnect->setEnabled(/*socket_ssl.state() == QAbstractSocket::ConnectedState);
}

void Widget::doConnected() {
    ui->pteMessage->insertPlainText("Connected \n");
}

void Widget::doDisconnected() {
    ui->pteMessage->insertPlainText("Disconnected \n");
}

void Widget::doReadyRead(){
    if (ui->checkBox->isChecked()) {
        ui->pteMessage->insertPlainText(socket_ssl.readAll());
    }
    else {
        ui->pteMessage->insertPlainText(socket_tcp.readAll());
    }

}

void Widget::on_checkBox_stateChanged(int arg1)
{
    if (ui->checkBox->isChecked()) {
        ui->pteMessage->insertPlainText("SSL mode.\n");
        ui->lePort->setText("443");
    }
    else {
        ui->pteMessage->insertPlainText("TCP mode.\n");
        ui->lePort->setText("80");
    }
}

void Widget::on_pbConnect_clicked()
{
    if (ui->checkBox->isChecked()) {
        socket_ssl.connectToHostEncrypted(ui->leHost->text(), ui->lePort->text().toUShort()); //SSL
    }
    else {
        socket_tcp.connectToHost(ui->leHost->text(), ui->lePort->text().toUShort()); //TCP,UDP
    }
    ui->pbConnect->setEnabled(false);
    ui->pbDisconnect->setEnabled(true);
}

void Widget::on_pbDisconnect_clicked()
{
    if (ui->checkBox->isChecked()) {
        socket_ssl.close();
    }
    else {
        socket_tcp.close();
    }
    ui->pbConnect->setEnabled(true);
    ui->pbDisconnect->setEnabled(false);
}

void Widget::on_pbSend_clicked()
{
    if (ui->checkBox->isChecked()) {
        socket_ssl.write(ui->pteSend->toPlainText().toUtf8());
    }
    else {
        socket_tcp.write(ui->pteSend->toPlainText().toUtf8());
    }
}

void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}


