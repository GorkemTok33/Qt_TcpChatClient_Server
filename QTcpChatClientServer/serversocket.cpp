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

serverSocket::serverSocket(QTcpSocket *socketMain, int socketId, QString serverPass, QString serverName, QVector<serverSocket*> *inclientsockets, QListWidget *chatHistory)
{
    this->tcpsocket = socketMain;
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
    this->tcpsocket->write(" Please enter the server password ! Counting: 3");
    this->tcpsocket->waitForBytesWritten(3000);
    iscanChat = false;
    onebyonechat = false;
    passCounter = 3;
    onbyoneactivated = false;
    this->chatHistory = chatHistory;
}

QString serverSocket::NickName()
{
    return this->nickName;
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

    inclientSockets->removeOne(this);

    delete this;

}

void serverSocket::bytesWritten(qint64 bytes)
{

}

void serverSocket::readyRead()
{
    data = this->tcpsocket->readAll();

    if(iscanChat && this->NickName() != "")
    {
        if(!onebyonechat && !(QString::fromUtf8(data) == "/startp" || QString::fromUtf8(data) =="/stopp") && !onbyoneactivated)
        {
            stringWord = QString("%1: %2").arg(this->nickName).arg(QString::fromUtf8(data));
            foreach(serverSocket *clients, *inclientSockets)
            {
                if(clients->getsocketId() != this->socketId && clients->iscanChatStatus())
                {
                    clients->getsocket()->write(stringWord.toUtf8());
                }
            }

            chatHistory->addItem(stringWord);
            chatHistory->scrollToBottom();

            tcpsocket->write("You: "+data);
            tcpsocket->waitForBytesWritten(3000);
        }

        else
        {
            if(QString::fromUtf8(data) == "/startp" || QString::fromUtf8(data) =="/stopp" || onbyoneactivated)
            {
                onebyoneChat(QString::fromUtf8(data));
                return;
            }

            if(onebyonechat)
            {
                stringWord = QString("%1(private): %2").arg(this->nickName).arg(QString::fromUtf8(data));
                onebyoneSocket->getsocket()->write(stringWord.toUtf8());
                onebyoneSocket->getsocket()->waitForBytesWritten(3000);
                tcpsocket->write("You(private): "+data);
                tcpsocket->waitForBytesWritten(3000);
            }
        }
     }

     else if(iscanChat && NickName() == "")
     {
         this->nickName = QString::fromUtf8(data);
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
         if(QString::fromUtf8(data)== serverPass && passCounter > 0)
         {
             tcpsocket->write("Password accepted !");
             tcpsocket->waitForBytesWritten(3000);
             tcpsocket->write("Plase enter nick name(this is chat identity) !");
             tcpsocket->waitForBytesWritten(3000);
             iscanChat = true;
             return;
         }

         else if(passCounter > 0)
         {
            passCounter--;
            tcpsocket->write("Password declined ! Counting: "+QString::number(passCounter).toUtf8());
            tcpsocket->waitForBytesWritten(3000);
            return;
         }

         else
         {
             tcpsocket->write("Password declined ! "+QString::number(passCounter).toUtf8());
             tcpsocket->waitForBytesWritten(3000);
             tcpsocket->write("Password entry rights are expired");
             tcpsocket->waitForBytesWritten(3000);
             inclientSockets->removeOne(this);
             tcpsocket->close();
             delete this;
         }
    }

    return;
}



