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

#include "connectserverdialog.h"
#include "ui_connectserverdialog.h"
#include <QListWidgetItem>
#include "commands.h"

ConnectServerDialog::ConnectServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectServerDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("TCP/IP Chat Server 1.0.0");
    this->chatserverMain = new chatServerMain(ui->btnChatStartServer, ui->chatConsole, ui->chatRoom, ui->chatClients);
    ui->chatConsole->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->chatRoom->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    ui->chatClients->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
    connect(this,SIGNAL(onSecureHostChanged(const bool)),chatserverMain, SLOT(setAcceptSameHost(const bool)));
    connect(this,SIGNAL(onAutoIncrementCapacityChanged(const bool)),chatserverMain, SLOT(setAutoIncrementCapacity(const bool)));
    connect(chatserverMain, SIGNAL(clientCounterUpdate(int)), this, SLOT(clientCounterUpdate(int)));
    connect(chatserverMain, SIGNAL(addClientLive(const QString, QColor)), this, SLOT(addClientLive(const QString, QColor)));
    connect(chatserverMain, SIGNAL(addConsoleHistory(const QString, QColor)), this, SLOT(addConsoleHistory(const QString, QColor)));
    connect(chatserverMain, SIGNAL(addChatHistory(const QString, QColor)), this, SLOT(addChatHistory(const QString, QColor)));
    connect(this, SIGNAL(onDeleteDisconnectedClientsChanged(const bool)), chatserverMain, SLOT(setDeleteDisconnectedClients(const bool)));
    QListWidgetItem *item = new QListWidgetItem("Chat Client Console V 1.0.0 - Coded By Görkem Tok");
    item->setTextColor(Qt::green);
    ui->chatConsole->addItem(item);
    item = new QListWidgetItem("For help: write \\help");
    item->setTextColor(Qt::white);
    ui->chatConsole->addItem(item);
    enterNameEnabled = new bool();
    *enterNameEnabled = false;
    ismute = new bool();
    *ismute = false;
    isDisconnect = new bool();
    *isDisconnect = false;
}

ConnectServerDialog::~ConnectServerDialog()
{
    delete ui;
}

void ConnectServerDialog::on_btnChatStartServer_clicked()
{
    if(ui->btnChatStartServer->text() == "Start Server"){
        chatserverMain->chatServerStart(ui->lineEditHostAddrChat->text(),ui->chatportnumber->text().toInt(), ui->chatmaxclient->text().toInt(),
                                        ui->chatname->text(), ui->chatpass->text());
        ui->pushButtonServerMessage->setEnabled(true);
    }
    else if(ui->btnChatStartServer->text() == "Stop Server")
    {
        chatserverMain->chatServerStop();
        ui->pushButtonServerMessage->setEnabled(false);
    }
}

void ConnectServerDialog::on_pushButtonServerMessage_clicked()
{

    addChatHistory("Server: "+ui->lineEditServerMessage->text(), Qt::white);
    ui->chatRoom->addItem("Server: "+ui->lineEditServerMessage->text());
    ui->lineEditServerMessage->clear();
}

void ConnectServerDialog::on_chatmaxclient_textChanged(const QString &arg1)
{

}

void ConnectServerDialog::on_checkBoxSecureHost_clicked(bool checked)
{
    emit onSecureHostChanged(!checked);
}

void ConnectServerDialog::on_checkBoxAutoIncrement_clicked(bool checked)
{
    emit onAutoIncrementCapacityChanged(checked);
}

void ConnectServerDialog::clientCounterUpdate(int count)
{
    ui->lcdNumber->display(count);
}

void ConnectServerDialog::addClientLive(const QString value, QColor color)
{
    QListWidgetItem *item = new QListWidgetItem(value);
    item->setForeground(color);
    ui->chatClients->addItem(item);
    ui->chatClients->scrollToBottom();
}

