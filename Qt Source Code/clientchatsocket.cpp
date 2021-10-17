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
        addConsoleHistory("Couldn't connect host !", Qt::red);
        return false;
    }

    return true;
}

void ClientChatSocket::addChatHistory(QString value, QColor color)
{
    QListWidgetItem *item = new QListWidgetItem(value);
    item->setTextColor(color);
    chatHistory->addItem(item);
    chatHistory->scrollToBottom();
}

void ClientChatSocket::addConsoleHistory(QString value, QColor color)
{
    QListWidgetItem *item = new QListWidgetItem(value);
    item->setTextColor(color);
    consolHistory->addItem(item);
    consolHistory->scrollToBottom();
}

int ClientChatSocket::randomInteger(const int min, const int max)
{
    return qrand() % ((max + 1) - min) + min;
}

void ClientChatSocket::connected()
{
    addConsoleHistory("Connected the host !", Qt::green);
    messageSendButton->setEnabled(true);
    btnConnect->setText("Disconnect");
}

void ClientChatSocket::disconnected()
{
    addConsoleHistory("Disconnected from the host !", Qt::yellow);
    messageSendButton->setEnabled(false);
    btnConnect->setText("Connect");
}

void ClientChatSocket::bytesWritten(qint64 bytes)
{
    qDebug()<<QString::number(bytes)+" Bytes writted to chat host !";
}

void ClientChatSocket::readyRead()
{
    QByteArray data = qtcpsocket->readAll();
    std::string stringData = data.toStdString();
    addChatHistory(QString::fromStdString(stringData), randomColor());
    chatHistory->scrollToBottom();
}

void ClientChatSocket::SendMessage(QString data)
{
    QByteArray byteData = data.toUtf8();
    qtcpsocket->write(byteData);
    if(!qtcpsocket->waitForBytesWritten(3000))
        addConsoleHistory("Time out ! Couldn't send data host !", Qt::red);
}

QColor ClientChatSocket::randomColor()
{
    QColor color;
    int randNum = randomInteger(0,6);

    if(randNum == 0)
        color = Qt::red;
    else if(randNum == 1)
        color = Qt::green;
    else if(randNum == 2)
        color = Qt::white;
    else if(randNum == 3)
        color = Qt::cyan;
    else if(randNum == 4)
        color = Qt::yellow;
    else if(randNum == 5)
        color = Qt::blue;
    else if(randNum == 6)
        color = Qt::magenta;
    else
        color = Qt::white;

    return color;
}



