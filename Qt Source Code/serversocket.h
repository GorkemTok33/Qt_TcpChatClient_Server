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
#include <QHostAddress>

class serverSocket:public QObject
{
    Q_OBJECT

public:
    serverSocket(QTcpSocket *socketMain, int sockedId, QString serverPass, QString serverName,
                 QVector<serverSocket*> *inclientsockets, QListWidget *chatHistory, QListWidget *consol,
                 QListWidget *clientInChat, bool *flagDeleteDisconnectedServers);
    QHostAddress *hostAddress;
    int getsocketId();
    QTcpSocket *getsocket();
    bool iscanChatStatus();
    QString NickName();
    void disconnectByServer();
    bool *isMuted;

signals:
    void addClientLive(const QString value, QColor color = Qt::white);
    void addConsoleHistory(const QString value, QColor color = Qt::white);
    void addChatHistory(const QString value, QColor color = Qt::white);
    void updateCurrentClientCount();

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
    QListWidget *consol;
    QListWidget *clientInChat;
    int socketId;
    int passCounter;
    bool iscanChat;
    bool onebyonechat;
    bool onbyoneactivated;
    bool *flagDeleteDisconnectedServers;
    void updateClientStatus(const QString, QColor color = Qt::white);
    QColor randomColor();
    int randomInteger(const int min, const int max);
};

#endif // SERVERSOCKET_H
