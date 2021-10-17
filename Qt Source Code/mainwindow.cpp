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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap image(":/data/web_hosting.jpg");
    int w = ui->label->width();
    int h = ui->label->height();
    ui->label->setPixmap(image.scaled(w,h,Qt::IgnoreAspectRatio));
    this->setWindowTitle("Tcp/ip Chat Server v1.0.0");

    connect(this, SIGNAL(ClientButtonClicked()), this, SLOT(OpenClient()));
    connect(this, SIGNAL(ServerButtonClicked()), this, SLOT(OpenServer()));
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::OpenClient()
{
    client = new Client();
    client->setModal(false);
    client->setAttribute(Qt::WA_DeleteOnClose, true);
    client->show();
    client->exec();
}

void MainWindow::OpenServer()
{
    Server = new ConnectServerDialog();
    Server->setModal(false);
    Server->setAttribute(Qt::WA_DeleteOnClose, true);
    Server->show();
    Server->exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    emit ClientButtonClicked();
}

void MainWindow::on_pushButton_clicked()
{
    emit ServerButtonClicked();
}
