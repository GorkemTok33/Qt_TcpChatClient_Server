/*
****************************************************************************************
****************************************************************************************
Program: QTcpChatClientServer
Publish Date: 09.10.2021
Author: Görkem Tok
Language: C++
Platform: Qt Creator
Contact: ceng.gorkem.tok@gmail.com
License: MIT License, Copyright 2021 Görkem Tok
****************************************************************************************
****************************************************************************************
*/

#include "connectserverdialog.h"
#include "ui_connectserverdialog.h"

ConnectServerDialog::ConnectServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectServerDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("TCP/IP Chat Server 1.0.0");
    this->chatserverMain = new chatServerMain(ui->btnChatStartServer, ui->chatConsole, ui->chatRoom, ui->chatClients);

    ui->chatConsole->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->chatRoom->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->chatClients->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);

}

ConnectServerDialog::~ConnectServerDialog()
{
    delete ui;
}

void ConnectServerDialog::on_btnChatStartServer_clicked()
{
    if(ui->btnChatStartServer->text() == "Start Server")
        chatserverMain->chatServerStart(ui->lineEditHostAddrChat->text(),ui->chatportnumber->text().toInt(), ui->chatmaxclient->text().toInt(),
                                    ui->chatname->text(), ui->chatpass->text());
    else if(ui->btnChatStartServer->text() == "Stop Server")
        chatserverMain->chatServerStop();
}


