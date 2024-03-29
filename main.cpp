#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <iostream>
#include <QSqlTableModel>
#include <QDir>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase sdb;
    sdb = QSqlDatabase::addDatabase("QSQLITE");

    sdb.setDatabaseName("C:/Files/data.db");           //"/var/lib/wirenboard/db"

    QSqlTableModel *devices;

    std::vector<std::string> device = {"wb_map12e_28", "WB-MSWv.3"};

    std::vector<int> int_id;

    if (!sdb.open()) {
        std::cout << "Error" << std::endl;
        return false;
    }
    else {

        devices = new QSqlTableModel(nullptr, sdb);
        devices->setTable("channels");
        devices->setEditStrategy(QSqlTableModel::OnManualSubmit);
        devices->select();
        devices->setFilter("device = 'wb-map12e_28' OR device = 'WB-MSWv.3'");
        for (int i = 0; i < devices->rowCount(); ++i) {
            int_id.push_back();
            std::cout << i << std::endl;
        }

    }


    return a.exec();
}
