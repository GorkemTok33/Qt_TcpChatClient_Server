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

#include "client.h"
#include "ui_client.h"
#include <QDebug>

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    this->setWindowTitle("TCP/IP Chat Server Client 1.0.0");
    ui->lineEditServerPort->setValidator(new QIntValidator());
    ui->lineEditServerPort->setMaxLength(4);
    ui->pushButtonConnectToServer->setWhatsThis("Connect to the chat server given host name and port number !");
    connect(this, SIGNAL(ConnectServer()), this, SLOT(SlotConnectToTheServer()));
    chatSocket = new ClientChatSocket(ui->pushButtonConnectToServer,ui->listWidgetConsol, ui->pushButtonSendMessage, ui->listWidgetMessages);
}

Client::~Client()
{
    delete ui;
}

void Client::closeEvent(QCloseEvent *event)
{
    emit dialogClosed();
    event->accept();
}

void Client::on_pushButtonConnectToServer_clicked()
{
    emit ConnectServer();
}

void Client::SlotConnectToTheServer()
{
    if(ui->pushButtonConnectToServer->text() == "Connect")
    chatSocket->ConnectToHost(ui->lineEditServerHostname->text(), ui->lineEditServerPort->text().toInt());
    else if(ui->pushButtonConnectToServer->text()=="Disconnect")
        chatSocket->CloseConnection();
}

void Client::on_pushButtonSendMessage_clicked()
{
    chatSocket->SendMessage(ui->lineEditMessage->text());
    ui->lineEditMessage->setText("");
}













