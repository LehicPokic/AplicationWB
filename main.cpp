/*
    1. Решить вопрос с числом папок, чтобы числа не уходили в миллиарды (решено)
    2. Решить вопрос со времеными метками
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


int countFilesInDirectory(const QString &path) {  //Подсчет числа файлов в папке
    QDir dir(path);
    int count = 0;

    // Устанавливаем фильтр для поиска всех файлов и директорий, исключая специальные папки "." и ".."
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    // Получаем список всех файлов и папок
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir()) {
            // Если это директория, рекурсивно вызываем функцию
            count += countFilesInDirectory(fileInfo.absoluteFilePath());
        } else {
            // Если это файл, увеличиваем счетчик
            ++count;
        }
    }

    return count;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int number_of_folders = 0; //Переменная отвечающая за число папок

    QString hostname = "Demo3";

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
        QFile filedevices("C:/Files/Project/WirenBoard_monitor/ApplicationCreate/release/release/files/devices.txt"); //Заполнение вектора devices
        if (!filedevices.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug() << "File devices.txt NOT OPEN!!! " /*<< argv[3] */ << filedevices.errorString();
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
            QModelIndex indexInt_id = devices->index(i, 0);
            QModelIndex indexControl = devices->index(i, 2);
            int_id.push_back(devices->data(indexInt_id).toInt());
            control.push_back(devices->data(indexControl).toString());
            qDebug() << devices->data(indexInt_id).toInt() << " " << devices->data(indexControl).toString();
            //qDebug() << devices->data(index2).toString();
        }

        SetModelFilter *filter = new SetModelFilter("data", int_id, sdb);

        data = filter->getModel();

        int dataCounter = data->rowCount();

        qDebug() << dataCounter;

        number_of_folders = countFilesInDirectory("C:/Files/Project/WirenBoard_monitor/ApplicationCreate/release/release/files/input");
        QString pathtoinput = "C:/Files/Project/WirenBoard_monitor/ApplicationCreate/release/release/files/input";
        while (dataCounter != 0) {
            QString path = QString(pathtoinput+"/input_file%1.txt").arg(number_of_folders+1);
            QFile file(path);
            if (!file.open(QIODevice::ReadWrite)) {
                    qDebug() << path << " - ERROR open!" << file.errorString();
                    return 1;
                }
            else {
                qDebug() << path << " - file is open!";
                QTextStream out(&file);
                for (int i = 0; i < 249; i++) {
                    if (dataCounter != 0) {
                        QModelIndex indexTime = data->index(i, 3);
                        QModelIndex indexValue = data->index(i, 2);
                        QModelIndex indexChannel = data->index(i, 1);
                        QSqlQuery query;
                        QString q = QString("SELECT control FROM channels JOIN data ON data.channel = channels.int_id WHERE data.channel = '%1'").arg(data->data(indexChannel).toInt());
                        query.exec(q);
                        query.first();
                        QString control = query.value(0).toString();
                        out << hostname << " " << control.replace(" ", "_") << " " << data->data(indexTime).toInt() << " " << data->data(indexValue).toDouble() << endl;
                        dataCounter--;
                    }
                }
                number_of_folders++;
                file.close();
            }
        }
    }
    return 0;
}
