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

#ifndef CONNECTSERVERDIALOG_H
#define CONNECTSERVERDIALOG_H

#include <QDialog>
#include "chatservermain.h"
#include "commands.h"

namespace Ui {
class ConnectServerDialog;
}

class ConnectServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectServerDialog(QWidget *parent = nullptr);
    ~ConnectServerDialog();

signals:
    void onSecureHostChanged(const bool value);
    void onAutoIncrementCapacityChanged(const bool value);
    void onDeleteDisconnectedClientsChanged(const bool value);

private slots:
    void on_btnChatStartServer_clicked();
    void on_pushButtonServerMessage_clicked();
    void on_chatmaxclient_textChanged(const QString &arg1);
    void on_checkBoxSecureHost_clicked(bool checked);
    void on_checkBoxAutoIncrement_clicked(bool checked);
    void clientCounterUpdate(int count);
    void addClientLive(const QString value, QColor color = Qt::white);
    void addConsoleHistory(const QString value, QColor color = Qt::white);
    void addChatHistory(const QString value, QColor color = Qt::white);
    void on_checkBoxRemoveDisconnectedClients_clicked(bool checked);
    void on_pushButtonSendCmd_clicked();

private:
    Ui::ConnectServerDialog *ui;
    chatServerMain *chatserverMain;
    void initHelpList();
    void clearConsole();
    void mute(bool state);
    void errorMessage();
    void disCnt();
    bool *enterNameEnabled;
    bool *ismute;
    bool *isDisconnect;
};

#endif // CONNECTSERVERDIALOG_H
