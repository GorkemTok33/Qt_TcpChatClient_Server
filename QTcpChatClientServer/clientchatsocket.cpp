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

#include "clientchatsocket.h"

ClientChatSocket::ClientChatSocket(QPushButton *btnconnect,QListWidget *consolHistory, QPushButton *messagesendButton, QListWidget *chatHistory)
{
    this->qtcpsocket = new QTcpSocket();
    this->consolHistory = consolHistory;
    this->chatHistory = chatHistory;
    this->messageSendButton = messagesendButton;
    this->btnConnect = btnconnect;
    connect(qtcpsocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(qtcpsocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(qtcpsocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(qtcpsocket, SIGNAL(readyRead()), this, SLOT(readyRead()));

}

void ClientChatSocket::TestConnection()
{

}

void ClientChatSocket::CloseConnection()
{
    if(qtcpsocket->isOpen())
        qtcpsocket->close();
    qDebug()<<"Socket closed !";
    btnConnect->setText("Connect");
}

void ClientChatSocket::ConnectToHost(QString hostName, int port)
{
    this->hostName = hostName;
    this->port = port;
    ConnectHost();
}

bool ClientChatSocket::ConnectHost()
{
    qtcpsocket->connectToHost(this->hostName, this->port);

    if(!qtcpsocket->waitForConnected(5000))
    {
        consolHistory->addItem("Couldn't connect the chat host !");
        consolHistory->scrollToBottom();
        return false;
    }

    return true;
}

void ClientChatSocket::connected()
{
    consolHistory->addItem("Connected the chat host !");
    messageSendButton->setEnabled(true);
    btnConnect->setText("Disconnect");
}

void ClientChatSocket::disconnected()
{
    consolHistory->addItem("Disconnected from the chat host !");
    consolHistory->scrollToBottom();
    messageSendButton->setEnabled(false);
    btnConnect->setText("Connect");
}

void ClientChatSocket::bytesWritten(qint64 bytes)
{
    consolHistory->addItem(QString::number(bytes)+" Bytes writted to chat host !");
    consolHistory->scrollToBottom();
}

void ClientChatSocket::readyRead()
{
    QByteArray data = qtcpsocket->readAll();
    std::string stringData = data.toStdString();
    chatHistory->addItem(QString::fromStdString(stringData));
    chatHistory->scrollToBottom();
}

void ClientChatSocket::SendMessage(QString data)
{
    QByteArray byteData = data.toUtf8();
    qtcpsocket->write(byteData);
    if(!qtcpsocket->waitForBytesWritten(3000))
        consolHistory->addItem("Time out ! Couldn't send data to chat server !");
    consolHistory->scrollToBottom();
}



