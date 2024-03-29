#include "setmodelfilter.h"

SetModelFilter::SetModelFilter(QString nameTable, const QVector<int> &ids,  QSqlDatabase db)
{
    model = new QSqlTableModel(nullptr, db);
    if (model == nullptr) {
        qDebug() << "Модель не инициализирована";
        return;
    }
    if (ids.isEmpty()) {
        qDebug() << "Список ID пуст";
        return;
    }

    qint64 now = QDateTime::currentDateTime().toSecsSinceEpoch(); //работаем со временем
    qint64 thirtySecondsAgo = now - 30;

    // Преобразование QVector<int> в строку для SQL запроса
    QStringList idListStrings;
    for (int id : ids) {
       idListStrings << QString::number(id);
    }
    QString filter = QString("channel IN (%1) AND timestamp <= %2 AND timestamp >= %3").arg(idListStrings.join(',')).arg(1711358093073).arg(1711358093043); //Поменять на переменные

    // Установка фильтра для модели
    model->setTable(nameTable);
    model->setFilter(filter);
    model->select();

    qDebug() << "Фильтр установлен:" << filter;

}

QSqlTableModel* SetModelFilter::getModel()
{
    return model;
}


