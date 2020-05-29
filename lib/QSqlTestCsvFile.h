/**
 * @file QSqlTestCsvFile.h
 * @brief SQL table data for running tests.
 */

#ifndef QSQLTESTCSVFILE_H
#define QSQLTESTCSVFILE_H

#include <QObject>
#include <QAbstractTableModel>
#include <QVariant>

QT_FORWARD_DECLARE_CLASS(QSqlTestCsvFilePrivate)
QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QStringList)

class QSqlTestCsvFile : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName)
    Q_PROPERTY(QString lastError READ lastError)
public:
    QSqlTestCsvFile(QObject *parent = nullptr);

    bool load(const QString &fileName);

    QString fileName() const;
    QString lastError() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal,
                        int role = Qt::DisplayRole) const override;

private:
    QSqlTestCsvFilePrivate *d;
};

#endif
