#include <QString>
#include <QSqlTableModel>
#include <QDebug>


void setModelFilter(QSqlTableModel* model, const QVector<int>& ids) {
    if (model == nullptr || ids.isEmpty()) {
        qDebug() << "Модель не инициализирована или список ID пуст";
        return;
    }

    // Преобразование QVector<int> в строку для SQL запроса
    QStringList idListStrings;
    for (int id : ids) {
        idListStrings << QString::number(id);
    }
    QString filter = QString("id IN (%1)").arg(idListStrings.join(','));

    // Установка фильтра для модели
    model->setFilter(filter);
    model->select();

    qDebug() << "Фильтр установлен:" << filter;
}
