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

#include "serversocket.h"
#include <QHostAddress>

serverSocket::serverSocket(QTcpSocket *socketMain, int socketId, QString serverPass, QString serverName,
                           QVector<serverSocket*> *inclientsockets, QListWidget *chatHistory, QListWidget *consol,
                           QListWidget *clientInChat, bool *flagDeleteDisconnectedServers)
{
    this->tcpsocket = socketMain;
    this->hostAddress = new QHostAddress();
    *hostAddress = socketMain->peerAddress();
    this->socketId = socketId;
    this->serverPass = serverPass;
    this->serverName = serverName;
    this->inclientSockets = inclientsockets;
    connect(tcpsocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpsocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpsocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    stringWord = "Welcome to the "+serverName+" chat room !";
    data = stringWord.toUtf8();
    this->tcpsocket->write(data);
    this->tcpsocket->waitForBytesWritten(3000);
    this->tcpsocket->write("\nPlease enter the server password ! Counting: 3");
    this->tcpsocket->waitForBytesWritten(3000);
    iscanChat = false;
    onebyonechat = false;
    passCounter = 3;
    onbyoneactivated = false;
    this->chatHistory = chatHistory;
    this->consol = consol;
    this->clientInChat = clientInChat;
    this->flagDeleteDisconnectedServers = flagDeleteDisconnectedServers;
    this->isMuted = new bool();
    *isMuted = false;
}

QString serverSocket::NickName()
{
    return this->nickName;
}

void serverSocket::disconnectByServer()
{
    tcpsocket->write("Server disconnected this client");
    tcpsocket->waitForBytesWritten(3000);
    tcpsocket->close();
}

void serverSocket::onebyoneChat(QString command)
{
    if(command == "/startp" || onbyoneactivated)
    {
        if(!onbyoneactivated)
        {
            onbyoneactivated = true;
            tcpsocket->write("Plase enter username for private message !");
            tcpsocket->waitForBytesWritten(3000);
        }

        else if(onbyoneactivated)
        {
            bool isFound = false;

            tcpsocket->write("searching... !");
            tcpsocket->waitForBytesWritten(3000);

            foreach(serverSocket *clients, *inclientSockets)
            {
                if(clients->NickName() == command && clients->getsocketId() != this->getsocketId())
                {
                    onebyoneSocket = clients;
                    onebyonechat = true;
                    onbyoneactivated = false;
                    isFound = true;
                    break;
                }
            }

            if(isFound)
            {
                this->tcpsocket->write("In private message with "+onebyoneSocket->NickName().toUtf8()+" For exit = /stopp");
                this->tcpsocket->waitForBytesWritten(3000);
            }

            else
            {
                this->tcpsocket->write(command.toUtf8()+" username not found !");
                this->tcpsocket->waitForBytesWritten(3000);
            }

        }

    }

    else if(command =="/stopp")
    {
        if(onebyonechat)
        {
            onebyonechat = false;
            tcpsocket->write("Private message with "+onebyoneSocket->NickName().toUtf8()+" closed !");
            tcpsocket->waitForBytesWritten(3000);
            onebyoneSocket->getsocket()->write(this->NickName().toUtf8()+" closed private message !");
            onebyoneSocket->getsocket()->waitForBytesWritten(3000);
        }

        else
        {
            tcpsocket->write("Private message: Wrong command !");
            tcpsocket->waitForBytesWritten(3000);
        }

        onbyoneactivated = false;
    }

    return;
}

void serverSocket::updateClientStatus(const QString text, QColor color)
{
    QList<QListWidgetItem*> founded = clientInChat->findItems("h:"+hostAddress->toString()+" i:"+QString::number(socketId), Qt::MatchContains);
    if(founded.count() > 0){
        founded[0]->setText("h:"+hostAddress->toString()+" i:"+QString::number(socketId)+": "+text);
        founded[0]->setTextColor(color);
    }

}

QColor serverSocket::randomColor()
{
    QColor color;
    int randNum = randomInteger(0,6);

    if(randNum == 0)
        color = Qt::red;
    else if(randNum == 1)
        color = Qt::green;
    else if(randNum == 2)
        color = Qt::white;
    else if(randNum == 3)
        color = Qt::cyan;
    else if(randNum == 4)
        color = Qt::yellow;
    else if(randNum == 5)
        color = Qt::blue;
    else if(randNum == 6)
        color = Qt::magenta;
    else
        color = Qt::white;

    return color;
}

int serverSocket::getsocketId()
{
    return this->socketId;
}

QTcpSocket* serverSocket::getsocket()
{
    return this->tcpsocket;
}

bool serverSocket::iscanChatStatus()
{
    return this->iscanChat;
}

void serverSocket::connected()
{

}

void serverSocket::disconnected()
{
    if(iscanChat)
    {
        stringWord = QString("%1: %2").arg(this->nickName).arg(" Disconnected");
        foreach(serverSocket *clients, *inclientSockets)
        {
            if(clients->getsocketId() != this->socketId && clients->iscanChatStatus())
            {
                clients->getsocket()->write(stringWord.toUtf8());
            }
        }
    }

    updateClientStatus("Socket address:"+hostAddress->toString()+" disconnected from server !", Qt::red);

    if(*flagDeleteDisconnectedServers){

        QList<QListWidgetItem*> founded = clientInChat->findItems("h:"+hostAddress->toString()+" i:"+QString::number(socketId), Qt::MatchContains);
        if(founded.count() > 0){
            clientInChat->takeItem(clientInChat->row(founded[0]));
            qDebug()<<"removed ";
        }
    }

    inclientSockets->removeOne(this);
    emit updateCurrentClientCount();
    delete this;

}

void serverSocket::bytesWritten(qint64 bytes)
{

}

void serverSocket::readyRead()
{
    data = this->tcpsocket->readAll();
    QString msg = QString::fromUtf8(data);
    msg.replace("\r\n","");
    qDebug()<<msg;

    if(iscanChat && this->NickName() != "")
    {
        if(!*isMuted)
        {
            if(!onebyonechat && !(msg == "/startp" || msg =="/stopp") && !onbyoneactivated)
            {
                stringWord = QString("%1: %2").arg(this->nickName).arg(msg);
                foreach(serverSocket *clients, *inclientSockets)
                {
                    if(clients->getsocketId() != this->socketId && clients->iscanChatStatus())
                    {
                        clients->getsocket()->write(stringWord.toUtf8());
                    }
                }

                qDebug()<<stringWord;
                emit addChatHistory(stringWord, randomColor());

                tcpsocket->write("You: "+data);
                tcpsocket->waitForBytesWritten(3000);
            }

            else
            {
                if(msg == "/startp" || msg =="/stopp" || onbyoneactivated)
                {
                    onebyoneChat(msg);
                    return;
                }

                if(onebyonechat)
                {
                    stringWord = QString("%1(private): %2").arg(this->nickName).arg(msg);
                    onebyoneSocket->getsocket()->write(stringWord.toUtf8());
                    onebyoneSocket->getsocket()->waitForBytesWritten(3000);
                    tcpsocket->write("You(private): "+data);
                    tcpsocket->waitForBytesWritten(3000);
                }
            }
        }
        else{
            tcpsocket->write("Message could'nt send\nBecause the server is muted this client");
            tcpsocket->waitForBytesWritten(3000);
        }
     }

     else if(iscanChat && NickName() == "")
     {
         this->nickName = msg;
         updateClientStatus("Chat name: "+nickName, Qt::white);
         tcpsocket->write("Welcome "+data+" !");
         tcpsocket->waitForBytesWritten(3000);
         stringWord = QString("%1: %2").arg(this->nickName).arg(" joined chat !");
         foreach(serverSocket *clients, *inclientSockets)
         {
             if(clients->getsocketId() != this->socketId && clients->iscanChatStatus())
             {
                 clients->getsocket()->write(stringWord.toUtf8());
             }
         }
         return;
     }

    else
    {
         if(msg == serverPass && passCounter > 0)
         {
             updateClientStatus("Password accepted - waiting name", Qt::green);
             tcpsocket->write("Password accepted !");
             tcpsocket->waitForBytesWritten(3000);
             tcpsocket->write("\nPlase enter nick name(this is chat identity) !");
             tcpsocket->waitForBytesWritten(3000);
             iscanChat = true;
             return;
         }

         else if(passCounter > 1)
         {
            passCounter--;
            updateClientStatus("Password decliend - counting: "+QString::number(passCounter), Qt::yellow);
            tcpsocket->write("Password declined ! Counting: "+QString::number(passCounter).toUtf8());
            tcpsocket->waitForBytesWritten(3000);
            return;
         }

         else
         {
             emit addConsoleHistory(("Socket address:"+hostAddress->toString()+" entered three times wrong password !"), Qt::red);
             emit addConsoleHistory(("Socket address:"+hostAddress->toString()+" disconnected from server !"), Qt::red);
             tcpsocket->write("Password entry rights are expired");
             tcpsocket->waitForBytesWritten(3000);
             updateClientStatus("Password entry rights are expired\ndisconnected from server", Qt::red);
             inclientSockets->removeOne(this);
             if(*flagDeleteDisconnectedServers){

                 QList<QListWidgetItem*> founded = clientInChat->findItems("h:"+hostAddress->toString()+" i:"+QString::number(socketId), Qt::MatchContains);
                 if(founded.count() > 0){
                     clientInChat->takeItem(clientInChat->row(founded[0]));
                 }
             }
             tcpsocket->close();
         }
    }

    return;
}

int serverSocket::randomInteger(const int min, const int max)
{
    return qrand() % ((max + 1) - min) + min;
}



