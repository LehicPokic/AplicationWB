#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <QSqlTableModel>
#include <QDir>
#include "setmodelfilter.h"



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int number_of_folders = 0; //Переменная отвечающая за число папок

    QSqlDatabase sdb;

    sdb = QSqlDatabase::addDatabase("QSQLITE");

    sdb.setDatabaseName("C:/Files/data.db");           //"/var/lib/wirenboard/db"

    QSqlTableModel *devices;

    QSqlTableModel *data = new QSqlTableModel();

    QVector<std::string> device = {"wb_map12e_28", "WB-MSWv.3"};

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

        data = filter->getModelFilter();

        int dataCounter = data->rowCount();

        qDebug() << dataCounter;

        QFile file("C:/Files/Project/WirenBoard_monitor/ApplicationCreate/index.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "File index.txt NOT OPEN!!!" << file.errorString();
                return 1;
        }
        else {
            QTextStream in(&file);
            QString firstLine = in.readLine();
            number_of_folders = firstLine.toInt();
            qDebug() << number_of_folders;
        }

        while (dataCounter != 0) {
            QString path = QString("./input_file_%1").arg(number_of_folders);
            QFile file(path);
            if (!file.open(QIODevice::ReadWrite)) {
                    qDebug() << path << " - ERROR open!"; //<< file.errorString();
                }
            else {
                qDebug() << path << " - file is open!";
            }
            file.close();
            number_of_folders++;
            dataCounter--;
        }

    }


    return a.exec();
}