void ConnectServerDialog::addConsoleHistory(const QString value, QColor color)
{
    QListWidgetItem *item = new QListWidgetItem(value);
    item->setForeground(color);
    ui->chatConsole->addItem(item);
    ui->chatConsole->scrollToBottom();

}

void ConnectServerDialog::addChatHistory(const QString value, QColor color)
{
    QListWidgetItem *item = new QListWidgetItem(value);
    item->setForeground(color);
    ui->chatRoom->addItem(item);
    ui->chatRoom->scrollToBottom();
}

void ConnectServerDialog::on_checkBoxRemoveDisconnectedClients_clicked(bool checked)
{
    emit onDeleteDisconnectedClientsChanged(checked);
}

void ConnectServerDialog::on_pushButtonSendCmd_clicked()
{
    if(!*enterNameEnabled || !*isDisconnect){
        QListWidgetItem *item = new QListWidgetItem("input: "+ui->lineEditCmd->text());
        item->setTextColor(Qt::cyan);
        ui->chatConsole->addItem(item);
    }

    if(!*enterNameEnabled && !*isDisconnect)
    {
        switch (commands::cmd.indexOf(ui->lineEditCmd->text())) {
        case 0:
            clearConsole();
            break;
        case 1:
            initHelpList();
            break;
        case 2:
            mute(true);
            break;
        case 3:
            mute(false);
            break;
        case 4:
            disCnt();
            break;
        default:
            errorMessage();
            break;
        }
    }

    else{
        ui->chatConsole->setCurrentRow(ui->chatConsole->count()-1);
        ui->chatConsole->currentItem()->setText("chat name: "+ui->lineEditCmd->text());
        if(!*isDisconnect)
            chatserverMain->muteClient(ui->lineEditCmd->text(), *ismute);
        else{
            chatserverMain->disconnect(ui->lineEditCmd->text());
            *isDisconnect = false;
        }
        *enterNameEnabled = false;
    }

    ui->lineEditCmd->clear();
}

void ConnectServerDialog::initHelpList()
{
    QListWidgetItem *item = new QListWidgetItem("[CHAT SERVER CONSOLE COMMANDS]");
    item->setTextColor(Qt::green);
    ui->chatConsole->addItem(item);
    item = new QListWidgetItem("\\clr : clean the console");
    item->setTextColor(Qt::white);
    ui->chatConsole->addItem(item);
    item = new QListWidgetItem("\\help : shows console commands");
    item->setTextColor(Qt::white);
    ui->chatConsole->addItem(item);
    item = new QListWidgetItem("\\mute : disable message sending feature of matched chat name client");
    item->setTextColor(Qt::white);
    ui->chatConsole->addItem(item);
    item = new QListWidgetItem("\\unmute : enable message sending feature of matched chat name client");
    item->setTextColor(Qt::white);
    ui->chatConsole->addItem(item);
    item = new QListWidgetItem("\\discnt : disconnect matched chat name from server");
    item->setTextColor(Qt::white);
    ui->chatConsole->addItem(item);
}

void ConnectServerDialog::clearConsole()
{
    ui->chatConsole->clear();
    QListWidgetItem *item = new QListWidgetItem("Chat Client Console V 1.0.0 - Coded By Görkem Tok");
    item->setTextColor(Qt::green);
    ui->chatConsole->addItem(item);
}

void ConnectServerDialog::mute(bool state)
{
    QListWidgetItem *item = new QListWidgetItem("chat name: ");
    item->setTextColor(Qt::yellow);
    ui->chatConsole->addItem(item);
    *enterNameEnabled = true;
    *ismute = state;
}

void ConnectServerDialog::errorMessage()
{
    QListWidgetItem *item = new QListWidgetItem("Input did not match any command, for see all command write \\help");
    item->setTextColor(Qt::red);
    ui->chatConsole->addItem(item);
}

void ConnectServerDialog::disCnt()
{
    QListWidgetItem *item = new QListWidgetItem("chat name: ");
    item->setTextColor(Qt::yellow);
    ui->chatConsole->addItem(item);
    *isDisconnect = true;
}
