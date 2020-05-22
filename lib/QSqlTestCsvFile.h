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
QT_FORWARD_DECLARE_CLASS(QSqlTestCsvFile2Private)
QT_FORWARD_DECLARE_CLASS(QString)
QT_FORWARD_DECLARE_CLASS(QStringList)

class QSqlTestCsvFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename)
    Q_PROPERTY(QStringList columnNames READ columnNames)
    Q_PROPERTY(QList<QVariantList> rows READ rows)

public:
    QSqlTestCsvFile(const QString &filename, QObject *parent = nullptr);
    QSqlTestCsvFile(QObject *parent = nullptr);

    QString filename() const;
    QStringList columnNames() const;
    QList<QVariantList> rows() const;

    QVariantList row(int index) const;
    QVariantList column(int index) const;
    QVariantList column(const QString &name) const;
    QVariant column(int rowIndex, int columnIndex) const;

signals:
    void error(const QString &err);
    void loaded();

public slots:
    bool load(const QString &filename);

private:
    QSqlTestCsvFilePrivate *d;
};

////////////////////////////////////////////////////////////////////////////////

class QSqlTestCsvFile2 : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName)
    Q_PROPERTY(QString lastError READ lastError)
public:
    QSqlTestCsvFile2(QObject *parent = nullptr);

    bool load(const QString &fileName);

    QString fileName() const;
    QString lastError() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal,
                        int role = Qt::DisplayRole) const override;

private:
    QSqlTestCsvFile2Private *d;
};

#endif
