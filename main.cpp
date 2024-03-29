/*
    1. Решить вопрос с числом папок, чтобы числа не уходили в миллиарды
*/
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <QSqlTableModel>
#include <QDir>
#include "setmodelfilter.h"
#include <QSqlQuery>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int number_of_folders = 0; //Переменная отвечающая за число папок

    QString hostname = argv[1];

    QSqlDatabase sdb;

    sdb = QSqlDatabase::addDatabase("QSQLITE");

    sdb.setDatabaseName("C:/Files/data.db");           //"/var/lib/wirenboard/db"

    QSqlTableModel *devices;

    QSqlTableModel *data = new QSqlTableModel();

    QVector<std::string> device;

    QVector<int> int_id;

    QVector<QString> control;

    if (!sdb.open()) {
        std::cout << "Error DATABASE" << std::endl;
        return 1;
    }
    else {

        devices = new QSqlTableModel(nullptr, sdb);
        devices->setTable("channels");
        devices->select();
        QFile filedevices(argv[3]);
        if (!filedevices.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "File devices.txt NOT OPEN!!! " << argv[3] << filedevices.errorString();
                return 1;
        }
        else {
            QTextStream in(&filedevices);
            if (!in.atEnd()) {
                device.push_back(in.readLine().toStdString()); // Читаем первую строку
            }
        }



        devices->setFilter("device = 'wb-map12e_28' OR device = 'WB-MSWv.3'"); //Заменить на получение данных из вектора device
        for (int i = 0; i < devices->rowCount(); ++i) {
            QModelIndex index = devices->index(i, 0);
            QModelIndex index2 = devices->index(i, 2);
            int_id.push_back(devices->data(index).toInt());
            control.push_back(devices->data(index2).toString());
            qDebug() << devices->data(index).toInt() << " " << devices->data(index2).toString();
            //qDebug() << devices->data(index2).toString();
        }

        SetModelFilter *filter = new SetModelFilter("data", int_id, sdb);

        data = filter->getModel();

        int dataCounter = data->rowCount();

        qDebug() << dataCounter;

        QFile fileindex(argv[2]);
        if (!fileindex.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "File index.txt NOT OPEN!!!" << fileindex.errorString();
                return 1;
        }
        else {
            QTextStream in(&fileindex);
            QString firstLine = in.readLine();
            number_of_folders = firstLine.toInt();
            qDebug() << number_of_folders;
        }
        QString pathtoinput = argv[4];
        while (dataCounter != 0) {
            QString path = QString(pathtoinput+"/input_file%1.txt").arg(number_of_folders+1);
            QFile file(path);
            if (!file.open(QIODevice::ReadWrite)) {
                    qDebug() << path << " - ERROR open!" << file.errorString();
                }
            else {
                qDebug() << path << " - file is open!";
                QTextStream out(&file);
                for (int i = 0; i < 249; i++) {
                    if (dataCounter != 0) {
                        QModelIndex indexTime = data->index(i, 3);
                        QModelIndex indexValue = data->index(i, 2);
                        QModelIndex indexChannel = data->index(i, 1);
                        out << hostname << " " <<  data->data(indexChannel).toInt() << " " << data->data(indexTime).toInt() << " " << data->data(indexValue).toDouble() << endl;
                        dataCounter--;
                    }
                }
                number_of_folders++;
                file.close();
            }
        }
        fileindex.close();
        QFile fileindexnew(argv[2]);
        if (!fileindexnew.open(QIODevice::WriteOnly | QIODevice::Text)) {
                qDebug() << "File index.txt NOT OPEN!!!" << fileindex.errorString();
                return 1;
        }
        else {
            QTextStream out(&fileindexnew);
            out << number_of_folders;
        }
    }
    return 0;
}
