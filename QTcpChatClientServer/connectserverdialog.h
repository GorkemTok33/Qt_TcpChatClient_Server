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

namespace Ui {
class ConnectServerDialog;
}

class ConnectServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectServerDialog(QWidget *parent = nullptr);
    ~ConnectServerDialog();

private slots:
    void on_btnChatStartServer_clicked();

private:
    Ui::ConnectServerDialog *ui;
    chatServerMain *chatserverMain;
};

#endif // CONNECTSERVERDIALOG_H
