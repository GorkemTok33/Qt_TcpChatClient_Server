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

#include "chatservermain.h"

int chatServerMain::socketId = 0;

chatServerMain::chatServerMain(QPushButton *btn, QListWidget *consol, QListWidget *chat, QListWidget *client)
{
    this->tcpServer = new QTcpServer(this);
    this->btnConnect = btn;
    this->consolHistory = consol;
    this->chatRoom = chat;
    this->clientInChat = client;
    this->socketinclient = new QVector<serverSocket*>;
    currentClient = 0;
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

void chatServerMain::chatServerStart(QString hostAdress,int portNumber, int maxClient, QString serverName, QString serverPass)
{
    this->serverPort = portNumber;
    this->serverName = serverName;
    this->serverPass = serverPass;
    this->maxClient = maxClient;
    this->hostAdress = hostAdress;
    CreateServer();
}

void chatServerMain::CreateServer()
{
    if(!tcpServer->isListening())
    {
        if(tcpServer->listen(QHostAddress(hostAdress), serverPort))
        {
            consolHistory->addItem("Chat Server started !");
            UpdateBtnConnect("Stop Server", true);
        }

        else
            consolHistory->addItem("Couldn't start chat server, please change socket number ");

        consolHistory->scrollToBottom();

        return;
    }

    consolHistory->addItem("Couldn't create chat server, Server already open !");
    consolHistory->scrollToBottom();
    return;
}

void chatServerMain::chatServerStop()
{
    if(tcpServer->isListening())
    {
        tcpServer->close();
        UpdateBtnConnect("Start Server", true);
        consolHistory->addItem("Chat Server is closed...");
        consolHistory->scrollToBottom();
        return;
    }

    UpdateBtnConnect("Start Server", true);
    consolHistory->addItem("Couldn't find active chat server !");
    consolHistory->scrollToBottom();
    return;
}

void chatServerMain::newConnection()
{
    currentClient=socketinclient->size()+1;
    tcpSocket = new QTcpSocket();
    tcpSocket = tcpServer->nextPendingConnection();

    if(maxClient>=currentClient)
    {
        serversocket = new serverSocket(tcpSocket, socketId, serverPass, serverName, socketinclient, chatRoom);
        socketinclient->append(serversocket);
        consolHistory->addItem("Connection request accepted !");
        clientInChat->addItem(QString::number(socketId)+" in chat server !");
        clientInChat->scrollToBottom();
        socketId ++;
        return;
    }

    tcpSocket->write("Server is full !");
    tcpSocket->waitForBytesWritten(2000);
    consolHistory->addItem("Connection request declined, server is full !");
    consolHistory->scrollToBottom();
    delete  tcpSocket;
    return;
}

void chatServerMain::UpdateBtnConnect(QString text, bool enabled)
{
    this->btnConnect->setText(text);
    this->btnConnect->setEnabled(enabled);
}


