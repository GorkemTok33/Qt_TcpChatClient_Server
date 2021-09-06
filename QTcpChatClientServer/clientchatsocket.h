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

#ifndef CLIENTCHATSOCKET_H
#define CLIENTCHATSOCKET_H

#include <QObject>
#include <QtCore>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QListWidget>
#include <QPushButton>

class ClientChatSocket:public QObject
{
    Q_OBJECT
public:
    explicit ClientChatSocket(QPushButton *btnConnect,QListWidget *consolHistory, QPushButton *messagesendButton, QListWidget *chatHistory);
    void TestConnection();
    void ConnectToHost(QString hostName, int port);
    void SendMessage(QString data);
    void CloseConnection();

private slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket *qtcpsocket;
    QListWidget *consolHistory;
    QListWidget *chatHistory;
    QPushButton *messageSendButton;
    QPushButton *btnConnect;
    QString hostName="NULL";
    int port = 0;
    bool ConnectHost();
};

#endif // CLIENTCHATSOCKET_H
