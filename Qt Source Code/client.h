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

#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QCloseEvent>
#include <QStringListModel>
#include "clientchatsocket.h"

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();

signals:
    void ConnectServer();
    void dialogClosed();

private slots:
    void SlotConnectToTheServer();
    void on_pushButtonConnectToServer_clicked();
    void on_pushButtonSendMessage_clicked();

private:
    //classes
    Ui::Client *ui;
    ClientChatSocket *chatSocket;
    //funcitons
    void closeEvent(QCloseEvent *event);
    //variables
    int channel = 0;
    //Models



};

#endif // CLIENT_H
