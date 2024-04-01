#include "setmodelfilter.h"

SetModelFilter::SetModelFilter(QString nameTable, const QVector<int> &ids,  QSqlDatabase db)
{
    model = new QSqlTableModel(nullptr, db);
    if (model == nullptr) {
        qDebug() << "The model is not initialized";
        return;
    }
    if (ids.isEmpty()) {
        qDebug() << "The ID list is emptyт";
        return;
    }

    qint64 now = QDateTime::currentDateTime().toSecsSinceEpoch(); //работаем со временем
    qint64 thirtySecondsAgo = now - 30;

    // Преобразование QVector<int> в строку для SQL запроса
    QStringList idListStrings;
    for (int id : ids) {
       idListStrings << QString::number(id);
    }
    QString filter = QString("channel IN (%1) AND timestamp <= %2 AND timestamp >= %3").arg(idListStrings.join(',')).arg(1711248412476).arg(1711248412446); //Поменять на переменные

    // Установка фильтра для модели
    model->setTable(nameTable);
    model->select();
    model->setFilter(filter);


    qDebug() << "Filter is select:" << filter;

}

QSqlTableModel* SetModelFilter::getModel()
{
    return model;
}


