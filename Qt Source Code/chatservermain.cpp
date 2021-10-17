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
    this->flagAcceptSameHost = new bool();
    this->flagAutoIncrementCapacity = new bool();
    this->flagDeleteDisconnectedServers = new bool();
    *flagAcceptSameHost = true;
    *flagAutoIncrementCapacity = false;
    *flagDeleteDisconnectedServers = false;
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
    this->controlAdd = new bool();
    *controlAdd = false;
    CreateServer();
}

void chatServerMain::CreateServer()
{
    if(!tcpServer->isListening())
    {
        if(tcpServer->listen(QHostAddress(hostAdress), serverPort))
        {
            emit addConsoleHistory("Chat Server started !", Qt::green);
            UpdateBtnConnect("Stop Server", true);
        }

        else
            emit addConsoleHistory("Couldn't start chat server, please change socket number ", Qt::red);

        return;
    }

    emit addConsoleHistory("Couldn't create chat server, Server already open !", Qt::yellow);
    return;
}

void chatServerMain::chatServerStop()
{
    if(tcpServer->isListening())
    {
        tcpServer->close();
        for(int i = 0; i < socketinclient->length(); i++){
            socketinclient->at(i)->getsocket()->close();
        }
        free(socketinclient);
        socketinclient = new QVector<serverSocket*>;
        UpdateBtnConnect("Start Server", true);
        emit addConsoleHistory("Chat Server is closed...", Qt::red);
        return;
    }

    UpdateBtnConnect("Start Server", true);
    emit addConsoleHistory("Couldn't find active chat server !", Qt::yellow);
    return;
}

void chatServerMain::sendMessageAsServer(const QString message)
{
    for(int i = 0; i < socketinclient->count(); i++){
        if(socketinclient->at(i)->iscanChatStatus() == true){
            socketinclient->at(i)->getsocket()->write(message.toUtf8());
            socketinclient->at(i)->getsocket()->waitForBytesWritten(1000);
        }
    }
}

void chatServerMain::disconnect(QString name)
{
    bool founded = false;
    int index = -1;
    for(int i = 0; i < socketinclient->length(); i++){
        if(socketinclient->at(i)->NickName() == name){
            founded = true;
            index = i;
            break;
        }
    }

    if(founded){
        socketinclient->at(index)->disconnectByServer();
        emit addConsoleHistory(name+" named client disconnected", Qt::cyan);
        return;
    }

    emit addConsoleHistory(name+" named client not found", Qt::red);
    return;
}

void chatServerMain::newConnection()
{
    currentClient = currentClient=socketinclient->length()+1;
    tcpSocket = new QTcpSocket();
    tcpSocket = tcpServer->nextPendingConnection();
    *controlAdd = false;

    if(!*flagAcceptSameHost){
        for(int i = 0; i < socketinclient->count(); i++){
            if(socketinclient->at(i)->hostAddress->toString().contains(tcpSocket->peerAddress().toString())){
                *controlAdd = true;
                break;
            }
        }
    }

    if(!*flagAcceptSameHost && *controlAdd)
    {
        tcpSocket->write("Host already connected, access denied !");
        tcpSocket->waitForBytesWritten(2000);
        emit addConsoleHistory(tcpSocket->peerAddress().toString()+" ip request declined ! Cause secure host enabled", Qt::red);
        delete  tcpSocket;
        return;
    }

    else
    {
        if(maxClient < currentClient && *flagAutoIncrementCapacity)
        {
            maxClient +=1 ;
            emit addConsoleHistory("Auto increment status is true - server capacity increased to: "+QString::number(currentClient)+" !", Qt::darkCyan);
        }

        emit clientCounterUpdate(currentClient);

        if(maxClient>=currentClient)
        {
            serversocket = new serverSocket(tcpSocket, socketId, serverPass, serverName, socketinclient, chatRoom, consolHistory, clientInChat, flagDeleteDisconnectedServers);
            socketinclient->append(serversocket);
            connect(serversocket, SIGNAL(addClientLive(const QString, QColor)), this, SLOT(addClientLive_(const QString, QColor)));
            connect(serversocket, SIGNAL(addConsoleHistory(const QString, QColor)), this, SLOT(addConsoleHistory_(const QString, QColor)));
            connect(serversocket, SIGNAL(addChatHistory(const QString, QColor)), this, SLOT(addChatHistory_(const QString, QColor)));
            connect(serversocket, SIGNAL(updateCurrentClientCount()), this, SLOT(decrementCurrentClientCount()));
            emit addConsoleHistory("h:"+tcpSocket->peerAddress().toString()+" Connection request accepted !", Qt::yellow);
            emit addClientLive("h:"+tcpSocket->peerAddress().toString()+" i:"+QString::number(socketId)+" waiting pass password check", Qt::green);
            socketId ++;
            return;
        }

        tcpSocket->write("Server is full !");
        tcpSocket->waitForBytesWritten(2000);
        emit addConsoleHistory("Connection request declined, server is full !", Qt::red);
        currentClient --;
        decrementCurrentClientCount();
        delete  tcpSocket;
        return;
    }
}

void chatServerMain::decrementCurrentClientCount()
{
    currentClient = currentClient=socketinclient->length();
    emit clientCounterUpdate(currentClient);
}

void chatServerMain::setAcceptSameHost(const bool value)
{
    *flagAcceptSameHost = value;
}

void chatServerMain::setAutoIncrementCapacity(const bool value)
{
    *flagAutoIncrementCapacity = value;
}

void chatServerMain::setDeleteDisconnectedClients(const bool value)
{
    *flagDeleteDisconnectedServers = value;
    QList<QListWidgetItem*> founded = clientInChat->findItems("disconnected", Qt::MatchContains);
    for(int i = 0; i < founded.count(); i++){
        clientInChat->takeItem(clientInChat->row(founded[i]));
        qDebug()<<"removed ";
    }
}

void chatServerMain::addClientLive_(const QString value, QColor color)
{
    emit addClientLive(value, color);
}

void chatServerMain::addConsoleHistory_(const QString value, QColor color)
{
    emit addConsoleHistory(value, color);
}

void chatServerMain::addChatHistory_(const QString value, QColor color)
{
    emit addChatHistory(value, color);
}

void chatServerMain::muteClient(const QString chatname, const bool mute)
{
    bool founded = false;
    int index = -1;
    for(int i = 0; i < socketinclient->length(); i++){
        if(socketinclient->at(i)->NickName() == chatname){
            founded = true;
            index = i;
            break;
        }
    }

    if(founded){
        *socketinclient->at(index)->isMuted = mute;
        if(mute){
            emit addConsoleHistory(chatname+" named client muted", Qt::cyan);
        }
        else
           {
            emit addConsoleHistory(chatname+" named client unmuted", Qt::cyan);
        }
        return;
    }

    emit addConsoleHistory(chatname+" named client not found", Qt::red);
    return;
}

void chatServerMain::UpdateBtnConnect(QString text, bool enabled)
{
    this->btnConnect->setText(text);
    this->btnConnect->setEnabled(enabled);
}


