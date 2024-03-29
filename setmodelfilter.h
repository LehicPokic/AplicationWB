#ifndef SETMODELFILTER_H
#define SETMODELFILTER_H

#include <QSqlTableModel>
#include <QDebug>
#include <QTime>


class SetModelFilter
{

    QSqlTableModel *model;


public:
    SetModelFilter(QString nameTable,const QVector<int>& ids, QSqlDatabase db);
    QSqlTableModel* getModelFilter();
};

#endif // SETMODELFILTER_H
