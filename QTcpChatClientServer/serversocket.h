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

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QObject>
#include <QVector>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QListWidget>

class serverSocket:public QObject
{
    Q_OBJECT

public:
    serverSocket(QTcpSocket *socketMain, int sockedId, QString serverPass, QString serverName,
                 QVector<serverSocket*> *inclientsockets, QListWidget *chatHistory);
    int getsocketId();
    QTcpSocket *getsocket();
    bool iscanChatStatus();
    QString NickName();

protected:
    void run();

private slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    void onebyoneChat(QString command);
    QVector<serverSocket*> *inclientSockets;
    QTcpSocket *tcpsocket;
    serverSocket *onebyoneSocket;
    QString serverName;
    QString serverPass;
    QString nickName;
    QString stringWord;
    QByteArray data;
    QListWidget *chatHistory;
    int socketId;
    int passCounter;
    bool iscanChat;
    bool onebyonechat;
    bool onbyoneactivated;
};

#endif // SERVERSOCKET_H
