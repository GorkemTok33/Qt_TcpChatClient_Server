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

#ifndef CHATSERVERMAIN_H
#define CHATSERVERMAIN_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPushButton>
#include <QListWidget>
#include <QVector>
#include "serversocket.h"

class chatServerMain:public QObject
{
    Q_OBJECT
public:
    chatServerMain(QPushButton *btnConnect, QListWidget *consolHistory, QListWidget *chatRoom,
                   QListWidget *clientinChat);
    void chatServerStart(QString hostAdress, int portNumber, int maxClient, QString serverName, QString serverPass);
    void chatServerStop();

private slots:
    void newConnection();

private:
    serverSocket *serversocket;
    QVector<serverSocket*> *socketinclient;
    void UpdateBtnConnect(QString text, bool enabled);
    void CreateServer();
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QTcpSocket *tcpserversocket;
    QPushButton *btnConnect;
    QListWidget *consolHistory;
    QListWidget *chatRoom;
    QListWidget *clientInChat;
    QString serverName;
    QString serverPass;
    int serverPort;
    int serverType;
    int maxClient;
    int currentClient;
    static int socketId;
    QString hostAdress;
};

#endif // CHATSERVERMAIN_H
